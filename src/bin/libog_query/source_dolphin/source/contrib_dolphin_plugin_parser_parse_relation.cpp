/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - addRangeTableEntryForRelation
 * - get_rte_part_attr_num
 * - buildRelationAliases
 * - set_rte_flags
 * - setRteOrientation
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * parse_relation.cpp
 *	  parser support routines dealing with relations
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/common/backend/parser/parse_relation.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include <ctype.h>

#include "access/sysattr.h"
#include "access/tableam.h"
#include "plugin_catalog/heap.h"
#include "catalog/namespace.h"
#include "catalog/pg_auth_history.h"
#include "catalog/pg_type.h"
#include "funcapi.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "plugin_parser/parsetree.h"
#include "plugin_parser/parse_hint.h"
#include "plugin_parser/parse_relation.h"
#include "plugin_parser/parse_type.h"
#include "plugin_parser/parse_clause.h"
#include "plugin_parser/parse_expr.h"
#ifdef PGXC
#include "access/transam.h"
#include "pgxc/pgxc.h"
#endif
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/partitionkey.h"
#include "utils/rel.h"
#include "utils/rel_gs.h"
#include "utils/syscache.h"
#include "utils/pl_package.h"
#include "catalog/pg_partition_fn.h"
#include "catalog/pg_synonym.h"
#include "plugin_parser/parse_utilcmd.h"
#include "plugin_parser/parse_expr.h"
#include "commands/tablecmds.h"
#include "catalog/pg_user_status.h"
#include "commands/user.h"
#include "utils/snapmgr.h"
#include "workload/workload.h"
#include "storage/lmgr.h"
#include "tcop/utility.h"
#include "optimizer/bucketinfo.h"
#include "plugin_optimizer/planner.h"
#include "storage/tcap.h"
#include "gs_ledger/ledger_utils.h"
#include "catalog/pg_object.h"
#include "catalog/pg_depend.h"
#include "catalog/pg_rewrite.h"
#ifdef ENABLE_MOT
#include "storage/mot/jit_def.h"
#endif
#ifdef DOLPHIN
#include "plugin_postgres.h"
#include "rewrite/rewriteSupport.h"
#endif
#include "rewrite/rewriteManip.h"

#define MAXSTRLEN ((1 << 11) - 1)
static RangeTblEntry* scanNameSpaceForRefname(ParseState* pstate, const char* refname, int location);
static RangeTblEntry* scanNameSpaceForRelid(ParseState* pstate, Oid relid, int location);
static void markRTEForSelectPriv(ParseState* pstate, RangeTblEntry* rte, int rtindex, AttrNumber col);
static void expandRelation(Oid relid, Alias* eref, int rtindex, int sublevels_up, int location, bool include_dropped,
    List** colnames, List** colvars);
static void expandTupleDesc(TupleDesc tupdesc, Alias* eref, int rtindex, int sublevels_up, int location,
    bool include_dropped, List** colnames, List** colvars);
static void setRteOrientation(Relation rel, RangeTblEntry* rte);
static int32* getValuesTypmods(RangeTblEntry* rte);
static IndexHintType preCheckIndexHints(ParseState* pstate, List* indexhints, Relation relation);
static void change_var_attno(Query* query, Oid rel_oid, int oldAttnum, int newAttnum);

#ifndef PGXC
static int specialAttNum(const char* attname);
#endif

#ifdef DOLPHIN

#endif



/*
 * @Description: set the RTE common flags
 * @inout rte: pointer to the RTE
 * @in: whether it is inherit
 * @in inFromCl: whether it is present in FROM clause
 * @in perm: required permission such as ACL_SELECT, etc.
 */
static void set_rte_flags(RangeTblEntry* rte, bool inh, bool inFromCl, int perm)
{
    /* ----------
     * Flags:
     * - this RTE should be expanded to include descendant tables,
     * - this RTE is in the FROM clause,
     * - this RTE should be checked for appropriate access rights.
     *
     * Joins are never checked for access rights.
     * ----------
     */
    rte->inh = inh; /* never true for joins */
    rte->inFromCl = inFromCl;

    rte->requiredPerms = perm;
    rte->checkAsUser = InvalidOid;
    rte->selectedCols = NULL;
    rte->insertedCols = NULL;
    rte->updatedCols = NULL;
    rte->extraUpdatedCols = NULL;
}

