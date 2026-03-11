/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - create_hintstate
 * - create_hintstate_worker
 * - output_hint_warning
 * - check_set_hint_in_white_list
 * - G_SET_HINT_WHITE_LIST
 * - G_NUM_SET_HINT_WHITE_LIST
 * - param_str_cmp
 * - HintStateCreate
 * - G_HINT_CREATOR
 * - AddJoinHint
 * - AddLeadingHint
 * - AddRowsHint
 * - AddStreamHint
 * - AddBlockNameHint
 * - AddScanMethodHint
 * - AddMultiNodeHint
 * - AddPredpushHint
 * - AddPredpushSameLevelHint
 * - AddSkewHint
 * - AddRewriteHint
 * - AddGatherHint
 * - AddSetHint
 * - AddPlanCacheHint
 * - AddNoExpandHint
 * - AddSqlIgnoreHint
 * - AddNoGPCHint
 * - drop_duplicate_blockname_hint
 * - delete_invalid_hint
 * - descHint
 * - JoinMethodHintDesc
 * - KeywordDesc
 * - G_HINT_KEYWORD
 * - relnamesToBuf
 * - LeadingHintDesc
 * - RowsHintDesc
 * - StreamHintDesc
 * - BlockNameHintDesc
 * - ScanMethodHintDesc
 * - SkewHintDesc
 * - append_space_mark
 * - append_value
 * - PredpushHintDesc
 * - PredpushSameLevelHintDesc
 * - RewriteHintDesc
 * - GatherHintDesc
 * - SetHintDesc
 * - PlanCacheHintDesc
 * - NoExpandHintDesc
 * - NoGPCHintDesc
 * - hintDelete
 * - JoinMethodHintDelete
 * - LeadingHintDelete
 * - RowsHintDelete
 * - StreamHintDelete
 * - BlockNameHintDelete
 * - ScanMethodHintDelete
 * - SkewHintDelete
 * - PredpushHintDelete
 * - PredpushSameLevelHintDelete
 * - RewriteHintDelete
 * - GatherHintDelete
 * - keep_last_hint_cell
 * - get_hints_from_comment
 *--------------------------------------------------------------------
 */

/*
 * Copyright (c) 2020 Huawei Technologies Co.,Ltd.
 *
 * openGauss is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 *
 * parse_hint.cpp
 *
 * IDENTIFICATION
 *    src/common/backend/parser/parse_hint.cpp
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"
#include "knl/knl_variable.h"

#include "nodes/nodes.h"
#include "nodes/nodeFuncs.h"
#include "plugin_parser/parse_hint.h"
#include "plugin_parser/scansup.h"
#include "plugin_optimizer/prep.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/numeric.h"
#include "utils/syscache.h"
#include "plugin_parser/parse_type.h"
#include "optimizer/var.h"

/* We must include  "plugin_parser/gramparse.h" after including "plugin_parser/parse_hint.h"
 * Otherwise, we will include the wrong header file.
 * Because we need to include hint_gram.hpp, not gram.hpp
 * Please see gramparse.h for the detail.
 */
#include "plugin_parser/gramparse.h"

#define NOTFOUNDRELNAME 0
#define AMBIGUOUSRELNAME (-1)

/* Too much skew value will cost to much memory and execution time, so we add a limit here. */
#define MAX_SKEW_NUM 10

/* free hint's base relnames if there are any */
#define HINT_FREE_RELNAMES(hint)                   \
    do {                                           \
        if ((hint)->base.relnames) {               \
            list_free_deep((hint)->base.relnames); \
        }                                          \
    } while (0)

typedef struct join_relid_relname {
    Relids relids;  /* Join relids*/
    List* rel_list; /* Join rel name.*/
} relid_relname;

static const char* KeywordDesc(HintKeyword keyword);
static void relnamesToBuf(List* relnames, StringInfo buf);
static void JoinMethodHintDesc(JoinMethodHint* hint, StringInfo buf);
static void LeadingHintDesc(LeadingHint* hint, StringInfo buf);
static void RowsHintDesc(RowsHint* hint, StringInfo buf);
static void StreamHintDesc(StreamHint* hint, StringInfo buf);
static void BlockNameHintDesc(BlockNameHint* hint, StringInfo buf);
static void ScanMethodHintDesc(ScanMethodHint* hint, StringInfo buf);
static void SkewHintDesc(SkewHint* hint, StringInfo buf);
static void find_unused_hint_to_buf(List* hint_list, StringInfo hint_buf);
static void JoinMethodHintDelete(JoinMethodHint* hint);
static void LeadingHintDelete(LeadingHint* hint);
static void RowsHintDelete(RowsHint* hint);
static void RewriteHintDelete(RewriteHint* hint);
static void GatherHintDelete(GatherHint* hint);
static void StreamHintDelete(StreamHint* hint);
static void BlockNameHintDelete(BlockNameHint* hint);
static void ScanMethodHintDelete(ScanMethodHint* hint);
static void SkewHintDelete(SkewHint* hint);
static void SkewHintTransfDelete(SkewHintTransf* hint);
static char* get_hints_from_comment(const char* comment_str);
static void drop_duplicate_blockname_hint(HintState* hstate);
static void drop_duplicate_join_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_stream_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_row_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_rewrite_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_gather_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_scan_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_skew_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_predpush_hint(PlannerInfo* root, HintState* hstate);
static void drop_duplicate_predpush_same_level_hint(PlannerInfo* root, HintState* hstate);
static int find_relid_aliasname(Query* parse, const char* aliasname, bool find_in_rtable = false);
static Relids create_bms_of_relids(
    PlannerInfo* root, Query* parse, Hint* hint, List* relnamelist = NIL, Relids currelids = NULL);
static List* set_hint_relids(PlannerInfo* root, Query* parse, List* l);
static void leading_to_join_hint(HintState* hstate, Relids join_relids, Relids inner_relids, List* relname_list);
static void transform_leading_hint(PlannerInfo* root, Query* parse, HintState* hstate);
static void transform_skew_hint(PlannerInfo* root, Query* parse, List* skew_hint_list);
static SkewHintTransf* set_skew_hint(PlannerInfo* root, Query* parse, SkewHint* skew_hint);
static void set_subquery_rel(
    Query* parse, RangeTblEntry* rte, SkewHintTransf* skew_hint_transf, RangeTblEntry* parent_rte = NULL);
static void set_base_rel(Query* parse, RangeTblEntry* rte, SkewHintTransf* skew_hint_transf);
static bool subquery_can_pull_up(RangeTblEntry* subquery_rte, Query* parse);
static void set_skew_column(PlannerInfo* root, Query* parse, SkewHintTransf* skew_hint_transf, Oid** col_typid);
static RangeTblEntry* find_column_in_rtable_subquery(Query* parse, const char* column_name, int* location);
static RangeTblEntry* find_column_in_rel_info_list(
    SkewHintTransf* skew_hint_transf, const char* column_name, int* count, int* location);
