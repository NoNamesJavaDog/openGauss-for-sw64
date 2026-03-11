/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - transformIndexOptions
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * parse_utilcmd.cpp
 *	  Perform parse analysis work for various utility commands
 *
 * Formerly we did this work during parse_analyze() in analyze.c.  However
 * that is fairly unsafe in the presence of querytree caching, since any
 * database state that we depend on in making the transformations might be
 * obsolete by the time the utility command is executed; and utility commands
 * have no infrastructure for holding locks or rechecking plan validity.
 * Hence these functions are now called at the start of execution of their
 * respective utility commands.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2010-2012 Postgres-XC Development Group
 *
 *	src/common/backend/parser/parse_utilcmd.cpp
 *
 * -------------------------------------------------------------------------
 */
#include <string.h>
#include "postgres.h"
#include "knl/knl_variable.h"
#include "plugin_nodes/parsenodes_common.h"

#include "access/reloptions.h"
#include "access/gtm.h"
#include "access/sysattr.h"
#include "catalog/dependency.h"
#include "catalog/gs_column_keys.h"
#include "catalog/gs_encrypted_columns.h"
#include "plugin_catalog/heap.h"
#include "catalog/index.h"
#include "catalog/namespace.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_constraint.h"
#include "catalog/pg_enum.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_partition_fn.h"
#include "catalog/pg_type.h"
#include "catalog/pg_proc.h"
#include "commands/comment.h"
#include "plugin_commands/defrem.h"
#include "commands/sequence.h"
#include "commands/tablecmds.h"
#include "commands/tablespace.h"
#include "foreign/foreign.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "nodes/primnodes.h"
#include "nodes/value.h"
#include "optimizer/clauses.h"
#include "plugin_parser/analyze.h"
#include "plugin_parser/parse_clause.h"
#include "plugin_parser/parse_collate.h"
#include "plugin_parser/parse_expr.h"
#include "plugin_parser/parse_relation.h"
#include "plugin_parser/parse_target.h"
#include "plugin_parser/parse_type.h"
#include "plugin_parser/parse_utilcmd.h"
#include "plugin_parser/parse_oper.h"
#include "plugin_parser/parse_coerce.h"
#ifdef PGXC
#include "optimizer/pgxcship.h"
#include "pgstat.h"
#include "pgxc/groupmgr.h"
#include "pgxc/locator.h"
#include "pgxc/pgxc.h"
#include "plugin_optimizer/pgxcplan.h"
#include "optimizer/nodegroups.h"
#include "pgxc/execRemote.h"
#include "pgxc/redistrib.h"
#include "executor/node/nodeModifyTable.h"
#endif
#include "plugin_parser/parser.h"
#include "rewrite/rewriteManip.h"
#include "executor/node/nodeModifyTable.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/extended_statistics.h"
#include "utils/lsyscache.h"
#include "utils/rel.h"
#include "utils/rel_gs.h"
#include "utils/partitionkey.h"
#include "utils/syscache.h"
#include "utils/typcache.h"
#include "utils/numeric.h"
#include "utils/numeric_gs.h"
#include "utils/int16.h"
#include "plugin_mb/pg_wchar.h"
#include "gaussdb_version.h"
#include "gs_ledger/ledger_utils.h"
#include "client_logic/client_logic.h"
#include "client_logic/client_logic_enums.h"
#include "storage/checksum_impl.h"
#include "catalog/gs_collation.h"

/* State shared by transformCreateSchemaStmt and its subroutines */
typedef struct {
    const char* stmtType; /* "CREATE SCHEMA" or "ALTER SCHEMA" */
    char* schemaname;     /* name of schema */
    char* authid;         /* owner of schema */
    List* sequences;      /* CREATE SEQUENCE items */
    List* tables;         /* CREATE TABLE items */
    List* views;          /* CREATE VIEW items */
    List* indexes;        /* CREATE INDEX items */
    List* triggers;       /* CREATE TRIGGER items */
    List* grants;         /* GRANT items */
} CreateSchemaStmtContext;