/*
 * @Description: get the valid relation's partition attribute number(s).
 * @in rel: relation
 * @return: a list of attribute numbers.
 */
static List* get_rte_part_attr_num(Relation rel)
{
    if (IS_PGXC_COORDINATOR && rel->rd_id >= FirstNormalObjectId && PointerIsValid(rel->rd_locator_info)) {
        return (List*)copyObject(rel->rd_locator_info->partAttrNum);
    }

    return NULL;
}
/*
 * refnameRangeTblEntry
 *	  Given a possibly-qualified refname, look to see if it matches any RTE.
 *	  If so, return a pointer to the RangeTblEntry; else return NULL.
 *
 *	  Optionally get RTE's nesting depth (0 = current) into *sublevels_up.
 *	  If sublevels_up is NULL, only consider items at the current nesting
 *	  level.
 *
 * An unqualified refname (schemaname == NULL) can match any RTE with matching
 * alias, or matching unqualified relname in the case of alias-less relation
 * RTEs.  It is possible that such a refname matches multiple RTEs in the
 * nearest nesting level that has a match; if so, we report an error via
 * ereport().
 *
 * A qualified refname (schemaname != NULL) can only match a relation RTE
 * that (a) has no alias and (b) is for the same relation identified by
 * schemaname.refname.	In this case we convert schemaname.refname to a
 * relation OID and search by relid, rather than by alias name.  This is
 * peculiar, but it's what SQL92 says to do.
 */


/*
 * Search the query's table namespace for an RTE matching the
 * given unqualified refname.  Return the RTE if a unique match, or NULL
 * if no match.  Raise error if multiple matches.
 */


/*
 * Search the query's table namespace for a relation RTE matching the
 * given relation OID.	Return the RTE if a unique match, or NULL
 * if no match.  Raise error if multiple matches (which shouldn't
 * happen if the namespace was checked correctly when it was created).
 *
 * See the comments for refnameRangeTblEntry to understand why this
 * acts the way it does.
 */


/*
 * Search the query's CTE namespace for a CTE matching the given unqualified
 * refname.  Return the CTE (and its levelsup count) if a match, or NULL
 * if no match.  We need not worry about multiple matches, since parse_cte.c
 * rejects WITH lists containing duplicate CTE names.
 */


/*
 * Search for a possible "future CTE", that is one that is not yet in scope
 * according to the WITH scoping rules.  This has nothing to do with valid
 * SQL semantics, but it's important for error reporting purposes.
 */


/*
 * searchRangeTableForRel
 *	  See if any RangeTblEntry could possibly match the RangeVar.
 *	  If so, return a pointer to the RangeTblEntry; else return NULL.
 *
 * This is different from refnameRangeTblEntry in that it considers every
 * entry in the ParseState's rangetable(s), not only those that are currently
 * visible in the p_relnamespace lists.  This behavior is invalid per the SQL
 * spec, and it may give ambiguous results (there might be multiple equally
 * valid matches, but only one will be returned).  This must be used ONLY
 * as a heuristic in giving suitable error messages.  See errorMissingRTE.
 *
 * Notice that we consider both matches on actual relation (or CTE) name
 * and matches on alias.
 */


/*
 * Check for relation-name conflicts between two relnamespace lists.
 * Raise an error if any is found.
 *
 * Note: we assume that each given argument does not contain conflicts
 * itself; we just want to know if the two can be merged together.
 *
 * Per SQL92, two alias-less plain relation RTEs do not conflict even if
 * they have the same eref->aliasname (ie, same relation name), if they
 * are for different relation OIDs (implying they are in different schemas).
 */


/*
 * given an RTE, return RT index (starting with 1) of the entry,
 * and optionally get its nesting depth (0 = current).	If sublevels_up
 * is NULL, only consider rels at the current nesting level.
 * Raises error if RTE not found.
 */


/*
 * Given an RT index and nesting depth, find the corresponding RTE.
 * This is the inverse of RTERangeTablePosn.
 */


/*
 * Fetch the CTE for a CTE-reference RTE.
 *
 * rtelevelsup is the number of query levels above the given pstate that the
 * RTE came from.  Callers that don't have this information readily available
 * may pass -1 instead.
 */


/*
 * scanRTEForColumn
 *	  Search the column names of a single RTE for the given name.
 *	  If found, return an appropriate Var node, else return NULL.
 *	  If the name proves ambiguous within this RTE, raise error.
 *
 * Side effect: if we find a match, mark the RTE as requiring read access
 * for the column.
 */
