/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - quote_qualified_identifier
 * - quote_identifier
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * ruleutils.c
 *	  Functions to convert stored expressions/querytrees back to
 *	  source text
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/ruleutils.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"
#include "plugin_postgres.h"

#include <fcntl.h>

#ifndef WIN32
#include <sys/syscall.h>
#endif

#ifdef PGXC
#include "access/reloptions.h"
#endif /* PGXC */
#include "access/sysattr.h"
#include "catalog/dependency.h"
#include "catalog/indexing.h"
#include "catalog/pg_attrdef.h"
#include "catalog/pg_authid.h"
#ifdef PGXC
#include "catalog/pg_aggregate.h"
#endif /* PGXC */
#include "catalog/pg_collation.h"
#include "catalog/pg_constraint.h"
#include "catalog/pg_depend.h"
#include "catalog/pg_enum.h"
#include "catalog/pg_language.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_partition.h"
#include "catalog/pg_partition_fn.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_rewrite.h"
#include "catalog/pg_synonym.h"
#include "catalog/pg_trigger.h"
#include "catalog/pg_type.h"
#include "catalog/pg_set.h"
#include "catalog/pg_object.h"
#include "plugin_catalog/heap.h"
#include "catalog/gs_encrypted_proc.h"
#include "catalog/gs_encrypted_columns.h"
#include "catalog/gs_package.h"
#include "catalog/pg_proc_ext.h"
#include "commands/comment.h"
#include "commands/defrem.h"
#include "commands/tablespace.h"
#include "commands/tablecmds.h"
#include "executor/spi.h"
#include "executor/spi_priv.h"
#include "funcapi.h"
#include "miscadmin.h"
#ifdef PGXC
#include "nodes/execnodes.h"
#include "catalog/pgxc_class.h"
#endif
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "nodes/parsenodes_common.h"
#include "optimizer/clauses.h"
#include "optimizer/tlist.h"
#include "plugin_parser/keywords.h"
#include "plugin_parser/parse_agg.h"
#include "plugin_parser/parse_func.h"
#include "plugin_parser/parse_hint.h"
#include "plugin_parser/parse_oper.h"
#include "plugin_parser/parse_type.h"
#include "plugin_parser/parser.h"
#include "plugin_parser/parsetree.h"
#include "plugin_parser/parse_expr.h"
#include "plugin_parser/parse_utilcmd.h"
#include "plugin_parser/parse_relation.h"
#ifdef PGXC
#include "pgxc/pgxc.h"
#include "plugin_optimizer/pgxcplan.h"
#include "optimizer/pgxcship.h"
#endif
#include "rewrite/rewriteHandler.h"
#include "rewrite/rewriteManip.h"
#include "rewrite/rewriteSupport.h"
#include "utils/acl.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/fmgroids.h"
#include "utils/lsyscache.h"
#include "utils/plpgsql.h"
#include "utils/rel.h"
#include "utils/rel_gs.h"
#include "utils/syscache.h"
#include "utils/snapmgr.h"
#include "utils/typcache.h"
#include "utils/xml.h"
#include "vecexecutor/vecnodes.h"
#include "db4ai/gd.h"
#include "commands/sqladvisor.h"
#include "commands/sequence.h"
#include "client_logic/client_logic.h"

#ifdef DOLPHIN
#include "plugin_commands/mysqlmode.h"
#endif

/* ----------
 * Pretty formatting constants
 * ----------
 */

/* Indent counts */
#define PRETTYINDENT_STD 8
#define PRETTYINDENT_JOIN 13
#define PRETTYINDENT_JOIN_ON (PRETTYINDENT_JOIN - PRETTYINDENT_STD)
#define PRETTYINDENT_VAR 4

/* Pretty flags */
#define PRETTYFLAG_PAREN 1
#define PRETTYFLAG_INDENT 2

/* Standard conversion of a "bool pretty" option to detailed flags */
#define GET_PRETTY_FLAGS(pretty) \
    ((pretty) ? (PRETTYFLAG_PAREN | PRETTYFLAG_INDENT) \
     : PRETTYFLAG_INDENT)

/* Default line length for pretty-print wrapping */
#define WRAP_COLUMN_DEFAULT 79

/* macro to test if pretty action needed */
#define PRETTY_PAREN(context) ((context)->prettyFlags & PRETTYFLAG_PAREN)
#define PRETTY_INDENT(context) ((context)->prettyFlags & PRETTYFLAG_INDENT)
#define MAXFLOATWIDTH 64
#define MAXDOUBLEWIDTH 128

#define atooid(x) ((Oid)strtoul((x), NULL, 10))

#define MAXINT128LEN 45

#define UNIQUE_OFFSET 7
#define PRIMARY_KEY_OFFSET 11

#ifdef DOLPHIN
#define  BEGIN_P_STR      " BEGIN_B_PROC " /* used in dolphin type proc body*/
#define  BEGIN_P_LEN      14
#define  BEGIN_N_STR      "    BEGIN     " /* BEGIN_P_STR to same length*/
#define  ARRAY_CONTAIN_FUNC_OID 2748
#define  ARRAY_REMOVE_FUNC_OID 6555
#endif
/* ----------
 * Local data types
 * ----------
 */

/* Context info needed for invoking a recursive querytree display routine */
typedef struct {
    StringInfo buf;           /* output buffer to append to */
    List* namespaces;         /* List of deparse_namespace nodes */
    List* windowClause;       /* Current query level's WINDOW clause */
    List* windowTList;        /* targetlist for resolving WINDOW clause */
    unsigned int prettyFlags; /* enabling of pretty-print functions */
    int wrapColumn;           /* max line length, or -1 for no limit */
    int indentLevel;          /* current indent level for prettyprint */
    bool varprefix;           /* TRUE to print prefixes on Vars */
#ifdef PGXC
    bool finalise_aggs;   /* should Datanode finalise the aggregates? */
    bool sortgroup_colno; /* instead of expression use resno for sortgrouprefs. */
    void* parser_arg;     /* dynamic variables */
#endif                    /* PGXC */
    bool qrw_phase;       /* for qrw phase, we support more deparse rule */
    bool viewdef;         /* just for dump viewdef */
    bool is_fqs;          /* just for fqs query */
    bool is_upsert_clause;  /* just for upsert clause */
    bool skip_lock;       /* no need to lock relation for invalid view */
} deparse_context;

