/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - LookupTypeName
 * - typeTypeId
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * parse_type.cpp
 *		handle type operations for parser
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/common/backend/parser/parse_type.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"
#include "gaussdb_version.h"

#include "access/transam.h"
#ifdef DOLPHIN
#include "access/xact.h"
#endif
#include "catalog/namespace.h"
#include "catalog/gs_package.h"
#ifdef DOLPHIN
#include "catalog/pg_enum.h"
#include "catalog/pg_type_fn.h"
#endif
#include "catalog/pg_type.h"
#include "lib/stringinfo.h"
#include "nodes/makefuncs.h"
#include "optimizer/nodegroups.h"
#include "plugin_parser/parser.h"
#include "plugin_parser/parse_type.h"
#include "pgxc/groupmgr.h"
#include "pgxc/pgxc.h"
#ifdef DOLPHIN
#include "utils/acl.h"
#endif
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/date.h"
#include "utils/datum.h"
#ifdef DOLPHIN
#include "utils/fmgroids.h"
#endif
#include "utils/fmgrtab.h"
#include "utils/lsyscache.h"
#include "utils/timestamp.h"
#include "utils/syscache.h"
#include "utils/pl_package.h"
#include "catalog/gs_collation.h"
#include "plugin_parser/parse_utilcmd.h"
#include "catalog/pg_object.h"
#include "catalog/gs_dependencies_fn.h"
#include "catalog/pg_type_fn.h"
#ifdef DOLPHIN
#include "miscadmin.h"
#include "commands/typecmds.h"
#include "storage/lock/lock.h"
#endif

static int32 typenameTypeMod(ParseState* pstate, const TypeName* typname, Type typ);

static bool IsTypeInBlacklist(Oid typoid);





/*
 * LookupTypeName
 *       Wrapper for typical case.
 */
Type LookupTypeName(ParseState *pstate, const TypeName *typeName, int32 *typmod_p, bool print_notice, TypeDependExtend* dependExtend){return NULL;}


/*
 * LookupTypeNameExtended
 *		Given a TypeName object, lookup the pg_type syscache entry of the type.
 *		Returns NULL if no such type can be found.	If the type is found,
 *		the typmod value represented in the TypeName struct is computed and
 *		stored into *typmod_p.
 *
 * NB: on success, the caller must ReleaseSysCache the type tuple when done
 * with it.
 *
 * NB: direct callers of this function MUST check typisdefined before assuming
 * that the type is fully valid.  Most code should go through typenameType
 * or typenameTypeId instead.
 *
 * typmod_p can be passed as NULL if the caller does not care to know the
 * typmod value, but the typmod decoration (if any) will be validated anyway,
 * except in the case where the type is not found.	Note that if the type is
 * found but is a shell, and there is typmod decoration, an error will be
 * thrown --- this is intentional.
 *
 * If temp_ok is false, ignore types in the temporary namespace.  Pass false
 * when the caller will decide, using goodness of fit criteria, whether the
 * typeName is actually a type or something else.  If typeName always denotes
 * a type (or denotes nothing), pass true.
 *
 * pstate is only used for error location info, and may be NULL.
 */


/*
 * typenameType - given a TypeName, return a Type structure and typmod
 *
 * This is equivalent to LookupTypeName, except that this will report
 * a suitable error message if the type cannot be found or is not defined.
 * Callers of this can therefore assume the result is a fully valid type.
 */


/*
 * typenameTypeId - given a TypeName, return the type's OID
 *
 * This is similar to typenameType, but we only hand back the type OID
 * not the syscache entry.
 */


/*
 * typenameTypeIdAndMod - given a TypeName, return the type's OID and typmod
 *
 * This is equivalent to typenameType, but we only hand back the type OID
 * and typmod, not the syscache entry.
 */

void typenameTypeIdAndMod(ParseState* pstate, const TypeName* typname, Oid* typeid_p, int32* typmod_p, TypeDependExtend* dependExtend) 
{ 
 return;
}



/*
 * typenameTypeMod - given a TypeName, return the internal typmod value
 *
 * This will throw an error if the TypeName includes type modifiers that are
 * illegal for the data type.
 *
 * The actual type OID represented by the TypeName must already have been
 * looked up, and is passed as "typ".
 *
 * pstate is only used for error location info, and may be NULL.
 */


/*
 * appendTypeNameToBuffer
 *		Append a string representing the name of a TypeName to a StringInfo.
 *		This is the shared guts of TypeNameToString and TypeNameListToString.
 *
 * NB: this must work on TypeNames that do not describe any actual type;
 * it is mostly used for reporting lookup errors.
 */


/*
 * TypeNameToString
 *		Produce a string representing the name of a TypeName.
 *
 * NB: this must work on TypeNames that do not describe any actual type;
 * it is mostly used for reporting lookup errors.
 */


/*
 * TypeNameListToString
 *		Produce a string representing the name(s) of a List of TypeNames
 */


/*
 * LookupCollation
 *
 * Look up collation by name, return OID, with support for error location.
 */
#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN

#endif

Oid get_column_def_collation_b_format(ColumnDef* coldef, Oid typeOid, Oid typcollation, bool is_bin_type, Oid rel_coll_oid) {return 0;}


