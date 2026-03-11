/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - resetOperatorPlusFlag
 * - getOperatorPlusFlag
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * parse_compatibility.cpp
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *    src/common/backend/parser/parse_compatibility.cpp
 *
 * -------------------------------------------------------------------------
 */


#include "postgres.h"
#include "knl/knl_variable.h"
#include "catalog/pg_type.h"
#include "commands/dbcommands.h"
#include "foreign/foreign.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "optimizer/clauses.h"
#include "optimizer/var.h"
#include "plugin_parser/analyze.h"
#include "plugin_parser/parse_coerce.h"
#include "plugin_parser/parse_collate.h"
#include "plugin_parser/parse_cte.h"
#include "plugin_parser/parse_clause.h"
#include "plugin_parser/parse_expr.h"
#include "plugin_parser/parse_func.h"
#include "plugin_parser/parse_oper.h"
#include "plugin_parser/parse_relation.h"
#include "plugin_parser/parse_target.h"
#include "plugin_parser/parse_type.h"
#include "plugin_parser/parse_agg.h"
#include "plugin_parser/parsetree.h"
#include "rewrite/rewriteManip.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/xml.h"

#include <string.h>

/*
 * The term of "JoinTerm" represents a convertable "(+)" outer join, normally it consists
 * of left & right rel and all conditions (and join filter) (in "t1.c1 = t1.c2 and t1.c2 = 1" format)
 *
 * - lrtf/rrtf: [left-joinrel, right-joinrel]
 * - quals: [aexpr1, aexpr2, aexpr3, aexpr4, ...], AExpr is a "t1.c1 = t2.c2(+)" format
 * - joincond: (t1.c1=t2.c1) AND (t1.c2=t2.c2) AND (t1.c2=t2.2)..
 *
 */
typedef struct JoinTerm {
    /* left/right range-table-entry */
    RangeTblEntry* lrte;
    RangeTblEntry* rrte;

    /*
     * list of single-column join expressions, {{t1.c1 = t2.c1}, {t1.c2 = t2.c2}}
     * Inserted when we do pre-process the whereClause
     */
    List* quals;

    /*
     * The join condition that will be put into the convert left/right outer join's
     * ON clause, it is created by AND all aggregate_jointerms()
     */
    Expr* joincond;

    /* Join type of current join term */
    JoinType jointype;
} JoinTerm;

/* Local function declerations */
static void preprocess_plus_outerjoin_walker(Node** expr, OperatorPlusProcessContext* ctx);
static void aggregate_jointerms(OperatorPlusProcessContext* ctx);
static void plus_outerjoin_check(const OperatorPlusProcessContext* ctx);
static void preprocess_plus_outerjoin(OperatorPlusProcessContext* ctx);
static void convert_plus_outerjoin(const OperatorPlusProcessContext* ctx);
static PlusJoinRTEInfo* makePlusJoinInfo(bool needrecord);
static void insert_jointerm(OperatorPlusProcessContext* ctx, Expr* expr, RangeTblEntry* lrte, RangeTblEntry* rrte);
static void find_joinexpr_walker(Node* node, const RangeTblRef* rtr, bool* found);
static bool find_joinexpr(JoinExpr* jexpr, const RangeTblRef* rtr);
static int find_RangeTblRef(RangeTblEntry* rte, List* p_rtable);
static Node* get_JoinArg(const OperatorPlusProcessContext* ctx, RangeTblEntry* rte);
static int plus_outerjoin_get_rtindex(const OperatorPlusProcessContext* ctx, Node* node, RangeTblEntry* rte);
static void convert_one_plus_outerjoin(const OperatorPlusProcessContext* ctx, JoinTerm* join_term);
static void InitOperatorPlusProcessContext(ParseState* pstate, Node** whereClause, OperatorPlusProcessContext* ctx);
static bool contain_ColumnRefPlus(Node* clause);
static bool contain_ColumnRefPlus_walker(Node* node, void* context);
static bool contain_ExprSubLink(Node* clause);
static bool contain_ExprSubLink_walker(Node* node, void* context);
static void unsupport_syntax_plus_outerjoin(const OperatorPlusProcessContext* ctx, Node* expr);
static bool contain_JoinExpr(List* l);
static bool contain_AEXPR_AND_OR(Node* clause);
static bool contain_AEXPR_AND_OR_walker(Node* node, void* context);
static bool assign_query_ignore_flag_walker(Node* node, void *context);