#define ALTER_FOREIGN_TABLE "ALTER FOREIGN TABLE"
#define CREATE_FOREIGN_TABLE "CREATE FOREIGN TABLE"
#define ALTER_TABLE "ALTER TABLE"
#define CREATE_TABLE "CREATE TABLE"
#define TYPE_IS_ANONYMOUS_ENUM(typid, typname) (type_is_enum(atttypid) && strstr(typname, "anonymous_enum"))

#define RELATION_ISNOT_REGULAR_PARTITIONED(relation)                                                              \
    (((relation)->rd_rel->relkind != RELKIND_RELATION && (relation)->rd_rel->relkind != RELKIND_FOREIGN_TABLE && \
      (relation)->rd_rel->relkind != RELKIND_STREAM) || RelationIsNonpartitioned((relation)))

static void transformColumnDefinition(CreateStmtContext* cxt, ColumnDef* column, bool preCheck);
static void transformTableConstraint(CreateStmtContext* cxt, Constraint* constraint);
static void transformTableLikeClause(
    CreateStmtContext* cxt, TableLikeClause* table_like_clause, bool preCheck, bool isFirstNode, bool is_row_table, TransformTableType transformType);
/* decide if serial column should be copied under analyze */
static bool IsCreatingSeqforAnalyzeTempTable(CreateStmtContext* cxt);
static void transformTableLikePartitionProperty(Relation relation, HeapTuple partitionTableTuple, List** partKeyColumns,
    List* partitionList, List** partitionDefinitions);
static IntervalPartitionDefState* TransformTableLikeIntervalPartitionDef(HeapTuple partitionTableTuple);
static void transformTableLikePartitionKeys(
    Relation relation, HeapTuple partitionTableTuple, List** partKeyColumns, List** partKeyPosList);
static void transformTableLikePartitionBoundaries(
    Relation relation, List* partKeyPosList, List* partitionList, List** partitionDefinitions);
static void transformOfType(CreateStmtContext* cxt, TypeName* ofTypename);
static List* get_collation(Oid collation, Oid actual_datatype);
static List* get_opclass(Oid opclass, Oid actual_datatype);
static void checkPartitionValue(CreateStmtContext* cxt, CreateStmt* stmt);
static void TransformListDistributionValue(ParseState* pstate, Node** listDistDef, bool needCheck);
static void checkClusterConstraints(CreateStmtContext* cxt);
static void checkPsortIndexCompatible(IndexStmt* stmt);
static void checkCBtreeIndexCompatible(IndexStmt* stmt);
static void checkCGinBtreeIndexCompatible(IndexStmt* stmt);

static void checkReserveColumn(CreateStmtContext* cxt);
static void CheckDistributionSyntax(CreateStmt* stmt);
static void CheckListRangeDistribClause(CreateStmtContext *cxt, CreateStmt *stmt);

static void transformIndexConstraints(CreateStmtContext* cxt);
static void checkPartitionConstraintWithExpr(Constraint* con);
static void checkConditionForTransformIndex(
    Constraint* constraint, CreateStmtContext* cxt, Oid index_oid, Relation index_rel);
static IndexStmt* transformIndexConstraint(Constraint* constraint, CreateStmtContext* cxt, bool mustGlobal = false);
static void transformFKConstraints(CreateStmtContext* cxt, bool skipValidation, bool isAddConstraint);
static void transformConstraintAttrs(CreateStmtContext* cxt, List* constraintList);
#ifdef DOLPHIN
static void transformColumnType(CreateStmtContext* cxt, ColumnDef* column, AlterTableCmd* cmd);
#else
static void transformColumnType(CreateStmtContext* cxt, ColumnDef* column);
#endif
static void setSchemaName(char* context_schema, char** stmt_schema_name);
static void TransformTempAutoIncrement(ColumnDef* column, CreateStmt* stmt);
static int128 TransformAutoIncStart(CreateStmt* stmt);
#ifdef DOLPHIN
static void transformTableIndex(CreateStmtContext* cxt);
static void transformIndexNode(IndexStmt* index, CreateStmtContext* cxt, bool mustGlobal);
static void CreatePartitionKeyFromIndexConstraints(PartitionState* partTableState, List* ixconstraints);
static void TransfromSortByNulls(IndexStmt* index);
#endif
extern void pg_ulltoa(uint64 value, char *a);
/*
 * @hdfs
 * The following three functions are used for HDFS foreign talbe constraint.
 */
