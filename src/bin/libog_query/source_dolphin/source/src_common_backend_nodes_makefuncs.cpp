/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - makeDefElem
 * - makeSimpleA_Expr
 * - makeA_Expr
 * - makeFuncCall
 * - makeBoolConst
 * - makeConst
 * - makeNullTest
 * - makeTypeNameFromNameList
 * - makeDefElemExtended
 * - MakeDefElemWithLoc
 * - makeRangeVar
 * - makeNullAConst
 * - makeTypeName
 * - makeAlias
 * - makeGroupingSet
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * makefuncs.cpp
 *	  creator functions for primitive nodes. The functions here are for
 *	  the most frequently created nodes.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/common/backend/nodes/makefuncs.cpp
 *
 * -------------------------------------------------------------------------
 */
#ifndef FRONTEND_PARSER
#include "postgres.h"
#include "knl/knl_variable.h"
#else
#include "postgres_fe.h"
#endif /* FRONTEND_PARSER */

#include "catalog/pg_class.h"
#include "catalog/pg_type.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "storage/item/itemptr.h"
#include "mb/pg_wchar.h"

#ifndef FRONTEND_PARSER
#include "utils/lsyscache.h"
#endif /* FRONTEND_PARSER */

extern TypeName* SystemTypeName(char* name);

/*
 * makeA_Expr -
 *		makes an A_Expr node
 */
A_Expr* makeA_Expr(A_Expr_Kind kind, List* name, Node* lexpr, Node* rexpr, int location)
{
    A_Expr* a = makeNode(A_Expr);

    a->kind = kind;
    a->name = name;
    a->lexpr = lexpr;
    a->rexpr = rexpr;
    a->location = location;
    return a;
}

/*
 * makeSimpleA_Expr -
 *		As above, given a simple (unqualified) operator name
 */
A_Expr* makeSimpleA_Expr(A_Expr_Kind kind, char* name, Node* lexpr, Node* rexpr, int location)
{
    A_Expr* a = makeNode(A_Expr);

    a->kind = kind;
    a->name = list_make1(makeString((char*)name));
    a->lexpr = lexpr;
    a->rexpr = rexpr;
    a->location = location;
    return a;
}

/*
 * makeVar -
 *	  creates a Var node
 */



#ifndef FRONTEND_PARSER
/*
 * makeVarFromTargetEntry -
 *		convenience function to create a same-level Var node from a
 *		TargetEntry
 */


/*
 * makeWholeRowVar -
 *	  creates a Var node representing a whole row of the specified RTE
 *
 * A whole-row reference is a Var with varno set to the correct range
 * table entry, and varattno == 0 to signal that it references the whole
 * tuple.  (Use of zero here is unclean, since it could easily be confused
 * with error cases, but it's not worth changing now.)  The vartype indicates
 * a rowtype; either a named composite type, or RECORD.  This function
 * encapsulates the logic for determining the correct rowtype OID to use.
 *
 * If allowScalar is true, then for the case where the RTE is a function
 * returning a non-composite result type, we produce a normal Var referencing
 * the function's result directly, instead of the single-column composite
 * value that the whole-row notation might otherwise suggest.
 */
#ifdef PGXC
#endif /* PGXC */

/*
 * makeTargetEntry -
 *	  creates a TargetEntry node
 */


/*
 * flatCopyTargetEntry -
 *	  duplicate a TargetEntry, but don't copy substructure
 *
 * This is commonly used when we just want to modify the resno or substitute
 * a new expression.
 */

#endif /* !FRONTEND_PARSER */

/*
 * makeFromExpr -
 *	  creates a FromExpr node
 */


/*
 * makeColumnRef -
 *      creates a ColumnRef node
 */


/*
 * makeConst -
 *	  creates a Const node
 */
Const* makeConst(Oid consttype, int32 consttypmod, Oid constcollid, int constlen, Datum constvalue, bool constisnull,
    bool constbyval, Cursor_Data* cur)
{
    Const* cnst = makeNode(Const);

    cnst->consttype = consttype;
    cnst->consttypmod = consttypmod;
    cnst->constcollid = constcollid;
    cnst->constlen = constlen;
    cnst->constvalue = constvalue;
    cnst->constisnull = constisnull;
    cnst->constbyval = constbyval;
    cnst->location = -1; /* "unknown" */
    cnst->ismaxvalue = false;
#ifndef FRONTEND_PARSER
    if (cur != NULL) {
        CopyCursorInfoData(&cnst->cursor_data, cur);
    } else {
        cnst->cursor_data.cur_dno = -1;
    }
#endif
    return cnst;
}
/*
 * makeNullTest -
 *	  creates a Null Test expr like "expr is (NOT) NULL"
 */
NullTest* makeNullTest(NullTestType type, Expr* expr)
{
    NullTest* n = makeNode(NullTest);

    n->nulltesttype = type;
    n->arg = expr;

    return n;
}

Node* makeNullAConst(int location)
{
    A_Const* n = makeNode(A_Const);

    n->val.type = T_Null;
    n->location = location;

    return (Node*)n;
}

#ifndef FRONTEND_PARSER
/*
 * makeNullConst -
 *	  creates a Const node representing a NULL of the specified type/typmod
 *
 * This is a convenience routine that just saves a lookup of the type's
 * storage properties.
 */

#endif /* FRONTEND_PARSER */

/*
 * makeBoolConst -
 *	  creates a Const node representing a boolean value (can be NULL too)
 */
Node* makeBoolConst(bool value, bool isnull)
{
    /* note that pg_type.h hardwires size of bool as 1 ... duplicate it */
    return (Node*)makeConst(BOOLOID, -1, InvalidOid, 1, BoolGetDatum(value), isnull, true);
}