static TargetEntry* find_column_in_targetlist_by_location(RangeTblEntry* rte, int location, RangeTblEntry** col_rte);
static TargetEntry* find_column_in_targetlist_by_name(List* targetList, const char* column_name);
static bool check_parent_rte_is_diff(RangeTblEntry* parent_rte, List* parent_rte_list);
void pull_up_expr_varno(Query* parse, RangeTblEntry* rte, SkewColumnInfo* column_info);
static void set_colinfo_by_relation(Oid relid, int location, SkewColumnInfo* column_info, char* column_name);
static void set_colinfo_by_tge(
    TargetEntry* tge, SkewColumnInfo* column_info, char* column_name, RangeTblEntry* rte = NULL, Query* parse = NULL);
static void set_skew_value(PlannerInfo* root, SkewHintTransf* skew_hint_transf, Oid* col_typid);
static bool set_skew_value_to_datum(
    Value* skew_value, Datum* val_datum, bool* constisnull, Oid val_typid, int4 val_typmod, ErrorData** edata);
static int get_col_location(const char* column_name, List* eref_col);
static RangeTblEntry* get_rte(Query* parse, const char* skew_relname);
static char* get_name(ListCell* lc);
static bool support_redistribution(Oid typid);
static List* delete_invalid_hint(PlannerInfo* root, HintState* hint, List* list);
static Relids OuterInnerJoinCreate(Query* parse, HintState* hstate, List* relnames, bool join_order_hint);
static unsigned int get_rewrite_rule_bits(RewriteHint* hint);

extern yyscan_t hint_scanner_init(const char* str, hint_yy_extra_type* yyext);
extern void hint_scanner_destroy(yyscan_t yyscanner);
extern void hint_scanner_yyerror(const char* msg, yyscan_t yyscanner);
extern Datum GetDatumFromString(Oid typeOid, int4 typeMod, char* value);
extern Const* makeConst(Oid consttype, int32 consttypmod, Oid constcollid, int constlen, Datum constvalue,
    bool constisnull, bool constbyval, Cursor_Data* vars = NULL);
extern Numeric int64_to_numeric(int64 v);

static bool IsScanUseDesthint(void* val1, void* val2);

/* Expression kind codes for preprocess_expression */
#define EXPRKIND_QUAL 0
#define EXPRKIND_TARGET 1
#define EXPRKIND_RTFUNC 2
#define EXPRKIND_VALUES 3
#define EXPRKIND_LIMIT 4
#define EXPRKIND_APPINFO 5
#define EXPRKIND_TABLESAMPLE 6

extern Node* preprocess_expression(PlannerInfo* root, Node* expr, int kind);

void yyset_lineno(int line_number, yyscan_t yyscanner);

/* @Description: append space mark into the buffer
 * @inout buf: buffer
 * @in location: the current location
 * @in column_len: number of columns going to write
 */
static void append_space_mark(StringInfo buf, int location, int column_len)
{
    if (location == 0) {
        appendStringInfoCharMacro(buf, '(');
    } else if (column_len > 1) {
        appendStringInfoCharMacro(buf, ')');
        appendStringInfoCharMacro(buf, ' ');
        appendStringInfoCharMacro(buf, '(');
    } else {
        /* For case B, isfirst is always true */
        appendStringInfoCharMacro(buf, ' ');
    }
}

/* @Description: append actual value into the buffer
 * @inout buf: buffer
 * @in value: the Value node going to write
 * @in node: parent node if there is any
 */
static void append_value(StringInfo buf, Value* value, Node* node)
{
    if (IsA(node, Null)) {
        appendStringInfoString(buf, value->val.str);
    } else {
        appendStringInfoCharMacro(buf, '\'');
        appendStringInfoString(buf, value->val.str);
        appendStringInfoCharMacro(buf, '\'');
    }
}

#define HINT_NUM 17
#ifdef DOLPHIN
#define HINT_KEYWORD_NUM 24
#else
#define HINT_KEYWORD_NUM 23
#endif

typedef struct {
    HintKeyword keyword;
    char* keyStr;
} KeywordPair;

 const char* G_HINT_KEYWORD[HINT_KEYWORD_NUM] = {
    (char*) HINT_NESTLOOP,
    (char*) HINT_MERGEJOIN,
    (char*) HINT_HASHJOIN,
    (char*) HINT_LEADING,
    (char*) HINT_ROWS,
    (char*) HINT_BROADCAST,
    (char*) HINT_REDISTRIBUTE,
    (char*) HINT_BLOCKNAME,
    (char*) HINT_TABLESCAN,
    (char*) HINT_INDEXSCAN,
    (char*) HINT_INDEXONLYSCAN,
    (char*) HINT_SKEW,
    (char*) HINT_PRED_PUSH,
    (char*) HINT_PRED_PUSH_SAME_LEVEL,
    (char*) HINT_REWRITE,
    (char*) HINT_GATHER,
    (char*) HINT_NO_EXPAND,
    (char*) HINT_SET,
    (char*) HINT_CPLAN,
    (char*) HINT_GPLAN,
    (char*) HINT_SQL_IGNORE,
    (char*) HINT_CHOOSE_ADAPTIVE_GPLAN,
    (char*) HINT_NO_GPC,
#ifdef DOLPHIN
    (char*) HINT_SET_VAR,
#endif
};


/*
 * @Description: Describe hint keyword to string
 * @in keyword: hint keyword.
 */
static const char* KeywordDesc(HintKeyword keyword)
{
    const char* value = NULL;
    /* In case new tag is added within the old range. Keep the LFS as the newest keyword */
#ifdef DOLPHIN
    Assert(HINT_KEYWORD_SET_VAR == HINT_KEYWORD_NUM - 1);
#else
    Assert(HINT_KEYWORD_NO_GPC == HINT_KEYWORD_NUM - 1);
#endif
    if ((int)keyword >= HINT_KEYWORD_NUM || (int)keyword < 0) {
        elog(WARNING, "unrecognized keyword %d", (int)keyword);
    } else {
        value = G_HINT_KEYWORD[(int)keyword];
    }

    return value;
}

/*
 * @Description: Append relnames to buf.
 * @in relnames: Rel name list.
 * @out buf: String buf.
 */