static void setInternalFlagIndexStmt(List* IndexList);
static void checkInformationalConstraint(Node* node, bool isForeignTbl);
static void checkConstraint(CreateStmtContext* cxt, Node* node);
static void setMemCheckFlagForIdx(List* IndexList);

/* check partition name */
static void check_partition_name_internal(List* partitionList, bool isPartition);
static void check_partition_name_start_end(List* partitionList, bool isPartition);

/* for range partition: start/end syntax */
static void precheck_start_end_defstate(List* pos, FormData_pg_attribute* attrs,
    RangePartitionStartEndDefState* defState, bool isPartition);
static Datum get_partition_arg_value(Node* node, bool* isnull);
static Datum evaluate_opexpr(
    ParseState* pstate, List* oprname, Node* leftarg, Node* rightarg, Oid* restypid, int location);
static Const* coerce_partition_arg(ParseState* pstate, Node* node, Oid targetType);
static Oid choose_coerce_type(Oid leftid, Oid rightid);
static void get_rel_partition_info(Relation partTableRel, List** pos, Const** upBound);
static void get_src_partition_bound(Relation partTableRel, Oid srcPartOid, Const** lowBound, Const** upBound);
static Oid get_split_partition_oid(Relation partTableRel, SplitPartitionState* splitState);
static List* add_range_partition_def_state(List* xL, List* boundary, const char* partName, const char* tblSpaceName);
static bool CheckStepInRange(ParseState *pstate, Const *startVal, Const *endVal, Const *everyVal,
    Form_pg_attribute attr);
static List* divide_start_end_every_internal(ParseState* pstate, char* partName, Form_pg_attribute attr,
    Const* startVal, Const* endVal, Node* everyExpr, int* numPart,
    int maxNum, bool isinterval, bool needCheck, bool isPartition);
static List* DividePartitionStartEndInterval(ParseState* pstate, Form_pg_attribute attr, char* partName,
    Const* startVal, Const* endVal, Const* everyVal, Node* everyExpr, int* numPart, int maxNum, bool isPartition);
extern Node* makeAConst(Value* v, int location);
static bool IsElementExisted(List* indexElements, IndexElem* ielem);
static char* CreatestmtGetOrientation(CreateStmt *stmt);
static void CheckAutoIncrementIndex(CreateStmtContext *cxt);
static List* semtc_generate_hash_partition_defs(CreateStmt* stmt, const char* prefix_name, int part_count);
static void TransformModifyColumndef(CreateStmtContext* cxt, AlterTableCmd* cmd);
static void TransformColumnDefinitionOptions(CreateStmtContext* cxt, ColumnDef* column);
static void TransformColumnDefinitionConstraints(
    CreateStmtContext* cxt, ColumnDef* column, bool preCheck, bool is_modify);
#define REDIS_SCHEMA "data_redis"

/*
 * transformCreateStmt -
 *	  parse analysis for CREATE TABLE
 *
 * Returns a List of utility commands to be done in sequence.  One of these
 * will be the transformed CreateStmt, but there may be additional actions
 * to be done before and after the actual DefineRelation() call.
 *
 * SQL92 allows constraints to be scattered all over, so thumb through
 * the columns and collect all constraints into one place.
 * If there are any implied indices (e.g. UNIQUE or PRIMARY KEY)
 * then expand those into multiple IndexStmt blocks.
 *	  - thomas 1997-12-02
 */



Oid check_collation_by_charset(const char* collate, int charset) {return 0;}


/*
 * transform_default_collation -
 *      Returns the processed collation oid of schema, relation or attribute level.
 */
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#else
#endif
#ifndef DOLPHIN
#endif
#ifndef DOLPHIN
#endif



#ifdef DOLPHIN



#endif



#ifdef DOLPHIN
#endif
#ifdef PGXC
#endif
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifdef PGXC
#endif
#ifdef DOLPHIN
#endif
#ifdef PGXC
#else
#endif
#ifdef PGXC
#endif
#ifdef DOLPHIN
#endif
#ifdef PGXC
#ifdef ENABLE_MOT
#else
#endif
#endif