/*
 * Each level of query context around a subtree needs a level of Var namespace.
 * A Var having varlevelsup=N refers to the N'th item (counting from 0) in
 * the current context's namespaces list.
 *
 * The rangetable is the list of actual RTEs from the query tree, and the
 * cte list is the list of actual CTEs.
 *
 * When deparsing plan trees, there is always just a single item in the
 * deparse_namespace list (since a plan tree never contains Vars with
 * varlevelsup > 0).  We store the PlanState node that is the immediate
 * parent of the expression to be deparsed, as well as a list of that
 * PlanState's ancestors.  In addition, we store its outer and inner subplan
 * state nodes, as well as their plan nodes' targetlists, and the indextlist
 * if the current PlanState is an IndexOnlyScanState.  (These fields could
 * be derived on-the-fly from the current PlanState, but it seems notationally
 * clearer to set them up as separate fields.)
 */
typedef struct {
    List* rtable; /* List of RangeTblEntry nodes */
    List* ctes;   /* List of CommonTableExpr nodes */
    /* Remaining fields are used only when deparsing a Plan tree: */
    PlanState* planstate;       /* immediate parent of current expression */
    List* ancestors;            /* ancestors of planstate */
    PlanState* outer_planstate; /* outer subplan state, or NULL if none */
    PlanState* inner_planstate; /* inner subplan state, or NULL if none */
    List* outer_tlist;          /* referent for OUTER_VAR Vars */
    List* inner_tlist;          /* referent for INNER_VAR Vars */
    List* index_tlist;          /* referent for INDEX_VAR Vars */
#ifdef PGXC
    bool remotequery; /* deparse context for remote query */
#endif
} deparse_namespace;

/* ----------
 * Global data
 * ----------
 */



typedef struct tableInfo {
    int1 relcmpr;
    char relkind;
    char relpersistence;
    char parttype;
    bool relrowmovement;
    bool hasindex;
    bool hasPartialClusterKey;
    Oid tablespace;
    Oid spcid;
    char* reloptions;
    char* relname;
    AttrNumber autoinc_attnum;
    Oid autoinc_consoid;
    Oid autoinc_seqoid;
    Oid collate;
#ifdef DOLPHIN
    Oid autoinc_idxoid;
#endif
} tableInfo;

typedef struct SubpartitionInfo {
    bool issubpartition;
    char subparttype; /* subpartition type, 'r'/'l'/'h' */
    Oid subparentid;
    Oid subpartkeytype; /* the typeid of subpartkey */
    AttrNumber attnum; /* the attribute number of subpartkey in the relation */
    bool istypestring;
} SubpartitionInfo;

/* ----------
 * Local functions
 *
 * Most of these functions used to use fixed-size buffers to build their
 * results.  Now, they take an (already initialized) StringInfo object
 * as a parameter, and append their text output to its contents.
 * ----------
 */
static char* deparse_expression_pretty(
    Node* expr, List* dpcontext, bool forceprefix, bool showimplicit, int prettyFlags, int startIndent,
    bool no_alias = false);
extern char* pg_get_viewdef_worker(Oid viewoid, int prettyFlags, int wrapColumn);
extern char* pg_get_functiondef_worker(Oid funcid, int* headerlines);
extern char* pg_get_trigger_whenclause(Form_pg_trigger trigrec,Node* whenClause, bool pretty);
static char* pg_get_triggerdef_worker(Oid trigid, bool pretty);
static void decompile_column_index_array(Datum column_index_array, Oid relId, StringInfo buf);
static char* pg_get_ruledef_worker(Oid ruleoid, int prettyFlags);
static char* pg_get_indexdef_worker(Oid indexrelid, int colno, const Oid* excludeOps, bool attrsOnly, bool showTblSpc,
    int prettyFlags, bool dumpSchemaOnly = false, bool showPartitionLocal = true, bool showSubpartitionLocal = true);
void pg_get_indexdef_partitions(Oid indexrelid, Form_pg_index idxrec, bool showTblSpc, StringInfoData *buf,
    bool dumpSchemaOnly, bool showPartitionLocal, bool showSubpartitionLocal);
static char* pg_get_constraintdef_worker(Oid constraintId, bool fullCommand, int prettyFlags, bool with_option=false);
static text* pg_get_expr_worker(text* expr, Oid relid, const char* relname, int prettyFlags);
static int print_function_arguments(StringInfo buf, HeapTuple proctup, bool print_table_args, bool print_defaults);
static void print_function_ora_arguments(StringInfo buf, HeapTuple proctup);
static void print_function_rettype(StringInfo buf, HeapTuple proctup);
static void print_parallel_enable(StringInfo buf, HeapTuple procTup, int2 parallelCursorSeq, Oid funcid);
static void set_deparse_planstate(deparse_namespace* dpns, PlanState* ps);
#ifdef PGXC
static void set_deparse_plan(deparse_namespace* dpns, Plan* plan);
#endif
static void push_child_plan(deparse_namespace* dpns, PlanState* ps, deparse_namespace* save_dpns);
static void pop_child_plan(deparse_namespace* dpns, deparse_namespace* save_dpns);
static void push_ancestor_plan(deparse_namespace* dpns, ListCell* ancestor_cell, deparse_namespace* save_dpns);
static void pop_ancestor_plan(deparse_namespace* dpns, deparse_namespace* save_dpns);
static void make_ruledef(StringInfo buf, HeapTuple ruletup, TupleDesc rulettc, int prettyFlags);
static void make_viewdef(StringInfo buf, HeapTuple ruletup, TupleDesc rulettc, int prettyFlags, int wrapColumn);
static void get_values_def(List* values_lists, deparse_context* context);
static void get_with_clause(Query* query, deparse_context* context);
static void get_select_query_def(Query* query, deparse_context* context, TupleDesc resultDesc);
static void get_insert_query_def(Query* query, deparse_context* context);
static void get_update_query_def(Query* query, deparse_context* context);
static void get_update_query_targetlist_def(
    Query* query, List* targetList, RangeTblEntry* rte, deparse_context* context);