/*
 * GetColumnDefCollation
 *
 * Get the collation to be used for a column being defined, given the
 * ColumnDef node and the previously-determined column type OID.
 *
 * pstate is only used for error location purposes, and can be NULL.
 */
#ifdef DOLPHIN
#else
#endif

/* return a Type structure, given a type id */
/* NB: caller must ReleaseSysCache the type tuple when done with it */


/* given type (as type struct), return the type OID */
Oid typeTypeId(Type tp)
{
    if (tp == NULL)  { /* probably useless */ 
        ereport(ERROR, (errcode(ERRCODE_UNRECOGNIZED_NODE_TYPE), errmsg("typeTypeId() called with NULL type struct")));
    }
    return HeapTupleGetOid(tp);
}

/* given type (as type struct), return the length of type */


/* given type (as type struct), return its 'byval' attribute */


/* given type (as type struct), return the type's name */


/* given type (as type struct), return its 'typrelid' attribute */


/* given type (as type struct), return its 'typcollation' attribute */


/*
 * Given a type structure and a string, returns the internal representation
 * of that string.	The "string" can be NULL to perform conversion of a NULL
 * (which might result in failure, if the input function rejects NULLs).
 *
 * With param can_ignore == true, truncation or transformation may be cast
 * for input string if string is invalid for target type.
 */
#ifndef DOLPHIN
#else
#endif
#ifdef RANDOMIZE_ALLOCATED_MEMORY
#endif



#ifdef DOLPHIN
#ifdef RANDOMIZE_ALLOCATED_MEMORY
#endif
#endif

/* given a typeid, return the type's typrelid (associated relation, if any) */


/*
 * error context callback for parse failure during parseTypeString()
 */


/*
 * Given a string that is supposed to be a SQL-compatible type declaration,
 * such as "int4" or "integer" or "character varying(32)", parse
 * the string and convert it to a type OID and type modifier.
 */


/*
 * Given a string that is supposed to be a SQL-compatible type declaration,
 * such as "int4" or "integer" or "character varying(32)", parse
 * the string and return the result as a TypeName.
 * If the string cannot be parsed as a type, an error is raised.
 */


/*
 * IsTypeSupportedByCStore
 *      Return true if the type is supported by column store
 *
 * The performance of this function relies on compiler to flat the branches. But
 * it is ok if compiler failed to do its job as it is not in critical code path.
 */




/*
 * IsTypeSupportedByORCRelation
 * Return true if the type is supported by ORC format relation.
 */






/*
 * Used in tsdb. Return true if the type is supported by tsdb.
 * Supported types for tags: text
 * Supported types for fields and tag: numeric, text, bool, int, float, double
 * for upgrade, if create the new table using new support data type before commit after all nodes are new version,
 * after rollback to old version, insert will core, so if we support new data type, we must use version number to
 * control only can create the table after commit 
 * Parameters:
 *  - kvtype: whether the column is a tag or a field
 *  - typeOid: oid of the data type
 */


/*
 * IsTypeSupportedByUStore
 *    Return true if the type is supported by UStore
 *
 * The performance of this function relies on compiler to flat the branches. But
 * it is ok if compiler failed to do its job as it is not in critical code path.
 */


/* Check whether the type is in blacklist. */
#ifdef ENABLE_MULTIPLE_NODES
#endif /* ENABLE_MULTIPLE_NODES */

/* Check whether the type is in installation group. */


/*
 * find the type if it is a function type.
 * typeName: type name
 * return : function type oid
 */






/* find if %type ref a package variable type */
#ifdef ENABLE_MULTIPLE_NODES
#else
#endif



/* find the type if it is a package type */


#ifdef DOLPHIN

#else

/* find type in curr function */
Oid LookupTypeInFunc(const char *typeName)
{
    Oid typOid = InvalidOid;
    char* castTypeName = NULL;
    Oid func_oid = InvalidOid;

    if (u_sess->plsql_cxt.curr_compile_context != NULL
        && u_sess->plsql_cxt.curr_compile_context->plpgsql_curr_compile != NULL) {
        func_oid = u_sess->plsql_cxt.curr_compile_context->plpgsql_curr_compile->fn_oid;
    }

    if (!OidIsValid(func_oid)) {
        return typOid;
    }

    castTypeName = CastPackageTypeName(typeName, func_oid, false, true);
    Oid namespaceId = getCurrentNamespace();
    if (OidIsValid(namespaceId)) {
        typOid = GetSysCacheOid2(TYPENAMENSP, PointerGetDatum(castTypeName), ObjectIdGetDatum(namespaceId));
        if (!OidIsValid(typOid)) {
            typOid = TryLookForSynonymType(castTypeName, namespaceId);
        }
    }
    pfree_ext(castTypeName);
    return typOid;
}

bool IsBinaryType(Oid typid)
{
    if (u_sess->hook_cxt.isBinaryType != NULL) {
        return ((isBinaryType)(u_sess->hook_cxt.isBinaryType))(typid);
    }
    return ((typid) == BLOBOID ||
            (typid) == BYTEAOID);
}
#endif





#ifdef DOLPHIN
/*
 * DefineAnonymousEnum
 *		Registers a new anoymous enum without an array type, using the given name.
 */



#endif