/*
 * createSetOwnedByTable -
 *		create a set owned by table, need to add record to pg_depend.
 */

#ifdef DOLPHIN
/* drop type that is owned by table column, such as set or enum */
#endif
#ifdef DOLPHIN
#endif

#ifdef DOLPHIN

#endif

/*
 * createSeqOwnedByTable -
 *		create a sequence owned by table, need to add record to pg_depend.
 *		used in CREATE TABLE and CREATE TABLE ... LIKE
 */
#ifdef DOLPHIN
#endif
#ifdef PGXC
#endif
#ifdef ENABLE_MUTIPLE_NODES
#endif
#ifdef PGXC
#endif





/*
 * precheckColumnTypeForSet -
 *  not support empty set type and set array.
 */


/*
 * transformColumnDefinition -
 *		transform a single ColumnDef within CREATE TABLE
 *		Also used in ALTER TABLE ADD COLUMN
 */
#ifdef DOLPHIN
#endif
#ifdef ENABLE_MULTIPLE_NODES
#endif
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif
#ifdef DOLPHIN
#else
#endif
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif
#ifdef DOLPHIN
#else
#endif





/*
 * transformTableConstraint
 *		transform a Constraint node within CREATE TABLE or ALTER TABLE
 */


/*
 * searchSeqidFromExpr
 *
 * search default expression for sequence oid.
 */


/*
 * checkTableLikeSequence
 *
 * Analyze default expression of table column, if default is nextval function,
 * it means the first argument of nextval function is sequence, we need to
 * check whether the sequence exists in current datanode.
 * We check sequence oid only because stringToNode in transformTableLikeFromSerialData
 * has already checked sequence name (see _readFuncExpr in readfuncs.cpp).
 * Suppose create a table like this:  CREATE TABLE t1 (id serial, a int) TO NODE GROUP ng1;
 * a sequence named t1_id_seq will be created and the sequence exists in NodeGroup ng1.
 * If create table like t1 in another NodeGroup ng2, error will be reported because t1_id_seq
 * does not exists some datanodes of NodeGroup ng2.
 */



/*
 * transformTableLikeFromSerialData
 *
 * Get meta info of a table from serialized data, the serialized data come from CN.
 * The function is used for CREATE TABLE ... LIKE across node group.
 */


/*
 * Get all tag for tsdb
 */



/*
 * Support Create table like on table with subpartitions
 * In transform phase, we need fill PartitionState
 * 1. Recursively fill partitionList in PartitionState also including subpartitionList
 * 2. Recursively fill PartitionState also including SubPartitionState
 */


/*
 * transformTableLikeClause
 *
 * Change the LIKE <srctable> portion of a CREATE TABLE statement into
 * column definitions which recreate the user defined column portions of
 * <srctable>.
 */
#ifdef DOLPHIN
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif

// this function is used to output 2 list,
// one for partitionkey, a list of column ref,
// another for partiton boundary, a list of















/*
 * Generate an IndexStmt node using information from an already existing index
 * "source_idx".  Attribute numbers should be adjusted according to attmap.
 */


/*
 * get_collation		- fetch qualified name of a collation
 *
 * If collation is InvalidOid or is the default for the given actual_datatype,
 * then the return value is NIL.
 */


/*
 * get_opclass			- fetch qualified name of an index operator class
 *
 * If the opclass is the default for the given actual_datatype, then
 * the return value is NIL.
 */






#ifdef DOLPHIN /* ChooseIndexNameAddition and ChooseIndexName are copy from indexcmds.cpp */
/*
 * Generate "name2" for a new index given the list of column names for it
 * (as produced by ChooseIndexColumnNames).  This will be passed to
 * ChooseRelationName along with the parent table name and a suitable label.
 *
 * We know that less than NAMEDATALEN characters will actually be used,
 * so we can truncate the result once we've generated that many.
 */


/*
 * Select the name to be used for an index.
 *
 * The argument list is pretty ad-hoc :-(
 */


/*
* For create index in CREATE TABLE/ ALTER TABLE stmt, we just have a list of column names and expressions.
*
* Make sure referenced columns exist. (Although we could leave
* it to DefineIndex to mark the columns NOT NULL, it's more efficient to
* get it right the first time.)
*/