#ifndef FRONTEND_PARSER
/*
 * @@GaussDB@@
 * Target		: data partition
 * Brief		: creates a Const node representing a max value
 * Description	:
 * Notes		:
 */

#endif /* FRONTEND_PARSER */

/*
 * makeBoolExpr -
 *	  creates a BoolExpr node
 */


/*
 * makeBoolExpr -
 *	  creates a BoolExpr tree node.
 */


/*
 * makeAlias -
 *	  creates an Alias node
 *
 * NOTE: the given name is copied, but the colnames list (if any) isn't.
 */
Alias* makeAlias(const char* aliasname, List* colnames)
{
    Alias* a = makeNode(Alias);

#ifndef FRONTEND_PARSER
    a->aliasname = pstrdup(aliasname);
#else
    a->aliasname = strdup(aliasname);
#endif
    a->colnames = colnames;

    return a;
}

/*
 * makeRelabelType -
 *	  creates a RelabelType node
 */


/*
 * makeRangeVar -
 *	  creates a RangeVar node (rather oversimplified case)
 */
RangeVar* makeRangeVar(char* schemaname, char* relname, int location)
{
    RangeVar* r = makeNode(RangeVar);

    r->catalogname = NULL;
    r->schemaname = schemaname;
    r->relname = relname;
    r->partitionname = NULL;
    r->subpartitionname = NULL;
    r->inhOpt = INH_DEFAULT;
    r->relpersistence = RELPERSISTENCE_PERMANENT;
    r->alias = NULL;
    r->location = location;
    r->ispartition = false;
    r->issubpartition = false;
    r->partitionKeyValuesList = NIL;
    r->isbucket = false;
    r->buckets = NIL;
    r->length = 0;
    r->withVerExpr = false;
    r->partitionNameList = NIL;
    
    return r;
}

/*
 * makeTypeName -
 *	build a TypeName node for an unqualified name.
 *
 * typmod is defaulted, but can be changed later by caller.
 */
TypeName* makeTypeName(char* typnam)
{
    return makeTypeNameFromNameList(list_make1(makeString(typnam)));
}

/*
 * makeTypeNameFromNameList -
 *	build a TypeName node for a String list representing a qualified name.
 *
 * typmod is defaulted, but can be changed later by caller.
 */
TypeName* makeTypeNameFromNameList(List* names)
{
    TypeName* n = makeNode(TypeName);

    n->names = names;
    n->typmods = NIL;
    n->typemod = -1;
    n->location = -1;
    n->pct_rowtype = false;
    n->charset = PG_INVALID_ENCODING;
    return n;
}

/*
 * makeTypeNameFromOid -
 *	build a TypeName node to represent a type already known by OID/typmod.
 */


/*
 * makeFuncExpr -
 *	build an expression tree representing a function call.
 *
 * The argument expressions must have been transformed already.
 */


/*
 * makeDefElem -
 *	build a DefElem node
 *
 * This is sufficient for the "typical" case with an unqualified option name
 * and no special action.
 */
DefElem* makeDefElem(char* name, Node* arg)
{
    DefElem* res = makeNode(DefElem);

    res->defnamespace = NULL;
    res->defname = name;
    res->arg = arg;
    res->defaction = DEFELEM_UNSPEC;
    res->begin_location = -1;
    res->end_location = -1;

    return res;
}

/*
 * makeDefElem -
 *	build a DefElem node
 *
 * similar to makeDefElem, add begin_location and end_location params
 */
DefElem* MakeDefElemWithLoc(char* name, Node* arg, int begin_loc, int end_loc)
{
    DefElem* res = makeDefElem(name, arg);

    res->begin_location = begin_loc;
    res->end_location = end_loc;
    
    return res;    
}


/*
 * makeDefElemExtended -
 *	build a DefElem node with all fields available to be specified
 */
DefElem* makeDefElemExtended(char* nameSpace, char* name, Node* arg, DefElemAction defaction)
{
    DefElem* res = makeNode(DefElem);

    res->defnamespace = nameSpace;
    res->defname = name;
    res->arg = arg;
    res->defaction = defaction;
    res->begin_location = -1;
    res->end_location = -1;

    return res;
}

/*
 * makeHashFilter -
 *	  creates a Hash Filter expr for replicated node to filter tuple using hash
 */


/*
 * makeGroupingSet
 *
 */
GroupingSet* makeGroupingSet(GroupingSetKind kind, List* content, int location)
{
    GroupingSet* n = makeNode(GroupingSet);

    n->kind = kind;
    n->content = content;
    n->location = location;
    return n;
}

/*
 * makeTidConst -
 *	  creates a Const node representing a Tid value
 */


/*
 * makeFuncCall -
 *	  creates a FuncCall node
 */
FuncCall* makeFuncCall(List* funcname, List* args, int location)
{
    FuncCall* funcCall = NULL;
    funcCall = (FuncCall*)makeNode(FuncCall);
    funcCall->funcname = funcname;
    funcCall->colname = NULL;
    funcCall->args = args;
    funcCall->agg_star = FALSE;
    funcCall->func_variadic = FALSE;
    funcCall->agg_distinct = FALSE;
    funcCall->agg_order = NIL;
    funcCall->agg_filter = NULL;
    funcCall->aggKeep = NULL;
    funcCall->over = NULL;
    funcCall->location = location;
    funcCall->is_from_last = FALSE;
    funcCall->is_ignore_nulls = FALSE;

    return funcCall;
}

/*
 * Param -
 *	  creates a Param node
 */

#ifndef FRONTEND_PARSER
/*
 * makeIndexInfo
 *	  create an IndexInfo node
 */

#endif