static void get_delete_query_def(Query* query, deparse_context* context);
static void get_utility_query_def(Query* query, deparse_context* context);
static void get_basic_select_query(Query* query, deparse_context* context, TupleDesc resultDesc);
static void get_target_list(Query* query, List* targetList, deparse_context* context, TupleDesc resultDesc);
static void get_setop_query(Node* setOp, Query* query, deparse_context* context, TupleDesc resultDesc);
static Node* get_rule_sortgroupclause(Index ref, List* tlist, bool force_colno, deparse_context* context);
#ifdef USE_SPQ
static Node* get_rule_sortgroupclause_spq(Index ref, bool force_colno, deparse_context* context);
#endif
static void get_rule_groupingset(GroupingSet* gset, List* targetlist, deparse_context* context);
static void get_rule_orderby(List* orderList, List* targetList, bool force_colno, deparse_context* context);
static void get_rule_windowclause(Query* query, deparse_context* context);
static void get_rule_windowspec(WindowClause* wc, List* targetList, deparse_context* context);
static void get_rule_windowspec_listagg(WindowClause* wc, List* targetList, deparse_context* context);
static char* get_variable(
    Var* var, int levelsup, bool istoplevel, deparse_context* context, bool for_star = false, bool no_alias = false);
static RangeTblEntry* find_rte_by_refname(const char* refname, deparse_context* context);
static Node* find_param_referent(
    Param* param, deparse_context* context, deparse_namespace** dpns_p, ListCell** ancestor_cell_p);
static void get_parameter(Param* param, deparse_context* context);
static const char* get_simple_binary_op_name(OpExpr* expr);
static bool isSimpleNode(Node* node, Node* parentNode, int prettyFlags);
static void appendContextKeyword(
    deparse_context* context, const char* str, int indentBefore, int indentAfter, int indentPlus);
static void get_rule_expr(Node* node, deparse_context* context, bool showimplicit, bool no_alias = false);
static void get_rule_expr_funccall(Node* node, deparse_context* context, bool showimplicit);
static bool looks_like_function(Node* node);
static void get_oper_expr(OpExpr* expr, deparse_context* context, bool no_alias = false);
static void get_func_expr(FuncExpr* expr, deparse_context* context, bool showimplicit);
static void get_agg_expr(Aggref* aggref, deparse_context* context);
static void get_windowfunc_expr(WindowFunc* wfunc, deparse_context* context);
static void get_coercion_expr(
    Node* arg, deparse_context* context, Oid resulttype, int32 resulttypmod, Node* parentNode);
static void get_const_expr(Const* constval, deparse_context* context, int showtype);
static void get_const_collation(Const* constval, deparse_context* context);
static void simple_quote_literal(StringInfo buf, const char* val);
static void get_sublink_expr(SubLink* sublink, deparse_context* context);
static void get_from_clause(Query* query, const char* prefix, deparse_context* context, List* fromlist = NIL,
    bool isNeedError = true);
static void get_from_clause_item(Node* jtnode, Query* query, deparse_context* context, bool isNeedError = true);
static void get_from_clause_partition(RangeTblEntry* rte, StringInfo buf, deparse_context* context);
static void get_from_clause_subpartition(RangeTblEntry* rte, StringInfo buf, deparse_context* context);
static void get_from_clause_bucket(RangeTblEntry* rte, StringInfo buf, deparse_context* context);
static void get_from_clause_alias(Alias* alias, RangeTblEntry* rte, deparse_context* context);
static void get_from_clause_coldeflist(
    List* names, List* types, List* typmods, List* collations, deparse_context* context);
static void get_tablesample_def(TableSampleClause* tablesample, deparse_context* context);
static void GetTimecapsuleDef(const TimeCapsuleClause* timeCapsule, deparse_context* context);
void get_opclass_name(Oid opclass, Oid actual_datatype, StringInfo buf);
static Node* processIndirection(Node* node, deparse_context* context, bool printit);
static void printSubscripts(ArrayRef* aref, deparse_context* context);
static char* get_relation_name(Oid relid, bool isNeedError = true);
static char* generate_relation_name(Oid relid, List* namespaces, bool isNeedError = true);
static char* generate_function_name(
    Oid funcid, int nargs, List* argnames, Oid* argtypes, bool was_variadic, bool* use_variadic_p);
static char* generate_operator_name(Oid operid, Oid arg1, Oid arg2);
static text* string_to_text(char* str);
static Oid SearchSysTable(const char* query);
static void replace_cl_types_in_argtypes(Oid func_id, int numargs, Oid* argtypes, bool *is_client_logic);

static void AppendSubPartitionByInfo(StringInfo buf, Oid tableoid, SubpartitionInfo *subpartinfo, tableInfo* tableinfo = NULL);
static void AppendSubPartitionDetail(StringInfo buf, tableInfo tableinfo, SubpartitionInfo *subpartinfo);
static void AppendRangeIntervalPartitionInfo(StringInfo buf, Oid tableoid, tableInfo tableinfo, int partkeynum,
    Oid *iPartboundary, SubpartitionInfo *subpartinfo);
static void AppendListPartitionInfo(StringInfo buf, Oid tableoid, tableInfo tableinfo, int partkeynum,
    Oid *iPartboundary, SubpartitionInfo *subpartinfo);
static void AppendHashPartitionInfo(StringInfo buf, Oid tableoid, tableInfo tableinfo, int partkeynum,
    Oid *iPartboundary, SubpartitionInfo *subpartinfo);
static void AppendTablespaceInfo(const char *spcname, StringInfo buf, tableInfo tableinfo);
static inline bool IsTableVisible(Oid tableoid);
static void get_table_partitiondef(StringInfo query, StringInfo buf, Oid tableoid, tableInfo tableinfo);

/* from pgxcship */
Var* get_var_from_node(Node* node, bool (*func)(Oid) = func_oid_check_reject);

#define only_marker(rte) ((rte)->inh ? "" : "ONLY ")

/* ----------
 * get_ruledef			- Do it all and return a text
 *				  that could be used as a statement
 *				  to recreate the rule
 * ----------
 */








/* ----------
 * get_viewdef			- Mainly the same thing, but we
 *				  only return the SELECT part of a view
 * ----------
 */










/*
 * Common code for by-OID and by-name variants of pg_get_viewdef
 */




/*
 * @Description: if the type is a string type
 * @in typid - type oid
 * @return - return true if the type is a string type.
 */




























#ifdef DOLPHIN
#else
#endif



/*
 * @Description: get partition table defination
 * @in query - append query for SPI_execute.
 * @in buf - append defination string for partition table.
 * @in tableoid - parent table oid.
 * @in tableinfo - parent table info.
 * @return - void
 */