static void relnamesToBuf(List* relnames, StringInfo buf)
{
    bool isfirst = true;
    char* relname = NULL;
    ListCell* lc = NULL;

    Assert(buf != NULL);
    foreach (lc, relnames) {
        Node* node = (Node*)lfirst(lc);
        if (IsA(node, String)) {
            Value* string_value = (Value*)node;
            relname = string_value->val.str;

            if (isfirst) {
                appendStringInfoString(buf, quote_identifier(relname));
                isfirst = false;
            } else {
                appendStringInfoCharMacro(buf, ' ');
                appendStringInfoString(buf, quote_identifier(relname));
            }
        } else if (IsA(node, List)) {
            if (isfirst) {
                appendStringInfoString(buf, "(");
                relnamesToBuf((List*)node, buf);
                appendStringInfoString(buf, ")");
                isfirst = false;
            } else {
                appendStringInfoCharMacro(buf, ' ');
                appendStringInfoString(buf, "(");
                relnamesToBuf((List*)node, buf);
                appendStringInfoString(buf, ")");
            }
        }
    }
}

/*
 * @Description: Collect predpush tables in the same level.
 * @in root: Root of the current SQL.
 * @out result: Relids of the tables which are predpush on.
 */


/*
 * is_predpush_same_level_matched
 *    Check if the predpush samwe level is matched.
 * @param predpush same level hint, relids from baserel, param path info.
 * @param check_dest: don't check dest id when create paths.
 * @return true if matched.
 */


/*
 * @Description: get the prompts for no_gpc hint into subquery.
 * @in hint: subquery no_gpc hint.
 * @out buf: String buf.
 */
static void NoGPCHintDesc(NoGPCHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    appendStringInfo(buf, " %s", KeywordDesc(hint->base.hint_keyword));
}

/*
 * @Description: get the prompts for no_expand hint into subquery.
 * @in hint: subquery no_expand hint.
 * @out buf: String buf.
 */
static void NoExpandHintDesc(NoExpandHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    appendStringInfo(buf, " %s", KeywordDesc(hint->base.hint_keyword));
}

/*
 * @Description: get the prompts for plancache hint into subquery.
 * @in hint: subquery plancache hint.
 * @out buf: String buf.
 */
static void PlanCacheHintDesc(PlanCacheHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    appendStringInfo(buf, " %s", KeywordDesc(hint->base.hint_keyword));
}

/*
 * @Description: get the prompts for set-guc hint into subquery.
 * @in hint: set-guc hint.
 * @out buf: String buf.
 */
static void SetHintDesc(SetHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->name != NULL) {
        appendStringInfo(buf, "%s", hint->name);
    }
    
    if (hint->value != NULL) {
        appendStringInfo(buf, " %s", hint->value);
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: get the prompts for redicate pushdown into subquery.
 * @in hint: predicate pushdown hint.
 * @out buf: String buf.
 */
static void PredpushHintDesc(PredpushHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->candidates != NULL)
        appendStringInfo(buf, "(");

    relnamesToBuf(base_hint.relnames, buf);

    if (hint->dest_name != NULL) {
        appendStringInfo(buf, ", %s", hint->dest_name);
    }

    if (hint->candidates != NULL)
        appendStringInfo(buf, ")");

    appendStringInfoString(buf, ")");
}

/*
 * @Description: get the prompts for predicate pushdown same level.
 * @in hint: predicate pushdown same level hint.
 * @out buf: String buf.
 */