#ifdef DOLPHIN
#else
#endif

/*
 * colNameToVar
 *	  Search for an unqualified column name.
 *	  If found, return the appropriate Var node (or expression).
 *	  If not found, return NULL.  If the name proves ambiguous, raise error.
 *	  If localonly is true, only names in the innermost query are considered.
 */


/*
 * searchRangeTableForCol
 *   See if any RangeTblEntry could possibly provide the given column name.
 *   If so, return a pointer to the RangeTblEntry; else return NULL.
 *
 * This is different from colNameToVar in that it considers every entry in
 * the ParseState's rangetable(s), not only those that are currently visible
 * in the p_varnamespace lists.  This behavior is invalid per the SQL spec,
 * and it may give ambiguous results (there might be multiple equally valid
 * matches, but only one will be returned).  This must be used ONLY as a
 * heuristic in giving suitable error messages.  See errorMissingColumn.
 */


/*
 * markRTEForSelectPriv
 *	   Mark the specified column of an RTE as requiring SELECT privilege
 *
 * col == InvalidAttrNumber means a "whole row" reference
 *
 * The caller should pass the actual RTE if it has it handy; otherwise pass
 * NULL, and we'll look it up here.  (This uglification of the API is
 * worthwhile because nearly all external callers have the RTE at hand.)
 */


/*
 * markVarForSelectPriv
 *	   Mark the RTE referenced by a Var as requiring SELECT privilege
 *
 * The caller should pass the Var's referenced RTE if it has it handy
 * (nearly all do); otherwise pass NULL.
 */


/*
 * buildRelationAliases
 *		Construct the eref column name list for a relation RTE.
 *		This code is also used for the case of a function RTE returning
 *		a named composite type.
 *
 * tupdesc: the physical column information
 * alias: the user-supplied alias, or NULL if none
 * eref: the eref Alias to store column names in
 *
 * eref->colnames is filled in.  Also, alias->colnames is rebuilt to insert
 * empty strings for any dropped columns, so that it will be one-to-one with
 * physical column numbers.
 */
static void buildRelationAliases(TupleDesc tupdesc, Alias* alias, Alias* eref)
{
    int maxattrs = tupdesc->natts;
    ListCell* aliaslc = NULL;
    int numaliases;
    int varattno;
    int numdropped = 0;

    AssertEreport(eref->colnames == NIL, MOD_OPT, "");

    if (alias != NULL) {
        aliaslc = list_head(alias->colnames);
        numaliases = list_length(alias->colnames);
        /* We'll rebuild the alias colname list */
        alias->colnames = NIL;
    } else {
        aliaslc = NULL;
        numaliases = 0;
    }

    for (varattno = 0; varattno < maxattrs; varattno++) {
        Form_pg_attribute attr = &tupdesc->attrs[varattno];
        Value* attrname = NULL;

        if (attr->attisdropped) {
            /* Always insert an empty string for a dropped column */
            attrname = makeString(pstrdup(""));
            if (aliaslc != NULL) {
                alias->colnames = lappend(alias->colnames, attrname);
            }
            numdropped++;
        } else if (aliaslc != NULL) {
            /* Use the next user-supplied alias */
            attrname = (Value*)lfirst(aliaslc);
            aliaslc = lnext(aliaslc);
            alias->colnames = lappend(alias->colnames, attrname);
        } else {
            attrname = makeString(pstrdup(NameStr(attr->attname)));
            /* we're done with the alias if any */
        }

        eref->colnames = lappend(eref->colnames, attrname);
    }

    /* Too many user-supplied aliases? */
    if (aliaslc != NULL) {
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_COLUMN_REFERENCE),
                errmsg("table \"%s\" has %d columns available but %d columns specified",
                    eref->aliasname,
                    maxattrs - numdropped,
                    numaliases)));
    }
}

/*
 * buildScalarFunctionAlias
 *		Construct the eref column name list for a function RTE,
 *		when the function returns a scalar type (not composite or RECORD).
 *
 * funcexpr: transformed expression tree for the function call
 * funcname: function name (used only for error message)
 * alias: the user-supplied alias, or NULL if none
 * eref: the eref Alias to store column names in
 *
 * eref->colnames is filled in.
 */