/*
 * @Description: get collation's namespace oid by collation oid.
 * @in colloid - collation oid.
 * @return - namespace oid.
 */




/*
 * @Description: get table's attribute infomartion.
 * @in tableoid - table oid.
 * @in buf - the string to append attribute info.
 * @in formatter -the formatter for foreign table.
 * @in ft_frmt_clmn -formatter columns.
 * @in cnt_ft_frmt_clmns - number of formatter columns.
 * @return - number of attribute.
 */
#ifdef DOLPHIN
#endif



/*
 * @Description: get table's distribute key.
 * @in relOid - table oid.
 * @return - distribute key.
 */


/*
 * @Description: get changing table's  info (alter table and comments).
 * @in tableoid - table oid.
 * @in buf - string to append changing info.
 * @in relname - table name.
 * @in relkind - relkind of the table.
 * @return - void
 */


/*
 * @Description: get table's constraint info.
 * @in conForm - constraint info.
 * @in tuple - tuple for constraint.
 * @in buf - string to append comment info.
 * @in constraintId - constraint oid.
 * @in relname - table name.
 * @return - void
 */


/*
 * @Description: get foreign table's constraint info.
 * @in buf - string to append comment info.
 * @in tableoid - table oid.
 * @in relname - table name.
 * @return - void
 */


/*
 * @Description: get table's index info.
 * @in tableoid - table oid.
 * @in buf - string to append comment info.
 * @in relname - table name.
 * @return - void
 */
#ifdef DOLPHIN
#else
#endif





/*
 * @Description: append table's info.
 * @in tableinfo - parent table info.
 * @in srvname - server name.
 * @in buf - string to append comment info.
 * @in tableoid - table oid.
 * @in isHDFSTbl - is a hdfs table
 * @in query - string for spi execute
 * @in ftoptions - options for foreign table
 * @in ft_write_only - write only
 * @return - is hdfs foreign table
 */


/*
 * @Description: append table's alter table info.
 * @in tableinfo - parent table info.
 * @in has_not_valid_check - check is not valid
 * @in buf - string to append comment info.
 * @in tableoid - table oid.
 * @in relname -table name
 * @return - void
 */







/*
 * @Description: in B-format, remove the collate attribute from reloptions and
 * add collate information to tableinfo.
 * @in reloptions - old reloptions.
 * @in tableinfo - table information.
 * @return - new reloptions.
 */


/*
 * @Description: get table's defination by table oid.
 * @in tableoid - table oid.
 * @return - table's defination.
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

/*
 * @Description: get table's defination by table oid.
 */


/* ----------
 * get_triggerdef			- Get the definition of a trigger
 * ----------
 */






#ifdef DOLPHIN
#endif
#ifdef PGXC
#endif /* PGXC */

/*
 * Pass back the TriggerWhen clause of a trigger given the pg_trigger record and
 * the expression tree (in nodeToString() representation) from pg_trigger.tgqual
 * for the trigger's WHEN condition.
 */
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * get_indexdef			- Get the definition of an index
 *
 * In the extended version, there is a colno argument as well as pretty bool.
 *	if colno == 0, we want a complete index definition.
 *	if colno > 0, we only want the Nth index key's variable or expression.
 *
 * Note that the SQL-function versions of this omit any info about the
 * index tablespace; this is intentional because pg_dump wants it that way.
 * However pg_get_indexdef_string() includes the index tablespace.
 * ----------
 */


/**
 * @Description: Get he definition of an index for dump scene.
 * If the table is an interval partitioned table and the index is a local index and dumpSchemaOnly is true, 
 * only output indexes of range partitions, else the output is the same as pg_get_indexdef.
 * @in The index table Oid and dump scheme only flag.
 * @return Returns a palloc'd C string; no pretty-printing.
 */




/**
 * @Description: Internal version for use by ALTER TABLE.
 * Includes a tablespace clause in the result.
 * @in The index table Oid.
 * @return Returns a palloc'd C string; no pretty-printing.
 */


/* Internal version that just reports the column definitions */








/*
 * Internal workhorse to decompile an index definition.
 *
 * This is now used for exclusion constraints as well: if excludeOps is not
 * NULL then it points to an array of exclusion operator OIDs.
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

/*
 * pg_get_constraintdef
 *
 * Returns the definition for the constraint, ie, everything that needs to
 * appear after "ALTER TABLE ... ADD CONSTRAINT <constraintname>".
 */




/* Internal version that returns a palloc'd C string */




#ifndef DOLPHIN
#else
#endif

/*
 * Convert an int16[] Datum into a comma-separated list of column names
 * for the indicated relation; append the list to buf.
 */


/* ----------
 * get_expr			- Decompile an expression tree
 *
 * Input: an expression tree in nodeToString form, and a relation OID
 *
 * Output: reverse-listed expression
 *
 * Currently, the expression can only refer to a single relation, namely
 * the one specified by the second parameter.  This is sufficient for
 * partial indexes, column default expressions, etc.  We also support
 * Var-free expressions, for which the OID can be InvalidOid.
 * ----------
 */









/* ----------
 * get_userbyid			- Get a user name by roleid and
 *				  fallback to 'unknown (OID=n)'
 * ----------
 */


/* ----------
 * pg_check_authid		- Check a user by roleid whether exist
 * ----------
 */


/*
 * pg_get_serial_sequence
 *		Get the name of the sequence used by a serial column,
 *		formatted suitably for passing to setval, nextval or currval.
 *		First parameter is not treated as double-quoted, second parameter
 *		is --- see documentation for reason.
 */




/*
 * pg_get_functiondef
 *		Returns the complete "CREATE OR REPLACE FUNCTION ..." statement for
 *		the specified function.
 *
 * Note: if you change the output format of this function, be careful not
 * to break psql's rules (in \ef and \sf) for identifying the start of the
 * function body.  To wit: the function body starts on a line that begins
 * with "AS ", and no preceding line will look like that.
 */





/*
 * pg_get_function_arguments
 *		Get a nicely-formatted list of arguments for a function.
 *		This is everything that would go between the parentheses in
 *		CREATE FUNCTION.
 */


/*
 * pg_get_function_identity_arguments
 *		Get a formatted list of arguments for a function.
 *		This is everything that would go between the parentheses in
 *		ALTER FUNCTION, etc.  In particular, don't print defaults.
 */


/*
 * pg_get_function_result
 *		Get a nicely-formatted version of the result type of a function.
 *		This is what would appear after RETURNS in CREATE FUNCTION.
 */