#ifdef DOLPHIN
#else
#endif

char* transformIndexOptions(List* list)
{
    char* method = NULL;
    ListCell* cell = NULL;
    int first = 0;

    /* check every access method name in index_options, if not exist, return error */
    foreach (cell, list) {
        void* pointer = lfirst(cell);
        if (IsA(pointer, A_Const)) {
            A_Const* con = (A_Const*) pointer;
            if (IsA((Node*)&con->val, String)) {
                /* return the last access method name */
                char* am = strVal(&con->val);
                if (am && get_am_oid(am, false) && first < 1) {
                    method = am;
                    first++;
                }
            }
        }
    }
    return method;
}
#endif

/*
 * transformIndexConstraints
 *		Handle UNIQUE, PRIMARY KEY, EXCLUDE constraints, which create indexes.
 *		We also merge in any index definitions arising from
 *		LIKE ... INCLUDING INDEXES.
 */


/*
 * If it's ALTER TABLE ADD CONSTRAINT USING INDEX,
 * verify the index is usable.
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * transformIndexConstraint
 *		Transform one UNIQUE, PRIMARY KEY, or EXCLUDE constraint for
 *		transformIndexConstraints.
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif
#ifdef DOLPHIN
#else
#endif
#ifdef PGXC
#endif
#ifdef DOLPHIN
#else
#endif

/*
 * transformFKConstraints
 *		handle FOREIGN KEY constraints
 */
#ifdef PGXC
#endif

/*
 * transformIndexStmt - parse analysis for CREATE INDEX and ALTER TABLE
 *
 * Note: this is a no-op for an index not using either index expressions or
 * a predicate expression.	There are several code paths that create indexes
 * without bothering to call this, because they know they don't have any
 * such expressions to deal with.
 */
#ifdef ENABLE_MOT
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif



/*
 * transformRuleStmt -
 *	  transform a CREATE RULE Statement. The action is a list of parse
 *	  trees which is transformed into a list of query trees, and we also
 *	  transform the WHERE clause if any.
 *
 * actions and whereClause are output parameters that receive the
 * transformed results.
 */
#ifdef PGXC
#endif

#ifdef DOLPHIN



#endif

/*
 * transformAlterTableStmt -
 *		parse analysis for ALTER TABLE
 *
 * Returns a List of utility commands to be done in sequence.  One of these
 * will be the transformed AlterTableStmt, but there may be additional actions
 * to be done before and after the actual AlterTable() call.
 */
#ifdef DOLPHIN
#endif
#ifdef PGXC
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef ENABLE_MOT
#else
#endif

/*
 * Preprocess a list of column constraint clauses
 * to attach constraint attributes to their primary constraint nodes
 * and detect inconsistent/misplaced constraint attributes.
 *
 * NOTE: currently, attributes are only supported for FOREIGN KEY, UNIQUE,
 * EXCLUSION, and PRIMARY KEY constraints, but someday they ought to be
 * supported for other constraint types.
 */
#define SUPPORTS_ATTRS(node)                                                                     \
    ((node) != NULL && ((node)->contype == CONSTR_PRIMARY || (node)->contype == CONSTR_UNIQUE || (node)->contype == CONSTR_CHECK || \
                           (node)->contype == CONSTR_EXCLUSION || (node)->contype == CONSTR_FOREIGN))

/**
 * tableof type is not  supported be a column in a table
 * @param ctype ctype
 */


/*
 * Special handling of type definition for a column
 */
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif

/*
 * transformCreateSchemaStmt -
 *	  analyzes the CREATE SCHEMA statement
 *
 * Split the schema element list into individual commands and place
 * them in the result list in an order such that there are no forward
 * references (e.g. GRANT to a table created later in the list). Note
 * that the logic we use for determining forward references is
 * presently quite incomplete.
 *
 * SQL92 also allows constraints to make forward references, so thumb through
 * the table columns and move forward references to a posterior alter-table
 * command.
 *
 * The result is a list of parse nodes that still need to be analyzed ---
 * but we can't analyze the later commands until we've executed the earlier
 * ones, because of possible inter-object references.
 *
 * Note: this breaks the rules a little bit by modifying schema-name fields
 * within passed-in structs.  However, the transformation would be the same
 * if done over, so it should be all right to scribble on the input to this
 * extent.
 */