/*
 * try to get new relation oid by namespace and relname, replace the old ones
 * in query and update pg_depend.
 */


#ifdef DOLPHIN

#endif

typedef struct change_relid_in_rtable_context {
    Oid rel_oid;
    Oid rewrite_oid;
    bool allExist;
} change_relid_in_rtable_context;



/*
 * walk through whole query to search all rtables, which some maybe
 * hang on a subquery
 */
#ifdef DOLPHIN
#endif

typedef struct search_rte_context {
    Oid rel_oid;
    int2 attnum;
    char* attname;
} search_rte_context;



/*
 * find rte byu walking through all rtables of query and its sublevel query,
 * and return colname.
 */


/* return true column if exists, and set newAttnum. */
#ifndef DOLPHIN
#else
#endif



enum ValidateDependResult {
    ValidateDependInvalid,
    ValidateDependValid,
    ValidateDependCircularDepend,
    
};

static ValidateDependResult ValidateDependView(Oid view_oid, char objType, List** list, bool force){return ValidateDependInvalid;}


bool ValidateDependView(Oid view_oid, char objType) {
    List * list = NIL;
    ValidateDependResult result = ValidateDependView(view_oid, objType, &list, false);
    list_free_ext(list);
    if (result == ValidateDependCircularDepend) {
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_OBJECT_DEFINITION),
                errmsg(
                    "infinite recursion detected in rules for relation: \"%s\"", get_rel_name(view_oid))));
    }
    return result != ValidateDependInvalid;
}



/*
 * Open a table during parse analysis
 *
 * This is essentially just the same as heap_openrv(), except that it caters
 * to some parser-specific error reporting needs, notably that it arranges
 * to include the RangeVar's parse location in any resulting error.
 *
 * Note: properly, lockmode should be declared LOCKMODE not int, but that
 * would require importing storage/lock/lock.h into parse_relation.h.  Since
 * LOCKMODE is typedef'd as int anyway, that seems like overkill.
 */
Relation parserOpenTable(ParseState *pstate, const RangeVar *relation, int lockmode, bool isFirstNode, bool isCreateView, bool isSupportSynonym) {return nullptr;}


/*
 * Add an entry for a relation to the pstate's range table (p_rtable).
 *
 * If pstate is NULL, we just build an RTE and return it without adding it
 * to an rtable list.
 *
 * Note: formerly this checked for refname conflicts, but that's wrong.
 * Caller is responsible for checking for conflicts in the appropriate scope.
 */
#ifdef ENABLE_MULTIPLE_NODES
#endif
#ifdef PGXC
#endif

/*
 * Add an entry for a relation to the pstate's range table (p_rtable).
 *
 * This is just like addRangeTableEntry() except that it makes an RTE
 * given an already-open relation instead of a RangeVar reference.
 */
RangeTblEntry* addRangeTableEntryForRelation(ParseState* pstate, Relation rel, Alias* alias, bool inh, bool inFromCl)
{
    RangeTblEntry* rte = makeNode(RangeTblEntry);
    char* refname = NULL;

    if (alias != NULL) {
        refname = alias->aliasname;
    } else if (OidIsValid(rel->rd_refSynOid)) {
        refname = GetQualifiedSynonymName(rel->rd_refSynOid, false);
    } else {
        refname = RelationGetRelationName(rel);
    }

    rte->rtekind = RTE_RELATION;
    rte->alias = alias;
    rte->relid = RelationGetRelid(rel);
    rte->relkind = rel->rd_rel->relkind;
    rte->is_ustore = RelationIsUstoreFormat(rel);
    rte->ispartrel = RELATION_IS_PARTITIONED(rel);
    rte->relhasbucket = RELATION_HAS_BUCKET(rel);
    rte->bucketmapsize = rel->rd_bucketmapsize;
    rte->isexcluded = false;
    /*
     * In cases that target relation's rd_refSynOid is valid, it has been referenced from one synonym.
     * thus, alias name is used to take its raw relname away in order to form the refname.
     */
    rte->refSynOid = rel->rd_refSynOid;

#ifdef PGXC
    rte->relname = pstrdup(RelationGetRelationName(rel));
    rte->partAttrNum = get_rte_part_attr_num(rel);
#endif

    /*
     * Build the list of effective column names using user-supplied aliases
     * and/or actual column names.
     */
    rte->eref = makeAlias(refname, NIL);
    buildRelationAliases(rel->rd_att, alias, rte->eref);

    /*
     * The initial default on access checks is always check-for-READ-access,
     * which is the right thing for all except target tables.
     */
    set_rte_flags(rte, inh, inFromCl, ACL_SELECT);
    rte->lateral = false;

    setRteOrientation(rel, rte);

    /*
     * Add completed RTE to pstate's range table list, but not to join list
     * nor namespace --- caller must do that if appropriate.
     */
    if (pstate != NULL) {
        pstate->p_rtable = lappend(pstate->p_rtable, rte);
        /*
         * If rte added into p_rtable is referenced by synonym object,
         * mark pstate->p_hasSynonyms.
         */
        if (!pstate->p_hasSynonyms && OidIsValid(rte->refSynOid)) {
            pstate->p_hasSynonyms = true;
        }
    }

    return rte;
}