#ifdef DOLPHIN
#endif
#ifndef DOLPHIN
#else
#endif
#ifndef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

/*
 * Guts of pg_get_function_result: append the function's return type
 * to the specified buffer.
 */


/*
 * get param string from pg_proc_proargsrc.
 */


/*
 * Common code for pg_get_function_arguments and pg_get_function_result:
 * append the desired subset of arguments to buf.  We print only TABLE
 * arguments when print_table_args is true, and all the others when it's false.
 * We print argument defaults only if print_defaults is true.
 * Function return value is the number of arguments printed.
 */




/*
 * deparse_expression			- General utility for deparsing expressions
 *
 * calls deparse_expression_pretty with all prettyPrinting disabled
 */


/* ----------
 * deparse_expression_pretty	- General utility for deparsing expressions
 *
 * expr is the node tree to be deparsed.  It must be a transformed expression
 * tree (ie, not the raw output of gram.y).
 *
 * dpcontext is a list of deparse_namespace nodes representing the context
 * for interpreting Vars in the node tree.
 *
 * forceprefix is TRUE to force all Vars to be prefixed with their table names.
 *
 * showimplicit is TRUE to force all implicit casts to be shown explicitly.
 *
 * tries to pretty up the output according to prettyFlags and startIndent.
 *
 * The result is a palloc'd string.
 * ----------
 */
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * deparse_context_for			- Build deparse context for a single relation
 *
 * Given the reference name (alias) and OID of a relation, build deparsing
 * context for an expression referencing only that relation (as varno 1,
 * varlevelsup 0).	This is sufficient for many uses of deparse_expression.
 * ----------
 */
#ifdef PGXC
#endif

/*
 * deparse_context_for_planstate	- Build deparse context for a plan
 *
 * When deparsing an expression in a Plan tree, we might have to resolve
 * OUTER_VAR, INNER_VAR, or INDEX_VAR references.  To do this, the caller must
 * provide the parent PlanState node.  Then OUTER_VAR and INNER_VAR references
 * can be resolved by drilling down into the left and right child plans.
 * Similarly, INDEX_VAR references can be resolved by reference to the
 * indextlist given in the parent IndexOnlyScan node.  (Note that we don't
 * currently support deparsing of indexquals in regular IndexScan or
 * BitmapIndexScan nodes; for those, we can only deparse the indexqualorig
 * fields, which won't contain INDEX_VAR Vars.)
 *
 * Note: planstate really ought to be declared as "PlanState *", but we use
 * "Node *" to avoid having to include execnodes.h in builtins.h.
 *
 * The ancestors list is a list of the PlanState's parent PlanStates, the
 * most-closely-nested first.  This is needed to resolve PARAM_EXEC Params.
 * Note we assume that all the PlanStates share the same rtable.
 *
 * The plan's rangetable list must also be passed.  We actually prefer to use
 * the rangetable to resolve simple Vars, but the plan inputs are necessary
 * for Vars with special varnos.
 */
#ifdef PGXC
#endif

/*
 * set_deparse_planstate: set up deparse_namespace to parse subexpressions
 * of a given PlanState node
 *
 * This sets the planstate, outer_planstate, inner_planstate, outer_tlist,
 * inner_tlist, and index_tlist fields.  Caller is responsible for adjusting
 * the ancestors list if necessary.  Note that the rtable and ctes fields do
 * not need to change when shifting attention to different plan nodes in a
 * single plan tree.
 */
#ifdef USE_SPQ
#endif
#ifdef USE_SPQ
#endif
#ifdef USE_SPQ
#endif

#ifdef PGXC
/*
 * This is a special case deparse context to be used at the planning time to
 * generate query strings and expressions for remote shipping.
 *
 * XXX We should be careful while using this since the support is quite
 * limited. The only supported use case at this point is for remote join
 * reduction and some simple plan trees rooted by Agg node having a single
 * RemoteQuery node as leftree.
 */


/*
 * Set deparse context for Plan. Only those plan nodes which are immediate (or
 * through simple nodes) parents of RemoteQuery nodes are supported right now.
 *
 * This is a kind of work-around since the new deparse interface (since 9.1)
 * expects a PlanState node. But planstates are instantiated only at execution
 * time when InitPlan is called. But we are required to deparse the query
 * during planning time, so we hand-cook these dummy PlanState nodes instead of
 * init-ing the plan. Another approach could have been to delay the query
 * generation to the execution time, but we are not yet sure if this can be
 * safely done, especially for remote join reduction.
 */


#endif
/*
 * push_child_plan: temporarily transfer deparsing attention to a child plan
 *
 * When expanding an OUTER_VAR or INNER_VAR reference, we must adjust the
 * deparse context in case the referenced expression itself uses
 * OUTER_VAR/INNER_VAR.  We modify the top stack entry in-place to avoid
 * affecting levelsup issues (although in a Plan tree there really shouldn't
 * be any).
 *
 * Caller must provide a local deparse_namespace variable to save the
 * previous state for pop_child_plan.
 */


/*
 * pop_child_plan: undo the effects of push_child_plan
 */


/*
 * push_ancestor_plan: temporarily transfer deparsing attention to an
 * ancestor plan
 *
 * When expanding a Param reference, we must adjust the deparse context
 * to match the plan node that contains the expression being printed;
 * otherwise we'd fail if that expression itself contains a Param or
 * OUTER_VAR/INNER_VAR/INDEX_VAR variable.
 *
 * The target ancestor is conveniently identified by the ListCell holding it
 * in dpns->ancestors.
 *
 * Caller must provide a local deparse_namespace variable to save the
 * previous state for pop_ancestor_plan.
 */


/*
 * pop_ancestor_plan: undo the effects of push_ancestor_plan
 */


/* ----------
 * make_ruledef			- reconstruct the CREATE RULE command
 *				  for a given pg_rewrite tuple
 * ----------
 */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * make_viewdef			- reconstruct the SELECT part of a
 *				  view rewrite rule
 * ----------
 */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */

#ifdef PGXC
/* ----------
 * deparse_query			- Parse back one query parsetree
 *
 * Purpose of this function is to build up statement for a RemoteQuery
 * The query generated has all object names schema-qualified. This is
 * done by temporarily setting search_path to NIL.
 * It calls get_query_def without pretty print flags.
 *
 * Caution: get_query_def calls AcquireRewriteLocks, which might modify the RTEs
 * in place. So it is generally appropriate for the caller of this routine to
 * have first done a copyObject() to make a writable copy of the querytree in
 * the current memory context.
 * ----------
 */