/*
 * setSchemaName
 *		Set or check schema name in an element of a CREATE SCHEMA command
 */












/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		: check synax for range partition defination
 * Description	:
 * Notes		:
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		: check partition value
 * Description	:
 * Notes		: partition key value must be const or const-evaluable expression
 */


/*
 * check_partition_name_internal
 *  check partition name with less/than stmt.
 *
 * [IN] partitionList: partition list
 *
 * RETURN: void
 */


/*
 * check_partition_name_start_end
 *  check partition name with start/end stmt.
 *
 * [IN] partitionList: partition list
 *
 * RETURN: void
 */


/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		: check partition name
 * Description	: duplicate partition name is not allowed
 * Notes		:
 */














/* get storage type from CreateStmt.optitions, which is generated by WITH clause */
#ifdef ENABLE_FINANCE_MODE
#endif









/*
 * Check partial cluster key constraints
 */


/*
 * Check reserve column
 */







    










/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		:
 * Description	:
 * Input		:
 * Output	:
 * Return		:
 * Notes		:
 */
#ifdef DOLPHIN
#endif
/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		:
 * Description	:
 * Notes		:
 */


/*
 * @hdfs
 * Brief        : set informational constraint flag in IndexStmt.
 * Description  : Set indexStmt's internal_flag. This flag will be set to false
 *                if indexStmt is built by "Creat index", otherwise be set to true.
 * Input        : the IndexStmt list.
 * Output       : none.
 * Return Value : none.
 * Notes        : This function is only used for HDFS foreign table.
 */


/*
 * Brief        : Check the foreign table constraint type.
 * Description  : This function checks HDFS foreign table constraint type. The supported constraint
 *                types and some useful comment are:
 *                1. Only the primary key, unique, not null and null will be supported.
 *                2. Only "NOT ENFORCED" clause is supported for HDFS foreign table informational constraint.
 *                3. Multi-column combined informational constraint is forbidden.
 * Input        : node, the node needs to be checked.
 * Output       : none.
 * Return Value : none.
 * Notes        : none.
 */


/*
 * @Description: Check Constraint.
 * @in cxt: CreateStmtContext or AlterTableStmt struct.
 * @in node: Constraint or ColumnDef.
 */


/*
 * @Description: set skip mem check flag for index stmt. If the
 *	index is created just after table creation, we will not do
 *	memory check and adaption
 * @in IndexList: index list after table creation
 */


/*
 * add_range_partition_def_state
 * 	add one partition def state into a List
 *
 * [IN] xL: List to be appended
 * [IN] boundary: a list of the end point (list_length must be 1)
 * [IN] partName: partition name
 * [IN] tblSpaceName: tablespace name
 *
 * RETURN: the partitionDefState List
 */


/*
 * get_range_partition_name_prefix
 * 	get partition name's prefix
 *
 * [out] namePrefix: an array of length NAMEDATALEN to store name prefix
 * [IN] srcName: src name
 * [IN] printNotice: print notice or not, default false
 *
 * RETURN: void
 */


/* get_rel_partition_info
 * 	get detail info of a partition rel
 *
 * [IN] partTableRel: partition relation
 * [OUT] pos: position of the partition key
 * [OUT] upBound: up boundary of last partition
 *
 * RETURN: void
 */


/* get_src_partition_bound
 * 	get detail info of a partition rel
 *
 * [IN] partTableRel: partition relation
 * [IN] srcPartOid: src partition oid
 * [OUT] lowBound: low boundary of the src partition
 * [OUT] upBound: up boundary of the src partition
 *
 * RETURN: void
 */


/* get_split_partition_oid
 * 	get oid of the split partition
 *
 * [IN] partTableRel: partition relation
 * [IN] splitState: split partition state
 *
 * RETURN: oid of the partition to be splitted
 */