/*
 * Add an entry for a subquery to the pstate's range table (p_rtable).
 *
 * This is just like addRangeTableEntry() except that it makes a subquery RTE.
 * Note that an alias clause *must* be supplied.
 */


/*
 * Add an entry for a function to the pstate's range table (p_rtable).
 *
 * This is just like addRangeTableEntry() except that it makes a function RTE.
 */


/*
 * Add an entry for a VALUES list to the pstate's range table (p_rtable).
 *
 * This is much like addRangeTableEntry() except that it makes a values RTE.
 */


/*
 * Add an entry for a join to the pstate's range table (p_rtable).
 *
 * This is much like addRangeTableEntry() except that it makes a join RTE.
 */


/*
 * Add an entry for a CTE reference to the pstate's range table (p_rtable).
 *
 * This is much like addRangeTableEntry() except that it makes a CTE RTE.
 */


/* return the rangeTableEntry according to the relation */
#ifdef PGXC
#endif

/*
 * Has the specified refname been selected FOR UPDATE/FOR SHARE?
 *
 * This is used when we have not yet done transformLockingClause, but need
 * to know the correct lock to take during initial opening of relations.
 *
 * Note: we pay no attention to whether it's FOR UPDATE vs FOR SHARE,
 * since the table-level lock is the same either way.
 */


/*
 * Add the given RTE as a top-level entry in the pstate's join list
 * and/or name space lists.  (We assume caller has checked for any
 * namespace conflicts.)
 */


/*
 * expandRTE -- expand the columns of a rangetable entry
 *
 * This creates lists of an RTE's column names (aliases if provided, else
 * real names) and Vars for each column.  Only user columns are considered.
 * If include_dropped is FALSE then dropped columns are omitted from the
 * results.  If include_dropped is TRUE then empty strings and NULL constants
 * (not Vars!) are returned for dropped columns.
 *
 * rtindex, sublevels_up, and location are the varno, varlevelsup, and location
 * values to use in the created Vars.  Ordinarily rtindex should match the
 * actual position of the RTE in its rangetable.
 *
 * The output lists go into *colnames and *colvars.
 * If only one of the two kinds of output list is needed, pass NULL for the
 * output pointer for the unwanted one.
 */


/*
 * expandRelation -- expandRTE subroutine
 */
static void expandRelation(Oid relid, Alias* eref, int rtindex, int sublevels_up, int location, bool include_dropped, List** colnames, List** colvars){return;}


/*
 * expandTupleDesc -- expandRTE subroutine
 */


/*
 * getValuesTypmods -- expandRTE subroutine
 *
 * Identify per-column typmods for the given VALUES RTE.  Returns a
 * palloc'd array.
 */


/*
 * expandRelAttrs -
 *	  Workhorse for "*" expansion: produce a list of targetentries
 *	  for the attributes of the RTE
 *
 * As with expandRTE, rtindex/sublevels_up determine the varno/varlevelsup
 * fields of the Vars produced, and location sets their location.
 * pstate->p_next_resno determines the resnos assigned to the TLEs.
 * The referenced columns are marked as requiring SELECT access.
 */


/*
 * get_rte_attribute_name
 *		Get an attribute name from a RangeTblEntry
 *
 * This is unlike get_attname() because we use aliases if available.
 * In particular, it will work on an RTE for a subselect or join, whereas
 * get_attname() only works on real relations.
 *
 * "*" is returned if the given attnum is InvalidAttrNumber --- this case
 * occurs when a Var represents a whole tuple of a relation.
 *
 * Must free the pointer after usage!!!
 */