#ifndef DOLPHIN
#endif
#ifdef PGXC
#endif /* PGXC */
#endif
/* ----------
 * get_query_def			- Parse back one query parsetree
 *
 * If resultDesc is not NULL, then it is the output tuple descriptor for
 * the view represented by a SELECT query.
 * ----------
 */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif

/* ----------
 * get_values_def			- Parse back a VALUES list
 * ----------
 */


/* ----------
 * get_with_clause			- Parse back a WITH clause
 * ----------
 */
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * get_select_query_def			- Parse back a SELECT parsetree
 * ----------
 */
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * Detect whether query looks like SELECT ... FROM VALUES();

 * if so, return the VALUES RTE.  Otherwise return NULL.
 */




/*
 * @Description: Convert hint to string
 * @in hstate: hint state.
 * @out buf: String buf.
 */




/* ----------
 * get_target_list			- Parse back a SELECT target list
 *
 * This is also used for RETURNING lists in INSERT/UPDATE/DELETE.
 * ----------
 */
#ifdef PGXC
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif

#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */

/*
 * Display a sort/group clause.
 *
 * Also returns the expression tree, so caller need not find it again.
 */

#ifdef USE_SPQ
/*
 * Display a sort/group clause.
 *
 * Also returns the expression tree, so caller need not find it again.
 */

static Node* get_rule_sortgroupclause_spq(Index ref, bool force_colno, deparse_context* context)
{
    StringInfo buf = context->buf;
    TargetEntry* tle = NULL;
    Node* expr = NULL;
    List* tlist;

    deparse_namespace* dpns_spq = (deparse_namespace*)linitial(context->namespaces);
    PlanState* ps = dpns_spq->planstate;
    WindowAgg* node = NULL;
    node = (WindowAgg*)ps->plan;
    tlist = node->plan.lefttree->targetlist;

    if (tlist == NULL){
        return expr;
    }

    tle = get_sortgroupref_tle_spq(ref, tlist);
    expr = (Node*)tle->expr;

    deparse_namespace* dpns = NULL;
    deparse_namespace save_dpns;

    dpns = (deparse_namespace*)list_nth(context->namespaces, ((Var*)expr)->varlevelsup);
    push_child_plan(dpns, dpns->outer_planstate, &save_dpns);


    /*
     * Use column-number form if requested by caller.  Otherwise, if
     * expression is a constant, force it to be dumped with an explicit cast
     * as decoration --- this is because a simple integer constant is
     * ambiguous (and will be misinterpreted by findTargetlistEntry()) if we
     * dump it without any decoration.	Otherwise, just dump the expression
     * normally.
     */
    if (force_colno || context->sortgroup_colno) {
        Assert(!tle->resjunk);
        appendStringInfo(buf, "%d", tle->resno);
    } else if (expr && IsA(expr, Var))
        get_rule_expr(expr, context, true);

    pop_child_plan(dpns, &save_dpns);

    return expr;
}
#endif

/*
 * @Description: Display a GroupingSet.
 * @in gset: Grouping Set.
 * @in targetlist: targetlist.
 * @in context: Context info needed for invoking a recursive querytree display routine.
 */




/*
 * Display an ORDER BY list.
 */
#ifdef USE_SPQ
#endif

/*
 * Display a WINDOW clause.
 *
 * Note that the windowClause list might contain only anonymous window
 * specifications, in which case we should print nothing here.
 */


/*
 * Display a window definition
 */
#ifdef USE_SPQ
#endif

/*
 * Display a window definition for listagg
 */
#ifdef USE_SPQ
#endif

/*
 * @Description: get_set_target_list    Parse back a SET clause
 * This is used for UPDATE and INSERT ON DUPLICATE KEY UPDATE.
 */


/* ----------
 * get_insert_query_def			- Parse back an INSERT parsetree
 * ----------
 */
#ifdef PGXC
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * get_update_query_def			- Parse back an UPDATE parsetree
 * ----------
 */
#ifdef ODLPHIN
#else
#endif

/* ----------
 * get_update_query_targetlist_def         - Parse back an UPDATE targetlist
 * ----------
 */


/* ----------
 * get_delete_query_def			- Parse back a DELETE parsetree
 * ----------
 */
#ifdef ODLPHIN
#else
#endif

/* 
 * only support add colum and drop column, support text and numeric, number datatype,
 * if timeseries table support more datatype, should test gs_expand if it works well
 */






















/* ----------
 * get_utility_query_def			- Parse back a UTILITY parsetree
 * ----------
 */
#ifdef PGXC
#endif

#ifdef PGXC

#endif

/*
 * Display a Var appropriately.
 *
 * In some cases (currently only when recursing into an unnamed join)
 * the Var's varlevelsup has to be interpreted with respect to a context
 * above the current one; levelsup indicates the offset.
 *
 * If istoplevel is TRUE, the Var is at the top level of a SELECT's
 * targetlist, which means we need special treatment of whole-row Vars.
 * Instead of the normal "tab.*", we'll print "tab.*::typename", which is a
 * dirty hack to prevent "tab.*" from being expanded into multiple columns.
 * (The parser will strip the useless coercion, so no inefficiency is added in
 * dump and reload.)  We used to print just "tab" in such cases, but that is
 * ambiguous and will yield the wrong result if "tab" is also a plain column
 * name in the query.
 *
 * Returns the attname of the Var, or NULL if the Var has no attname (because
 * it is a whole-row Var).
 */
#ifdef PGXC
#endif
#ifdef PGXC
#endif /* PGXC */
#ifndef ENABLE_MULTIPLE_NODES
#endif /* ENABLE_MULTIPLE_NODES */

/*
 * Get the name of a field of an expression of composite type.	The
 * expression is usually a Var, but we handle other cases too.
 *
 * levelsup is an extra offset to interpret the Var's varlevelsup correctly.
 *
 * This is fairly straightforward when the expression has a named composite
 * type; we need only look up the type in the catalogs.  However, the type
 * could also be RECORD.  Since no actual table or view column is allowed to
 * have type RECORD, a Var of type RECORD must refer to a JOIN or FUNCTION RTE
 * or to a subquery output.  We drill down to find the ultimate defining
 * expression and attempt to infer the field name from it.	We ereport if we
 * can't determine the name.
 *
 * Similarly, a PARAM of type RECORD has to refer to some expression of
 * a determinable composite type.
 */