bool getOperatorPlusFlag()
{
    return u_sess->parser_cxt.stmt_contains_operator_plus;
}

void resetOperatorPlusFlag()
{
    u_sess->parser_cxt.stmt_contains_operator_plus = false;
    return;
}





/* Whether ignore operator "(+)", if ignore is true, means ignore */


/*
 * - insert_jointerm()
 *
 * - brief: Try to inert join_expr (t1.c1=t2.c1) into jointerm [RTE1,RTE, quals]
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * - aggregate_jointerms()
 *
 * - brief: aggregate the quals(in list format) into one AND-ed connected A_Expr
 */


/*
 * - find_joinexpr_walker()
 *
 * - brief: Recursive walker function for find_joinexpr(), if found returned in output
 *   parameter "found"
 */


/*
 * - find_joinexpr()
 *
 * - brief: Check if given RangeTblRef exists in jexpr(JoinExpr), major used in multi-table
 *          converted case where, e.g.
 */




/*
 * -get_JoinArg()
 *
 * - brief: get Join arg by RTE.
 * 		Transform RTE to RTR firstly. The item of p_joinlist can be RTR or JoinExpr. So We
 * will walk through all the RTR and larg/rarg of JoinExpr in p_joinlist.
 */


/*
 * - preprocess_plus_outerjoin()
 *
 * - brief: entry point function of of pre-process operator (+), in the "preprocess"
 *   stage we do following
 *      [1]. Find convertable join expr (t1.c1=t2.c1(+)) in whereClause and insert into ctx->jointerm->quals
 *         list and groupped in same join pairs, see detail in function "insert_jointerm()"
 *      [2]. After walk-thru the whole expressoin tree, we do aggregation of join exprs to
 *           form joincond that will put into OnClause
 *      [3]. Check the collect JoinTerm and mark a convertable jointerm as valid
 */




/*
 * - convert_one_plus_outerjoin()
 *
 * - brief: convert jointerm to JoinExpr
 */


/*
 * - plus_outerjoin_check()
 *
 * - brief: check the ctx->jointerms globally.
 *    Only forbid more than one left RTE with same right RTE for now.
 */


/*
 * - plus_outerjoin_precheck()
 *
 * - brief: check the join condition is valid.
 */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif

















/*
 * - preprocess_plus_outerjoin_walker()
 *
 * - brief: Function to recursively walk-thru the expression tree, if we found a portential
 * convertable col=col condition, we record into "ctx->jointerms".
 */


/*
 * - IsColumnRefPlusOuterJoin()
 *
 * - brief: help function to lookup if ColumnRef is "(+)" operator attached
 */


/*
 * - convert_plus_outerjoin()
 *
 * - brief: Convert each JoinTerms collected at preprocess step into outer joins
 */


/*
 * - InitOperatorPlusProcessContext()
 *
 * - brief: init OperatorPlusProcessContext
 */



/*
 * - transformPlusOperator()
 *
 * - brief: transform operator "(+)" to outer join.
 */


/*
 * assign_query_ignore_flag
 *             Mark all query in the given Query with ignore information.
 *
 * Now only INSERT and UPDATE support ignore, so it only be called in
 * InsertStmt and UpdateStmt.
 */


/*
 * Walker for assign_query_ignore_flag
 *
 * Each expression found by query_tree_walker is processed independently.
 * Note that query_tree_walker may pass us a whole List, such as the
 * targetlist, in which case each subexpression must be processed
 * independently.
 */