/*
 * get_rte_attribute_type
 *		Get attribute type/typmod/collation information from a RangeTblEntry
 */


/*
 * get_rte_attribute_is_dropped
 *		Check whether attempted attribute ref is to a dropped column
 */


/*
 * Given a targetlist and a resno, return the matching TargetEntry
 *
 * Returns NULL if resno is not present in list.
 *
 * Note: we need to search, rather than just indexing with list_nth(),
 * because not all tlists are sorted by resno.
 */


/*
 * Given a Query and rangetable index, return relation's RowMarkClause if any
 *
 * Returns NULL if relation is not selected FOR UPDATE/SHARE
 */


/*
 *	given relation and att name, return attnum of variable
 *
 *	Returns InvalidAttrNumber if the attr doesn't exist (or is dropped).
 *
 *	This should only be used if the relation is already
 *	heap_open()'ed.  Use the cache version get_attnum()
 *	for access to non-opened relations.
 */
#ifdef DOLPHIN
#else
#endif

/* specialAttNum()
 *
 * Check attribute name to see if it is "special", e.g. "oid".
 * - thomas 2000-02-07
 *
 * Note: this only discovers whether the name could be a system attribute.
 * Caller needs to verify that it really is an attribute of the rel,
 * at least in the case of "oid", which is now optional.
 */
#ifdef PGXC
#else
#endif

/*
 * given attribute id, return name of that attribute
 *
 *	This should only be used if the relation is already
 *	heap_open()'ed.  Use the cache version get_atttype()
 *	for access to non-opened relations.
 */


/*
 * given attribute id, return type of that attribute
 *
 *	This should only be used if the relation is already
 *	heap_open()'ed.  Use the cache version get_atttype()
 *	for access to non-opened relations.
 */


/*
 * given attribute id, return collation of that attribute
 *
 *	This should only be used if the relation is already heap_open()'ed.
 */


/*
 * Generate a suitable error about a missing RTE.
 *
 * Since this is a very common type of error, we work rather hard to
 * produce a helpful message.
 */
void errorMissingRTE(ParseState* pstate, RangeVar* relation, bool hasplus) {return;}


/*
 * Generate a suitable error about a missing column.
 *
 * Since this is a very common type of error, we work rather hard to
 * produce a helpful message.
 */


/*
 * Brief        : Set relation store format.
 * Input        : rel, the table relation .
 *                rte, the range table entry of relation.
 * Output       : None.
 * Return Value : None.
 * Notes        : 1. If the Dfs table is CU format, mark as REL_COL_ORIENTED,
 *                otherwise, mark as REL_PAX_ORIENTED.
 */
static void setRteOrientation(Relation rel, RangeTblEntry* rte)
{
    if (RelationIsCUFormat(rel)) {
        rte->orientation = REL_COL_ORIENTED;
#ifdef ENABLE_MULTIPLE_NODES
    } else if (RelationIsPAXFormat(rel)) {
        rte->orientation = REL_PAX_ORIENTED;

        /* Make user to acquire only data on HDFS If is under analyzing only for HDFS table. */
        if (!u_sess->analyze_cxt.is_under_analyze) {
            /* Forbid user to acquire only data on HDFS. */
            rte->inh = true;
        }
    } else if(RelationIsTsStore(rel)) {
        rte->orientation = REL_TIMESERIES_ORIENTED;
#endif
    } else {
        rte->orientation = REL_ROW_ORIENTED;
    }
}
#ifdef DOLPHIN
/*
 * get_idxhint_relid
 *		Given hintname, and the relation index list, check if there is one and
 *      only one index fullfill the fuzzy condition hintname like 'indexname*',
 *
 * Returns oid of the index or InvalidOid if there is no such index.
 */

#endif

/*check index in the table , and mixd write force and use*/
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif

typedef struct change_target_list_attrno_context {
    Oid rel_oid;
    int oldAttnum;
    int newAttnum;
    int target_varno;
    int sublevels_up;
} change_target_list_attrno_context;

/*
 * return true if we found rte in current query->rtable, and save sequence to context,
 * which is the target varno we need to match target Vars.
 */


/*
 * walk through query, to search vars which match target_varno, sublevels_up and oldAttnum,
 * and set newAttnum.
 */


/*
 * correct varattno of vars in query, since it maybe different from the past.
 */