#ifdef PGXC
#endif
#ifdef PGXC
#endif
#ifdef PGXC
#endif /* PGXC */

/*
 * find_rte_by_refname		- look up an RTE by refname in a deparse context
 *
 * Returns NULL if there is no matching RTE or the refname is ambiguous.
 *
 * NOTE: this code is not really correct since it does not take account of
 * the fact that not all the RTEs in a rangetable may be visible from the
 * point where a Var reference appears.  For the purposes we need, however,
 * the only consequence of a false match is that we might stick a schema
 * qualifier on a Var that doesn't really need it.  So it seems close
 * enough.
 */


/*
 * Try to find the referenced expression for a PARAM_EXEC Param that might
 * reference a parameter supplied by an upper NestLoop or SubPlan plan node.
 *
 * If successful, return the expression and set *dpns_p and *ancestor_cell_p
 * appropriately for calling push_ancestor_plan().	If no referent can be
 * found, return NULL.
 */


/*
 * Display a Param appropriately.
 */


/*
 * get_simple_binary_op_name
 *
 * helper function for isSimpleNode
 * will return single char binary operator name, or NULL if it's not
 */


/*
 * isSimpleNode - check if given node is simple (doesn't need parenthesizing)
 *
 *	true   : simple in the context of parent node's type
 *	false  : not simple
 */


/*
 * appendContextKeyword - append a keyword to buffer
 *
 * If prettyPrint is enabled, perform a line break, and adjust indentation.
 * Otherwise, just append the keyword.
 */


/*
 * get_rule_expr_paren	- deparse expr using get_rule_expr,
 * embracing the string with parentheses if necessary for prettyPrint.
 *
 * Never embrace if prettyFlags=0, because it's done in the calling node.
 *
 * Any node that does *not* embrace its argument node by sql syntax (with
 * parentheses, non-operator keywords like CASE/WHEN/ON, or comma etc) should
 * use get_rule_expr_paren instead of get_rule_expr so parentheses can be
 * added.
 */


/* ----------
 * get_rule_expr			- Parse back an expression
 *
 * Note: showimplicit determines whether we display any implicit cast that
 * is present at the top of the expression tree.  It is a passed argument,
 * not a field of the context struct, because we change the value as we
 * recurse down into the expression.  In general we suppress implicit casts
 * when the result type is known with certainty (eg, the arguments of an
 * OR must be boolean).  We display implicit casts for arguments of functions
 * and operators, since this is needed to be certain that the same function
 * or operator will be chosen when the expression is re-parsed.
 * ----------
 */
#ifdef USE_SPQ
#endif

/*
 * @Description:
 * get_rule_expr_funccall	  - Parse back a function-call expression
 *
 * Same as get_rule_expr(), except that we guarantee that the output will
 * look like a function call, or like one of the things the grammar treats as
 * equivalent to a function call (see the func_expr_windowless production).
 * This is needed in places where the grammar uses func_expr_windowless and
 * you can't substitute a parenthesized a_expr.	If what we have isn't going
 * to look like a function call, wrap it in a dummy CAST() expression, which
 * will satisfy the grammar --- and, indeed, is likely what the user wrote to
 * produce such a thing.
 * @in node -	expr node
 * @in context - deparse context
 * @in showimplicit - whether display implicit casts for arguments of functions and operators
 * @out - void
 */


/*
 * @Description:  Helper function to identify node types that satisfy func_expr_windowless.
 * If in doubt, "false" is always a safe answer.
 * @in node -	expr node
 * @out - bool
 */


/*
 * get_oper_expr			- Parse back an OpExpr node
 */


/*
 * get_func_expr			- Parse back a FuncExpr node
 */


/*
 * get_agg_expr			- Parse back an Aggref node
 */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif /* PGXC */

#ifdef USE_SPQ
#endif

/*
 * construct_windowClause	- Construct a WindowClause node for parser the param of OVER
 */
#ifdef USE_SPQ
#endif



/*
 * get_windowfunc_expr	- Parse back a WindowFunc node
 */


/* ----------
 * get_coercion_expr
 *
 *	Make a string representation of a value coerced to a specific type
 * ----------
 */


/* ----------
 * get_const_expr
 *
 *	Make a string representation of a Const
 *
 * showtype can be -1 to never show "::typename" decoration, or +1 to always
 * show it, or 0 to show it only if the constant wouldn't be assumed to be
 * the right type by default.
 *
 * If the Const's collation isn't default for its type, show that too.
 * This can only happen in trees that have been through constant-folding.
 * We assume we don't need to do this when showtype is -1.
 * ----------
 */
#ifdef DOLPHIN
#endif

/*
 * helper for get_const_expr: append COLLATE if needed
 */


/*
 * simple_quote_literal - Format a string as a SQL literal, append to buf
 */






/*
 * @Description: Parse back a sublink
 * @in sublink: Subselect appearing in an expression.
 * @in context: Context info needed for invoking a recursive querytree display routine.
 */
#ifdef PGXC
#endif /* PGXC */

/* ----------
 * get_from_clause			- Parse back a FROM clause
 *
 * "prefix" is the keyword that denotes the start of the list of FROM
 * elements. It is FROM when used to parse back SELECT and UPDATE, but
 * is USING when parsing back DELETE.
 * ----------
 */








/* Clause "PATTITION (partition_name, subpartition_name, ...)" is used for a delete statement. */


#ifdef PGXC
#endif /* PGXC */
#ifdef PGXC
#endif
#ifdef DOLPHIN
#endif

/*
 * get_from_clause_alias - reproduce column alias list
 *
 * This is tricky because we must ignore dropped columns.
 */


/*
 * Description: Print a TableSampleClause.
 *
 * Parameters: @in tablesample: TableSampleClause node.
 *             @out context: rewrite sql.
 *
 * Return: void
 */


/*
 * Description: Print a TimeCapsuleClause.
 *
 * Parameters:
 *	@in timecapsule: TimeCapsuleClause node.
 *	@out context: rewrite sql.
 *
 * Return: void
 */


/*
 * get_from_clause_coldeflist - reproduce FROM clause coldeflist
 *
 * The coldeflist is appended immediately (no space) to buf.  Caller is
 * responsible for ensuring that an alias or AS is present before it.
 */