static void PredpushSameLevelHintDesc(PredpushSameLevelHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->candidates != NULL) {
        appendStringInfo(buf, "(");
    }

    relnamesToBuf(base_hint.relnames, buf);
    if (hint->dest_name != NULL) {
        appendStringInfo(buf, ", %s", hint->dest_name);
    }

    if (hint->candidates != NULL) {
        appendStringInfo(buf, ")");
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: get the prompts for rewrite hint into subquery.
 * @in hint: rewrite hint.
 * @out buf: String buf.
 */
static void RewriteHintDesc(RewriteHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    relnamesToBuf(hint->param_names, buf);

    appendStringInfoString(buf, ")");
}

/*
 * @Description: get the prompts for redicate pushdown into subquery.
 * @in hint: predicate pushdown hint.
 * @out buf: String buf.
 */
static void GatherHintDesc(GatherHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->source == HINT_GATHER_REL) {
        appendStringInfo(buf, "REL");
    } else if (hint->source == HINT_GATHER_JOIN) {
        appendStringInfo(buf, "JOIN");
    } else {
        appendStringInfo(buf, "ALL");
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe join hint to string.
 * @in hint: Join hint.
 * @out buf: String buf.
 */
static void JoinMethodHintDesc(JoinMethodHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);
    if (hint->negative) {
        appendStringInfo(buf, " %s", HINT_NO);
    }

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->inner_joinrelids != NULL) {
        appendStringInfo(buf, "(");
    }

    relnamesToBuf(base_hint.relnames, buf);

    if (hint->inner_joinrelids != NULL) {
        appendStringInfo(buf, ")");
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe leading hint to string.
 * @in hint: Leading hint.
 * @in buf: String buf.
 */
static void LeadingHintDesc(LeadingHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);
    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    if (hint->join_order_hint) {
        appendStringInfoString(buf, "(");
    }

    relnamesToBuf(base_hint.relnames, buf);

    if (hint->join_order_hint) {
        appendStringInfoString(buf, ")");
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe rows hint to string.
 * @in hint: Rows hint.
 * @in buf: String buf.
 */
static void RowsHintDesc(RowsHint* hint, StringInfo buf)
{
    Hint base_hint = hint->base;

    Assert(buf != NULL);
    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    relnamesToBuf(base_hint.relnames, buf);

    switch (hint->value_type) {
        case RVT_ABSOLUTE:
            appendStringInfoString(buf, " #");
            break;
        case RVT_ADD:
            appendStringInfoString(buf, " +");
            break;
        case RVT_SUB:
            appendStringInfoString(buf, " -");
            break;
        case RVT_MULTI:
            appendStringInfoString(buf, " *");
            break;
        default:
            break;
    }

    if (hint->rows_str != NULL) {
        appendStringInfo(buf, " %s", hint->rows_str);
    } else {
        appendStringInfo(buf, " %.lf", hint->rows);
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe stream hint to string.
 * @in hint: Stream hint.
 * @in buf: String buf.
 */
static void StreamHintDesc(StreamHint* hint, StringInfo buf)
{
#ifndef ENABLE_MULTIPLE_NODES
    DISTRIBUTED_FEATURE_NOT_SUPPORTED();
#endif
    Hint base_hint = hint->base;

    Assert(buf != NULL);
    if (hint->negative) {
        appendStringInfo(buf, " %s", HINT_NO);
    }

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));
    relnamesToBuf(base_hint.relnames, buf);
    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe blockname hint to string.
 * @in hint: block hint.
 * @in buf: String buf.
 */
static void BlockNameHintDesc(BlockNameHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    Hint base_hint = hint->base;
    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));
    relnamesToBuf(base_hint.relnames, buf);
    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe scan hint to string.
 * @in hint: Scan hint.
 * @in buf: String buf.
 */
static void ScanMethodHintDesc(ScanMethodHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    Hint base_hint = hint->base;
    if (hint->negative) {
        appendStringInfo(buf, " %s", HINT_NO);
    }

    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));
    relnamesToBuf(base_hint.relnames, buf);

    if (hint->indexlist) {
        appendStringInfoCharMacro(buf, ' ');
        relnamesToBuf(hint->indexlist, buf);
    }

    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe skew hint to string.
 * @in hint: Skew hint.
 * @in buf: String buf.
 */
static void SkewHintDesc(SkewHint* hint, StringInfo buf)
{
    Assert(buf != NULL);

    Hint base_hint = hint->base;
    appendStringInfo(buf, " %s(", KeywordDesc(hint->base.hint_keyword));

    /* Skew hint formats are:
     *  case A: skew(t (c1) (v1))
     *  case B: skew(t (c1) (v1 v2 v3 ...))
     *  case C: skew(t (c1 c2) (v1 v2))
     *  case D: skew(t (c1 c2) ((v1 v2) (v3 v4) (v5 v6) ...))
     *  case F: skew((t1 t3) (c1 c2) ((v1 v2) (v3 v4) (v5 v6) ...))
     */
    if (list_length(hint->base.relnames) == 1) {
        relnamesToBuf(base_hint.relnames, buf);
    } else {
        appendStringInfoCharMacro(buf, '(');
        relnamesToBuf(base_hint.relnames, buf);
        appendStringInfoString(buf, ")");
    }

    if (hint->column_list) {
        appendStringInfoCharMacro(buf, ' ');
        appendStringInfoCharMacro(buf, '(');
        relnamesToBuf(hint->column_list, buf);
        appendStringInfoString(buf, ")");
    }

    if (hint->value_list) {
        int c_length = list_length(hint->column_list);
        int v_length = list_length(hint->value_list);
        ListCell* lc = NULL;
        bool isfirst = true;
        int location = 0;

        appendStringInfoCharMacro(buf, ' ');

        /* For case D */
        if (c_length != 1 && v_length > c_length) {
            appendStringInfoCharMacro(buf, '(');
        }

        foreach (lc, hint->value_list) {
            if (location % c_length == 0) {
                isfirst = true;
            } else {
                isfirst = false;
            }

            /* For null value. */
            Node* node = (Node*)lfirst(lc);
            Value* string_value = (Value*)node;
            char* null_val = "NULL";
            string_value->val.str = IsA(node, Null) ? null_val : string_value->val.str;

            switch (nodeTag(node)) {
                case T_Null:
                case T_BitString:
                case T_String:
                case T_Float: {
                    if (isfirst) {
                        append_space_mark(buf, location, c_length);
                        append_value(buf, string_value, node);
                    } else {
                        appendStringInfoCharMacro(buf, ' ');
                        append_value(buf, string_value, node);
                    }
                    break;
                }
                case T_Integer: {
                    if (isfirst) {
                        append_space_mark(buf, location, c_length);
                        appendStringInfo(buf, "%ld", string_value->val.ival);
                    } else {
                        appendStringInfoCharMacro(buf, ' ');
                        appendStringInfo(buf, "%ld", string_value->val.ival);
                    }
                    break;
                }
                default:
                    break;
            }
            location++;
        }
        if (c_length != 1 && v_length > c_length) {
            appendStringInfoCharMacro(buf, ')');
        }

        appendStringInfoCharMacro(buf, ')');
    }
    appendStringInfoString(buf, ")");
}

/*
 * @Description: Describe hint to string.
 * @in hint: Hint.
 * @in buf: String buf.
 */
char* descHint(Hint* hint)
{
    StringInfoData str;
    initStringInfo(&str);

    switch (hint->type) {
        case T_JoinMethodHint:
            JoinMethodHintDesc((JoinMethodHint*)hint, &str);
            break;
        case T_LeadingHint:
            LeadingHintDesc((LeadingHint*)hint, &str);
            break;
        case T_RowsHint:
            RowsHintDesc((RowsHint*)hint, &str);
            break;
        case T_StreamHint:
            StreamHintDesc((StreamHint*)hint, &str);
            break;
        case T_BlockNameHint:
            BlockNameHintDesc((BlockNameHint*)hint, &str);
            break;
        case T_ScanMethodHint:
            ScanMethodHintDesc((ScanMethodHint*)hint, &str);
            break;
        case T_SkewHint:
            SkewHintDesc((SkewHint*)hint, &str);
            break;
        case T_PredpushHint:
            PredpushHintDesc((PredpushHint*)hint, &str);
            break;
        case T_PredpushSameLevelHint:
            PredpushSameLevelHintDesc((PredpushSameLevelHint*)hint, &str);
            break;
        case T_RewriteHint:
            RewriteHintDesc((RewriteHint*)hint, &str);
            break;
        case T_GatherHint:
            GatherHintDesc((GatherHint*)hint, &str);
            break;
        case T_SetHint:
            SetHintDesc((SetHint*)hint, &str);
            break;
        case T_PlanCacheHint:
            PlanCacheHintDesc((PlanCacheHint*)hint, &str);
            break;
        case T_NoExpandHint:
            NoExpandHintDesc((NoExpandHint*)hint, &str);
            break;
        case T_NoGPCHint:
            NoGPCHintDesc((NoGPCHint*)hint, &str);
            break;
        default:
            break;
    }

    return str.data;
}

/*
 * @Description: Append unused hint string into buf.
 * @in hint_list: Hint list.
 * @out hint_buf: Not used hint buf.
 */


/*
 * @Description: Append used or not used hint string into buf.
 * @in hstate: Hint State.
 * @out buf: Keep hint string.
 */


/*
 * @Description: Delete predicate pushdown, free memory.
 * @in hint: predicate pushdown hint.
 */
static void PredpushHintDelete(PredpushHint* hint)
{
    if (hint == NULL)
        return;

    HINT_FREE_RELNAMES(hint);

    bms_free(hint->candidates);
    pfree_ext(hint);
}

/*
 * @Description: Delete predicate pushdown same level, free memory.
 * @in hint: predicate pushdown same level hint.
 */
static void PredpushSameLevelHintDelete(PredpushSameLevelHint* hint)
{
    if (hint == NULL)
        return;

    HINT_FREE_RELNAMES(hint);

    bms_free(hint->candidates);
    pfree_ext(hint);
}

/*
 * @Description: Delete join method hint, free memory.
 * @in hint: Join hint.
 */
static void JoinMethodHintDelete(JoinMethodHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    bms_free(hint->joinrelids);
    bms_free(hint->inner_joinrelids);
    pfree_ext(hint);
}

/*
 * @Description: Delete leading hint.
 * @in hint: Leading hint.
 */
static void LeadingHintDelete(LeadingHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);
    
    pfree_ext(hint);
}

/*
 * @Description: Delete rows hint.
 * @in hint: Rows Hint.
 */
static void RowsHintDelete(RowsHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    bms_free(hint->joinrelids);
    pfree_ext(hint);
}

/*
 * @Description: Delete rows hint.
 * @in hint: Rows Hint.
 */
static void RewriteHintDelete(RewriteHint* hint)
{
    if (hint == NULL)
        return;

    HINT_FREE_RELNAMES(hint);

    pfree_ext(hint);
}

/*
 * @Description: Delete rows hint.
 * @in hint: Rows Hint.
 */
static void GatherHintDelete(GatherHint* hint)
{
    if (hint == NULL)
        return;
    pfree_ext(hint);
}

/*
 * @Description: Delete stream hint.
 * @in hint: Stream hint.
 */
static void StreamHintDelete(StreamHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    bms_free(hint->joinrelids);
    pfree_ext(hint);
}

/*
 * @Description: Delete BlockName hint.
 * @in hint: BlockName hint.
 */
static void BlockNameHintDelete(BlockNameHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    pfree_ext(hint);
}

/*
 * @Description: Delete scan hint.
 * @in hint: scan hint.
 */
static void ScanMethodHintDelete(ScanMethodHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    if (hint->indexlist) {
        list_free_deep(hint->indexlist);
    }

    pfree_ext(hint);
}

/*
 * @Description: Delete skew hint.
 * @in hint: skew hint.
 */
static void SkewHintDelete(SkewHint* hint)
{
    if (hint == NULL) {
        return;
    }

    HINT_FREE_RELNAMES(hint);

    if (hint->column_list) {
        list_free_deep(hint->column_list);
    }

    if (hint->value_list) {
        list_free_deep(hint->value_list);
    }

    pfree_ext(hint);
    hint = NULL;
}

/*
 * @Description: Delete skew hint.
 * @in hint: skew hint.
 */


/*
 * @Description: Delete hint, call different delete function according to type.
 * @in hint: Deleted hint.
 */
void hintDelete(Hint* hint)
{
    Assert(hint != NULL);

    switch (nodeTag(hint)) {
        case T_JoinMethodHint:
            JoinMethodHintDelete((JoinMethodHint*)hint);
            break;
        case T_LeadingHint:
            LeadingHintDelete((LeadingHint*)hint);
            break;
        case T_RowsHint:
            RowsHintDelete((RowsHint*)hint);
            break;
        case T_StreamHint:
            StreamHintDelete((StreamHint*)hint);
            break;
        case T_BlockNameHint:
            BlockNameHintDelete((BlockNameHint*)hint);
            break;
        case T_ScanMethodHint:
            ScanMethodHintDelete((ScanMethodHint*)hint);
            break;
        case T_SkewHint:
            SkewHintDelete((SkewHint*)hint);
            break;
        case T_PredpushHint:
            PredpushHintDelete((PredpushHint*)hint);
            break;
        case T_PredpushSameLevelHint:
            PredpushSameLevelHintDelete((PredpushSameLevelHint*)hint);
            break;
        case T_RewriteHint:
            RewriteHintDelete((RewriteHint*)hint);
            break;
        case T_GatherHint:
            GatherHintDelete((GatherHint*)hint);
            break;
        default:
            elog(WARNING, "unrecognized hint method: %d", (int)nodeTag(hint));
            break;
    }
}

/*
 * @Descroption: Delete HintState struct.
 * @void return
 */


/*
 * @Descroption: Create HintState struct.
 * @return: HintState struct.
 */
HintState* HintStateCreate()
{
    HintState* hstate = NULL;

    hstate = makeNode(HintState);

    hstate->nall_hints = 0;
    hstate->join_hint = NIL;
    hstate->leading_hint = NIL;
    hstate->row_hint = NIL;
    hstate->stream_hint = NIL;
    hstate->scan_hint = NIL;
    hstate->skew_hint = NIL;
    hstate->hint_warning = NIL;
    hstate->multi_node_hint = false;
    hstate->predpush_hint = NIL;
    hstate->predpush_same_level_hint = NIL;
    hstate->rewrite_hint = NIL;
    hstate->gather_hint = NIL;
    hstate->set_hint = NIL;
    hstate->cache_plan_hint = NIL;
    hstate->no_expand_hint = NIL;
    hstate->sql_ignore_hint = false;
    hstate->from_sql_patch = false;
    return hstate;
}

/*
 * @Description: Get hints from the comment in client-supplied query string.
 * @in comment_str: Comment string.
 * @return: Hint string.
 */
static char* get_hints_from_comment(const char* comment_str)
{
    char* head = NULL;
    int len;
    int comment_len = strlen(comment_str);
    int hint_start_len = strlen(HINT_START);
    int start_position = 0;
    int end_position = 0;
    errno_t rc;

    /* extract query head comment, hint string start with "\*+" */
    if (strncmp(comment_str, HINT_START, hint_start_len) != 0) {
        return NULL;
    }

    /* Find first is not space character. */
    for (start_position = hint_start_len; start_position < comment_len; start_position++) {
        if (comment_str[start_position] == '\n' || (!isspace(comment_str[start_position]))) {
            break;
        }
    }

    /* Find comment termination position. */
    for (end_position = comment_len - 1; end_position >= 0; end_position--) {
        if (comment_str[end_position] == '*') {
            break;
        }
    }

    /* Make a copy of hint. */
    len = end_position - start_position;

    if (len <= 0) {
        return NULL;
    }

    head = (char*)palloc(len + 1);
    rc = memcpy_s(head, len, comment_str + start_position, len);
    securec_check(rc, "\0", "\0");
    head[len] = '\0';

    return head;
}

/*
 * @Description: Delete repeated BlockName hint. If have more than one,
 * they will all be discarded. BlockName hint only can have one in a query block.
 * @in hstate: Hint state.
 */
static void drop_duplicate_blockname_hint(HintState* hstate)
{
    Assert(list_length(hstate->block_name_hint) > 1);
    ListCell* lc = list_head(hstate->block_name_hint);

    foreach (lc, hstate->block_name_hint) {
        if (lc == list_head(hstate->block_name_hint)) {
            continue;
        }

        /*
         * treat all but first one as duplicate hint, as there should
         * be only one block hint
         */
        Hint* hint = (Hint*)lfirst(lc);
        hint->state = HINT_STATE_DUPLICATION;
    }

    hstate->block_name_hint = delete_invalid_hint(NULL, hstate, hstate->block_name_hint);
}

static List* keep_last_hint_cell(List* hintList)
{
    if(list_length(hintList) <= 1) {
        return hintList;
    }
    Node* node = (Node*)copyObject(llast(hintList));
    list_free_deep(hintList);
    return lappend(NIL, node);
}

static void AddJoinHint(HintState* hstate, Hint* hint)
{
    hstate->join_hint = lappend(hstate->join_hint, hint);
}

static void AddLeadingHint(HintState* hstate, Hint* hint)
{
    hstate->leading_hint = lappend(hstate->leading_hint, hint);
}

static void AddRowsHint(HintState* hstate, Hint* hint)
{
    hstate->row_hint = lappend(hstate->row_hint, hint);
}

static void AddStreamHint(HintState* hstate, Hint* hint)
{
    hstate->stream_hint = lappend(hstate->stream_hint, hint);
}

static void AddBlockNameHint(HintState* hstate, Hint* hint)
{
    hstate->block_name_hint = lappend(hstate->block_name_hint, hint);
}

static void AddScanMethodHint(HintState* hstate, Hint* hint)
{
    hstate->scan_hint = lappend(hstate->scan_hint, hint);
}

static void AddSkewHint(HintState* hstate, Hint* hint)
{
    hstate->skew_hint = lappend(hstate->skew_hint, hint);
}

static void AddMultiNodeHint(HintState* hstate, Hint* hint)
{
    hstate->multi_node_hint = true;
}

static void AddPredpushHint(HintState* hstate, Hint* hint)
{
    hstate->predpush_hint = lappend(hstate->predpush_hint, hint);
}

static void AddPredpushSameLevelHint(HintState* hstate, Hint* hint)
{
    hstate->predpush_same_level_hint = lappend(hstate->predpush_same_level_hint, hint);
}

static void AddRewriteHint(HintState* hstate, Hint* hint)
{
    hstate->rewrite_hint = lappend(hstate->rewrite_hint, hint);
}

static void AddGatherHint(HintState* hstate, Hint* hint)
{
    hstate->gather_hint = lappend(hstate->gather_hint, hint);
}

static void AddSetHint(HintState* hstate, Hint* hint)
{
    hstate->set_hint = lappend(hstate->set_hint, hint);
}

static void AddPlanCacheHint(HintState* hstate, Hint* hint)
{
    hstate->cache_plan_hint = lappend(hstate->cache_plan_hint, hint);
}

static void AddNoExpandHint(HintState* hstate, Hint* hint)
{
    /* only keep one no_expand hint for each subquery */
    if (list_length(hstate->no_expand_hint) == 0) {
        hstate->no_expand_hint = lappend(hstate->no_expand_hint, hint);
    }
}

static void AddNoGPCHint(HintState* hstate, Hint* hint)
{
    /* only keep one no_gpc hint for each subquery */
    if (list_length(hstate->no_gpc_hint) == 0) {
        hstate->no_gpc_hint = lappend(hstate->no_gpc_hint, hint);
    }
}

static void AddSqlIgnoreHint(HintState* hstate, Hint* hint)
{
    hstate->sql_ignore_hint = true;
}

typedef void (*AddHintFunc)(HintState*, Hint*);

const AddHintFunc G_HINT_CREATOR[HINT_NUM] = {
    AddJoinHint,
    AddLeadingHint,
    AddRowsHint,
    AddStreamHint,
    AddBlockNameHint,
    AddScanMethodHint,
    AddMultiNodeHint,
    AddPredpushHint,
    AddPredpushSameLevelHint,
    AddSkewHint,
    AddRewriteHint,
    AddGatherHint,
    AddSetHint,
    AddPlanCacheHint,
    AddNoExpandHint,
    AddSqlIgnoreHint,
    AddNoGPCHint,
};

HintState* create_hintstate_worker(const char* hint_str)
{
    HintState* hstate = NULL;

    /* Initilized plan hint variable, which will be set in hint parser */
    u_sess->parser_cxt.hint_list = u_sess->parser_cxt.hint_warning = NIL;

    yyscan_t yyscanner;
    hint_yy_extra_type yyextra;

    /* initialize the flex scanner */
    yyscanner = hint_scanner_init(hint_str, &yyextra);

    yyset_lineno(1, yyscanner);

    /* we will go on whether yyparse is successful or not. */
    (void)yyparse(yyscanner);

    hint_scanner_destroy(yyscanner);

    hstate = HintStateCreate();

    if (u_sess->parser_cxt.hint_list != NULL) {
        ListCell* lc = NULL;
        int firstHintTag = T_JoinMethodHint; /* Do not add hint tag before JoinMethodHint. */
        int lastHintTag = T_NoGPCHint; /* Keep this as the last hint tag in nodes.h. */

        foreach (lc, u_sess->parser_cxt.hint_list) {
            Hint* hint = (Hint*)lfirst(lc);
            if (hint == NULL) {
                continue;
            }
            /* In case new tag is added within the old range. Keep the LFS as the newest keyword */
            Assert(lastHintTag == HINT_NUM - 1 + firstHintTag);
            if (nodeTag(hint) < firstHintTag || nodeTag(hint) > lastHintTag) {
                ereport(ERROR,
                    (errcode(ERRCODE_OPTIMIZER_INCONSISTENT_STATE),
                        (errmsg("[Internal Error]: Invalid hint type %d", (int)nodeTag(hint)),
                         errhint("Do not add new hint tag between %d and %d.", firstHintTag, lastHintTag))));
            }
            if (G_HINT_CREATOR[nodeTag(hint) - firstHintTag] == NULL) {
                ereport(ERROR,
                    (errcode(ERRCODE_OPTIMIZER_INCONSISTENT_STATE),
                        (errmsg("[Internal Error]: Hint add function not initialized %d", (int)nodeTag(hint)))));
            }
            G_HINT_CREATOR[nodeTag(hint) - firstHintTag](hstate, hint);
            hstate->nall_hints++;
        }

        list_free(u_sess->parser_cxt.hint_list);
        u_sess->parser_cxt.hint_list = NIL;
    }
    hstate->hint_warning = u_sess->parser_cxt.hint_warning;
    u_sess->parser_cxt.hint_warning = NIL;

    /* When nothing specified a hint, we free HintState and returns NULL. */
    if (hstate->nall_hints == 0 && hstate->hint_warning == NIL) {
        pfree_ext(hstate);
        hstate = NULL;
    } else {
        /* Delete repeated BlockName hint. */
        if (list_length(hstate->block_name_hint) > 1) {
            drop_duplicate_blockname_hint(hstate);
        }
        /* Only keep the last cplan/gplanhint */
        hstate->cache_plan_hint = keep_last_hint_cell(hstate->cache_plan_hint);
    }

    if (hstate && hstate->hint_warning != NULL) {
        u_sess->parser_cxt.has_hintwarning = true;
    }

    return hstate;
}

/*
 * @Description: Generate hint struct according to hint str.
 * @in hints: Hint string.
 * @return: Hintstate struct.
 */
HintState* create_hintstate(const char* hints)
{
    if (hints == NULL) {
        return NULL;
    }

    char* hint_str = NULL;

    hint_str = get_hints_from_comment(hints);
    if (hint_str == NULL) {
        return NULL;
    }

    HintState* hstate = create_hintstate_worker(hint_str);

    pfree_ext(hint_str);
    return hstate;
}

/*
 * @Description: Return index of relation which matches given aliasname.
 * @in pstate: State information used during parse analysis.
 * @in aliasname: rel name.
 * @return: if not found return 0. If same aliasname was used multiple times in a query,
 * return -1 else return relid.
 */


/*
 * @Description: Create relnames's relids.
 * @in root: query level info.
 * @in pstate: State information used during parse analysis.
 * @in relnames: relname list.
 * @return: relids.
 */


/*
 * @Description: Set accurate relids.
 * @in pstate: Query struct.
 * @in l: Hint list.
 */


/*
 * @Description: Find scan hint according to relid and hint_key_word.
 * @in hstate: HintState.
 * @in relid: Relids.
 * @in keyWord: Hint key word.
 * @return: Scan hint or Null.
 */


/*
 * @Descriptoion: Find join hint according to relid and join hint key word.
 * @in hstate: HintState.
 * @in joinrelids: Join relids.
 * @in innerrelids: Inner relids.
 * @in keyWord: Join hint key word.
 * @in leading: if leading hint also considered, default true.
 * @return: Join hint or NULL.
 */


/*
 * @Descriptoion: Find scan hint according to relid and scan hint key word.
 * @in hstate: HintState.
 * @in relids: Scan relids.
 * @in keyWord: Scan hint key word.
 * @return: Join hint or NULL.
 */


/*
 * @Description: According to leading hint, generate join hint.
 * @in hstate: Hint state.
 * @in join_relids: Join relids.
 * @in inner_relids: Inner relids.
 * @in relname_list: Rel name string.
 */


/*
 * @Description: Build join hint according to outer inner rels from leading hint.
 * @in parse: Query struct.
 * @in hstate: hint stats of current query level
 * @in relnames: Leading hint relnames.
 * @in join_order_hint: if join order is hinted
 * @return: Join relids.
 */


/*
 * @Description: Transform leading hint.
 * @in pstate: Query struct.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate and error hint from list.
 * @in list: Hint list.
 * @return: New list.
 */
static List* delete_invalid_hint(PlannerInfo* root, HintState* hint, List* list)
{
    ListCell* pre = NULL;
    ListCell* next = NULL;

    StringInfoData str_buf, warning_buf;
    initStringInfo(&str_buf);
    initStringInfo(&warning_buf);

    ListCell* lc = list_head(list);

    while (lc != NULL) {
        next = lnext(lc);

        Hint* hint = (Hint*)lfirst(lc);

        if (hint->state == HINT_STATE_DUPLICATION) {
            appendStringInfo(&str_buf, "%s", descHint(hint));

            hintDelete(hint);
            list = list_delete_cell(list, lc, pre);
        } else {
            pre = lc;
        }

        lc = next;
    }

    appendStringInfo(&warning_buf, "Duplicated or conflict hint:%s, will be discarded.", str_buf.data);

    /*
     * For blockname hint duplication, we detect it in parser phase, so append the info in hint
     * state, or append the info to planner info
     */
    if (root != NULL) {
        root->glob->hint_warning = lappend(root->glob->hint_warning, makeString(warning_buf.data));
    } else {
        /* In parse phase, we use String struct, since we will CopyObject() later */
        hint->hint_warning = lappend(hint->hint_warning, makeString(warning_buf.data));
    }

    return list;
}

/*
 * @Description: Delete duplicate join hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate stream hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate row hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate predpush hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate predpush same level hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate rewrite hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate gather hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate scan hint.
 * @in hstate: Hint state.
 */


/*
 * @Description: Delete duplicate skew hint.
 * @root: query info.
 * @in hstate: Hint state.
 */


/*
 * @Description: Column type whether can do redistribution, and now we do not support_extended_features.
 * @in typid: Column type oid.
 * @return: ture of false.
 */


/*
 * @Description: Get name from ListCell.
 * @in lc: ListCell stores Value struct..
 * @return: name.
 */


/*
 * @Description: Get relation`s RTE.
 * @in parse: Query struct.
 * @in skew_relname: relation name in skew hint.
 * @return: rel`s rte.
 */


/*
 * @Description: For subquery and CTE, to get the column location in sub targetlist.
 * @in column_name: column name in skew hint.
 * @in l_subquery_eref: subquery`s eref columns list.
 */


/* ------------------------------set skew value info---------------------------- */

/*
 * @Description: Transform skew value into datum.
 * @in skew_value: skew value in hint.
 * @in val_typid: column type oid.
 * @in val_typmod: column type mod.
 * @out val_datum: datum after transformed
 * @out constisnull: whether the value is NULL.
 */


/*
 * @Description:  Make skew value into const. If has error during transforming, then go to the next
 *                      value instead of returning with null value_info_list.
 * @in root: query level info.
 * @in skew_hint_transf: SkewHintTransf node used to store the info after transform.
 * @in col_typid: column type oid array.
 */


/* ------------------------------set skew column info---------------------------- */
/*
 * @Description: pull up varno of expr in column info if need.
 * @in parse: parse tree.
 * @in rte: subquery rte that the column belongs to.
 * @in column_info: column info struct.
 * @return: void.
 */


/*
 * @Description: Set column info by TargetEntry.
 * @in tge: TargetEntry of column.
 * @in column_name: column name.
 * @out column_info: column info after setting.
 */


/*
 * @Description: Set column info by base relation.
 * @in relid: relation oid that column belongs to.
 * @in location: same as the column attnum in relation.
 * @in column_name: column name.
 * @out column_info: column info after setting.
 */


/*
 * @Description: Check the rel whether comes from same subquery.
 * @in parent_rte: subquery rte.
 * @in parent_rte_list: subquery list.
 * @return: Rel comes from same subquery, return false. Otherwise return true.
 */


/*
 * @Description: Find column in tagetlist by name.
 * @in targetList: current parse`s targetList.
 * @in column_relname: column name.
 * @return: tge of the column.
 */


/*
 * @Description: Find column in targetlist by location in subquery`s column.
 * @in rte: subquery rte.
 * @in location: location in subquery.
 * @return: tge of the column.
 */


/*
 * @Description: Find column in subquery for the case that subquery pull up.
 * @in parse: parse tree.
 * @in column_name: column name.
 * @out location: column`s location in subquery.
 * @return: rte that the column belongs to.
 */


/*
 * @Description: Find column from rel_info_list of SkewHintTransf.
 * @in SkewHintTransf: SkewHintTransf node used to store the info after transform.
 * @in column_name: column name.
 * @out count: count the times the find column in rels.
 * @out location: column`s location in rel.
 * @return: rte that the column belongs to.
 */


/*
 * @Description: Set skew column info and store into column_info_list of skew_hint_transf.
 * @in root: query level info.
 * @in parse: parse tree.
 * @in SkewHintTransf: SkewHintTransf node used to store the info after transform.
 * @out col_typid: column type oid.
 */


/* -------------------------------set skew rel info----------------------------- */

/*
 * @Description: Judge the subquery whether can be pull up.
 * @in subquery_rte: the RangeTblEntry of subquery.
 * @in parse: Query struct.
 */


/*
 * @Description: Set base relations into rel_info_list.
 * @in parse: parse tree.
 * @in rte: rte of base rel.
 * @in skew_hint_transf: SkewHintTransf node used to store the info after transform.
 */


/*
 * @Description: Set subquery all related relations into rel_info_list.
 * @in parse: parse tree.
 * @in rte: rte of subquery.
 * @in skew_hint_transf: SkewHintTransf node used to store the info after transform.
 */


/*
 * @Description: Set skew relation info and store into rel_info_list of skew_hint_transf.
 * @in root: query level info.
 * @in parse: parse tree.
 * @in SkewHintTransf: SkewHintTransf node used to store the info after transform.
 */


/*
 * @Description: Transform only one SkewHint into SkewHintTransf form every time.
 * @in root: query level info.
 * @in parse: parse tree.
 * @in skew_hint: SkewHint struct.
 * @return: SkewHintTransf struct or NULL.
 */


/*
 * @Description: Transform skew hint into processible type, including:
 *  transfrom relation name to relation oid
 *  transform column name to column type oid
 *  transform value to corresponding type and make into const
 * @in root: query level info.
 * @in parse: parse tree.
 * @in skew_hint_list: SkewHint list.
 */


/*
 * Check Predpush hint rely on each other.
 */


/*
 * @Description: Transform predpush hint into processible type, including:
 *  transfrom subquery name
 * @in root: query level info.
 * @in parse: parse tree.
 * @in predpush_hint_list: predpush hint list.
 */


/*
 * @Description: Transform predpush same level hint into processible type, including:
 *  transfrom destination name
 * @in root: query level info.
 * @in parse: parse tree.
 * @in predpush_join_hint_list: predpush same level hint list.
 */


/*
 * @Description: Transform rewrite hint into bitmap.
 * @in root: query level info.
 * @in parse: parse tree.
 * @in rewrite_hint_list: Rewrite Hint list.
 */





/*
 * @Description: Check rewrite rule constraints hint:
 * @in root: Planner Info
 * @in params: Rewrite rule parameter
 */


/*
 * @Description: Check gather constraints hint:
 * @in root: Planner Info
 * @in src: Gather source
 */


/*
 * @Description: Get gather hint source:
 * @in root: Planner Info
 */


/*
 * @Description: Transform hint into handy form.
 *  create bitmap of relids from alias names, to make it easier to check
 *  whether a join path matches a join method hint.
 *  add join method hints which are necessary to enforce join order
 *  specified by Leading hint
 * @in root: query level info.
 * @in parse: Query struct.
 * @in hstate: hint state.
 */


/*
 * @Description: check validity of scan hint
 * @in root: planner info of current query level
 */


/*
 * @Description: Change scan rel id in scan hint of state, used in hdfs case.
 * @in hstate: Hint State.
 * @in oldIdx, newIdx: old and new table entry index.
 */








/*
 * @Description: Append used or not used hint string into buf.
 * @in hstate: Hint State.
 * @out buf: Keep hint string.
 */
void output_hint_warning(List* warning, int lev)
{
    ListCell* lc = NULL;

    foreach (lc, warning) {
        Value* msg = (Value*)lfirst(lc);
        ereport(lev, (errmodule(MOD_PLANHINT), errmsg("%s", strVal(msg))));
    }

    list_free_deep(warning);
    warning = NIL;
}

/*
 * enable predpush? we assume that 'No predpush' need to be the first element.
 */


 const char* G_SET_HINT_WHITE_LIST[] = {
    /* keep in the ascending alphabetical order of frequency */
    (char*)"best_agg_plan",
    (char*)"cost_weight_index",
    (char*)"cpu_index_tuple_cost",
    (char*)"cpu_operator_cost",
    (char*)"cpu_tuple_cost",
    (char*)"default_limit_rows",
#ifdef DOLPHIN
    (char*)"dolphin.optimizer_switch",
#endif
    (char*)"effective_cache_size",
    (char*)"enable_bitmapscan",
    (char*)"enable_broadcast",
    (char*)"enable_fast_query_shipping",
    (char*)"enable_functional_dependency",
    (char*)"enable_hashagg",
    (char*)"enable_hashjoin",
    (char*)"enable_index_nestloop",
    (char*)"enable_indexonlyscan",
    (char*)"enable_indexscan",
    (char*)"enable_inner_unique_opt",
    (char*)"enable_material",
    (char*)"enable_mergejoin",
    (char*)"enable_nestloop",
    (char*)"enable_remotegroup",
    (char*)"enable_remotejoin",
    (char*)"enable_remotelimit",
    (char*)"enable_remotesort",
    (char*)"enable_seqscan",
    (char*)"enable_sort",
    (char*)"enable_sortgroup_agg",
    (char*)"enable_stream_operator",
    (char*)"enable_stream_recursive",
    (char*)"enable_tidscan",
    (char*)"enable_trigger_shipping",
    (char*)"node_name",
    (char*)"partition_iterator_elimination",
    (char*)"partition_page_estimation",
    (char*)"query_dop",
    (char*)"random_page_cost",
    (char*)"rewrite_rule",
    (char*)"seq_page_cost",
    (char*)"try_vector_engine_strategy",
    (char*)"var_eq_const_selectivity"};


const unsigned int G_NUM_SET_HINT_WHITE_LIST = sizeof(G_SET_HINT_WHITE_LIST) / sizeof(G_SET_HINT_WHITE_LIST[0]);

static int param_str_cmp(const void *s1, const void *s2)
{
    const char *key = (const char *)s1;
    const char * const *arg = (const char * const *)s2;
    return pg_strcasecmp(key, *arg);
}

bool check_set_hint_in_white_list(const char* name)
{
    if (name == NULL) {
        return false;
    }
    char* res = (char*)bsearch((void *) name,
                               (void *) G_SET_HINT_WHITE_LIST,
                               G_NUM_SET_HINT_WHITE_LIST,
                               sizeof(char*),
                               param_str_cmp);
    return res != NULL;
}





/*
 * check if is dest hinttype, it's used by function list_cell_clear
 * val1: ScanMethodHint
 * val2: HintKeyword
 */