#define precheck_point_value_internal(a, isPartition)                                                                 \
    do {                                                                                                              \
        Node* pexpr = (Node*)linitial(a);                      /* original value */                                   \
        Const* pval = GetPartitionValue(pos, attrs, a, false, isPartition); /* cast(ori)::int */                      \
        if (!pval->ismaxvalue) {                                                                                      \
            Const* c = (Const*)evaluate_expr((Expr*)pexpr, exprType(pexpr), exprTypmod(pexpr), exprCollation(pexpr)); \
            if (partitonKeyCompare(&pval, &c, 1) != 0)                                                                \
                ereport(ERROR,                                                                                        \
                    (errcode(ERRCODE_INVALID_TABLE_DEFINITION),                                                       \
                        errmsg("start/end/every value must be an const-integer for %s \"%s\"",                        \
                            (isPartition ? "partition" : "slice"), defState->partitionName)));                        \
        }                                                                                                             \
    } while (0)

/*
 * precheck_start_end_defstate
 *    precheck start/end value of a range partition defstate
 */


/* is_start_end_def_list
 * 	check the partition state and return the type of state
 * 	true: start/end stmt; false: less/than stmt
 *
 * [IN] state: partition state
 *
 * RETURN: if it is start/end stmt
 */


/*
 * get_partition_arg_value
 * 	Get the actual value from the expression. There are only a limited range
 * 	of cases we must cover because the parser guarantees constant input.
 *
 * [IN] node: input node expr
 * [out] isnull: indicate the NULL of result datum
 *
 * RETURN: a datum produced by node
 */


/*
 * evaluate_opexpr
 * Evaluate a basic operator expression from a partitioning specification.
 * The expression will only be an op expr but the sides might contain
 * a coercion function. The underlying value will be a simple constant,
 * however.
 *
 * If restypid is non-NULL and *restypid is set to InvalidOid, we tell the
 * caller what the return type of the operator is. If it is anything but
 * InvalidOid, coerce the operation's result to that type.
 *
 * [IN] pstate: parser state
 * [IN] oprname: name of opreator which can be <, +, -, etc.
 * [IN] leftarg: left arg
 * [IN] rightarg: right arg
 * [IN/OUT] restypid: result type id, if given coerce to it, otherwise return the compute result-type.
 * [IN] location: location of the expr, not necessary
 *
 * RETURN: a datum produced by the expr: "leftarg oprname rightarg"
 */


/*
 * coerce_partition_arg
 * 	coerce a partition parameter (start/end/every) to targetType
 *
 * [IN] pstate: parse state
 * [IN] node: Node to be coerced
 * [IN] targetType: target type
 *
 * RETURN: a const
 */


/*
 * choose_coerce_type
 * 	choose a coerce type
 * Note: this function may help us to fix ambiguous problem
 */


/* check if everyVal <= endVal - startVal, but think of overflow, we should be careful */


/*
 * divide_start_end_every_internal
 * 	internal implementaion for dividing an interval indicated by any-datatype
 * 	for example:
 * 	-- start(1) end(100) every(30)
 * 	-- start(123.01) end(345.09) every(111.99)
 * 	-- start('12-01-2012') end('12-05-2018') every('1 year')
 *
 * If (end-start) is divided by every with a remainder, then last partition is smaller
 * than others.
 *
 * [IN] pstate: parse state
 * [IN] partName: partition name
 * [IN] attr: pg_attribute of the target type
 * [IN] startVal: start value
 * [IN] endVal: end value
 * [IN] everyVal: interval value
 * [OUT] numPart: number of partitions
 * [IN] maxNum: max partition number allowed
 * [IN] isinterval: if EVERY is a interval value
 *
 * RETURN: end points of all sub-intervals
 */