/*
 * get_opclass_name			- fetch name of an index operator class
 *
 * The opclass name is appended (after a space) to buf.
 *
 * Output is suppressed if the opclass is the default for the given
 * actual_datatype.  (If you don't want this behavior, just pass
 * InvalidOid for actual_datatype.)
 */


/*
 * processIndirection - take care of array and subfield assignment
 *
 * We strip any top-level FieldStore or assignment ArrayRef nodes that
 * appear in the input, and return the subexpression that's to be assigned.
 * If printit is true, we also print out the appropriate decoration for the
 * base column name (that the caller just printed).
 */




/*
 * quote_identifier			- Quote an identifier only if needed
 *
 * When quotes are needed, we palloc the required space; slightly
 * space-wasteful but well worth it for notational simplicity.
 */
const char* quote_identifier(const char* ident)
{
    /*
     * Can avoid quoting if ident starts with a lowercase letter or underscore
     * and contains only lowercase letters, digits, and underscores, *and* is
     * not any SQL keyword.  Otherwise, supply quotes.
     */
    int nquotes = 0;
    bool safe = false;
    const char* ptr = NULL;
    char* result = NULL;
    char* optr = NULL;

    /*
     * would like to use <ctype.h> macros here, but they might yield unwanted
     * locale-specific results...
     */
    safe = ((ident[0] >= 'a' && ident[0] <= 'z') || ident[0] == '_');

    for (ptr = ident; *ptr; ptr++) {
        char ch = *ptr;

        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch == '_')) {
            /* okay */
        } else {
            safe = false;
#ifdef DOLPHIN

            if (ch == GET_QUOTE())
#else
            if (ch == '"')
#endif
                nquotes++;
        }
    }

    if (u_sess->attr.attr_sql.quote_all_identifiers)
        safe = false;

    if (safe) {
        /*
         * Check for keyword.  We quote keywords except for unreserved ones.
         * (In some cases we could avoid quoting a col_name or type_func_name
         * keyword, but it seems much harder than it's worth to tell that.)
         *
         * Note: ScanKeywordLookup() does case-insensitive comparison, but
         * that's fine, since we already know we have all-lower-case.
         */
        int kwnum = ScanKeywordLookup(ident, &ScanKeywords);

        if (kwnum >= 0 && ScanKeywordCategories[kwnum] != UNRESERVED_KEYWORD)
            safe = false;
    }

    if (safe)
        return ident; /* no change needed */

    result = (char*)palloc(strlen(ident) + nquotes + 2 + 1);

    optr = result;
#ifdef DOLPHIN
    char quote = GET_QUOTE();
    *optr++ = quote;
    for (ptr = ident; *ptr; ptr++) {
        char ch = *ptr;

        if (ch == quote)
            *optr++ = quote;
        *optr++ = ch;
    }
    *optr++ = quote;
#else
    *optr++ = '"';
    for (ptr = ident; *ptr; ptr++) {
        char ch = *ptr;

        if (ch == '"')
            *optr++ = '"';
        *optr++ = ch;
    }
    *optr++ = '"';
#endif
    *optr = '\0';

    return result;
}

/*
 * quote_qualified_identifier	- Quote a possibly-qualified identifier
 *
 * Return a name of the form qualifier.ident, or just ident if qualifier
 * is NULL, quoting each component if necessary.  The result is palloc'd.
 */
char* quote_qualified_identifier(const char* qualifier, const char* ident1, const char* ident2)
{
    StringInfoData buf;

    initStringInfo(&buf);
    if (qualifier != NULL)
        appendStringInfo(&buf, "%s.", quote_identifier(qualifier));
    if (ident1 != NULL)
        appendStringInfoString(&buf, quote_identifier(ident1));
    if (ident2 != NULL && ident1 != NULL) {
        appendStringInfo(&buf, ".%s", quote_identifier(ident2));
    } else if (ident1 == NULL && ident2 != NULL) {
        appendStringInfoString(&buf, quote_identifier(ident2));
    }
    return buf.data;
}

/*
 * get_relation_name
 *		Get the unqualified name of a relation specified by OID
 *
 * This differs from the underlying get_rel_name() function in that it will
 * throw error instead of silently returning NULL if the OID is bad.
 */


/*
 * generate_relation_name
 *		Compute the name to display for a relation specified by OID
 *
 * The result includes all necessary quoting and schema-prefixing.
 *
 * If namespaces isn't NIL, it must be a list of deparse_namespace nodes.
 * We will forcibly qualify the relation name if it equals any CTE name
 * visible in the namespace list.
 */


/*
 * generate_function_name
 *		Compute the name to display for a function specified by OID,
 *		given that it is being called with the specified actual arg names and
 *		types.	(Those matter because of ambiguous-function resolution rules.)
 *
 * The result includes all necessary quoting and schema-prefixing.	We can
 * also pass back an indication of whether the function is variadic.
 */




/*
 * generate_operator_name
 *		Compute the name to display for an operator specified by OID,
 *		given that it is being called with the specified actual arg types.
 *		(Arg types matter because of ambiguous-operator resolution rules.
 *		Pass InvalidOid for unused arg of a unary operator.)
 *
 * The result includes all necessary quoting and schema-prefixing,
 * plus the OPERATOR() decoration needed to use a qualified operator name
 * in an expression.
 */


/*
 * generate_collation_name
 *		Compute the name to display for a collation specified by OID
 *
 * The result includes all necessary quoting and schema-prefixing.
 */


/*
 * Given a C string, produce a TEXT datum.
 *
 * We assume that the input was palloc'd and may be freed.
 */


/*
 * Generate a C string representing a relation's reloptions, or NULL if none.
 */


/*Get table distribute columns*/







/*
 * deparse_create_sequence
 *          - General utility for deparsing create sequence.
 *
 */


/*
 * deparse_alter_sequence
 *          - General utility for deparsing alter sequence.
 *
 */


/*
 * Replace column managed type with original type for proper print
 * via proallargtypes
 */




#ifdef DOLPHIN


;
extern "C" DLL_PUBLIC Datum gs_get_viewdef_name(PG_FUNCTION_ARGS);



;
extern "C" DLL_PUBLIC Datum gs_get_viewdef_oid(PG_FUNCTION_ARGS);



;
extern "C" DLL_PUBLIC Datum gs_get_schemadef_name(PG_FUNCTION_ARGS);


#endif