/*
 * DividePartitionStartEndInterval
 * 	divide the partition interval of start/end into specified sub-intervals
 *
 * [IN] pstate: parse state
 * [IN] attr: pg_attribute
 * [IN] partName: partition name
 * [IN] startVal: start value
 * [IN] endVal: end value
 * [IN] everyVal: interval value
 * [OUT] numPart: number of partitions
 * [IN] maxNum: max partition number allowed
 *
 * RETURN: end points of all sub-intervals
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

#define add_last_single_start_partition                                                                              \
    do {                                                                                                             \
        Const* laststart = NULL;                                                                                     \
        /* last DefState is a single START, so add the last partition here */                                        \
        Assert(lastState->startValue && !lastState->endValue);                                                       \
        pnt = (Const*)copyObject(startVal);                                                                          \
        boundary = list_make1(pnt);                                                                                  \
        laststart = GetPartitionValue(pos, attrs, lastState->startValue, false, isPartition);                        \
        if (partitonKeyCompare(&laststart, &startVal, 1) >= 0)                                                       \
            ereport(ERROR,                                                                                           \
                (errcode(ERRCODE_INVALID_TABLE_DEFINITION),                                                          \
                    errmsg("start value of %s \"%s\" is too low.",                                                   \
                        (isPartition ? "partition" : "slice"), defState->partitionName),                             \
                    errhint("%s gap or overlapping is not allowed.",                                                 \
                                     (isPartition ? "partition" : "slice"))));                                       \
        if (lowBound == NULL && curDefState == 1) {                                                                  \
            /* last single START is the first DefState and MINVALUE is included */                                   \
            get_range_partition_name_prefix(namePrefix, lastState->partitionName, false, isPartition);               \
            ret = sprintf_s(partName, sizeof(partName), "%s_%d", namePrefix, 1);                                     \
            securec_check_ss(ret, "\0", "\0");                                                                       \
            newPartList = add_range_partition_def_state(newPartList, boundary, partName, lastState->tableSpaceName); \
            totalPart++;                                                                                             \
        } else {                                                                                                     \
            newPartList = add_range_partition_def_state(                                                             \
                newPartList, boundary, lastState->partitionName, lastState->tableSpaceName);                         \
            totalPart++;                                                                                             \
        }                                                                                                            \
        pfree_ext(laststart);                                                                                        \
    } while (0)

/*
 * transformRangePartStartEndStmt
 * 	entry of transform range partition which is defined by "start/end" syntax
 *
 * [IN] pstate: parse state
 * [IN] partitionList: partition list to be rewrote
 * [IN] attrs: pg_attribute item
 * [IN] pos: position of partition key in ColDef
 * [IN] existPartNum: number of partitions already exists
 * [IN] lowBound: low-boundary of all paritions
 * [IN] upBound: up-boundary of all partitions
 * [IN] needFree: free input partitionList or not, true: free, false: not
 * [IN] isPartition: true when checking partition definition, false when checking list/range distribution definition.
 *
 * lowBound/upBound rules:
 *
 *                                lowBound                       upBound
 *   not-NULL:    check SP == lowBound      check EP == upBound
 *                                                        (START) include upBound
 *
 *         NULL:         include MINVALUE      (START) include MAXVALUE
 * SP: first start point of the def; EP: final end point of the def
 * (START) include xxx: for a single start as final clause, include xxx.
 *
 * -- CREATE TABLE PARTITION: lowBound=NULL, upBound=NULL
 * -- ADD PARTITION: lowBound=ExistUpBound, upBound=NULL
 * -- SPLIT PARTITION: lowBound=CurrentPartLowBound, upBound=CurrentPartUpBound
 *
 * RETURN: a new partition list (wrote by "less/than" syntax).
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

/*
 * Check if CreateStmt contains TableLikeClause, and the table to be defined is
 * on different nodegrop with the parent table.
 *
 * CreateStmt: the Stmt need check.
 */


/*
 * Check if the parent table and the table to be define in the same cluseter.
 * oid : the parent Table OID
 * subcluster: the new table where to create
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif



#ifdef DOLPHIN
#else
#endif





/*
 * semtc_generate_hash_partition_defs
 *     Generate a specified number of partition definitions.
 * prefix_name: parent partition name for subpartition, used as a prefix for subpartition names to be generated
 * part_count: number of partitions to be generated
 */


#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#else
#endif
 

 
#ifndef DOLPHIN
#endif

#ifdef DOLPHIN
/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		: Create a partition key from index constraints.
 * Description	: This function takes a `PartitionState` object and a list of index constraints (`ixconstraints`),
 * and creates a partition key based on the primary key constraint. If no primary keys are found or a composite
 * primary key is found, an error is thrown.
 * Notes		: This function is used when no partition key is explicitly provided.
 */

#endif