/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - get_namespace_oid
 * - recomputeNamespacePath
 * - SYS_NAMESPACE_NAME
 * - recomputeOverrideStackTempPath
 * - getCurrentNamespace
 * - NameListToString
 * - DeconstructQualifiedName
 * - FindDefaultConversionProc
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * namespace.cpp
 *	  code to support accessing and searching namespaces
 *
 * This is separate from pg_namespace.c, which contains the routines that
 * directly manipulate the pg_namespace system catalog.  This module
 * provides routines associated with defining a "namespace search path"
 * and implementing search-path-controlled searches.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 * IDENTIFICATION
 *	  src/common/backend/catalog/namespace.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/xact.h"
#include "access/xlog.h"
#ifdef PGXC
#include "access/transam.h"
#include "pgxc/pgxc.h"
#include "pgxc/poolmgr.h"
#include "pgxc/redistrib.h"
#endif
#include "catalog/gs_client_global_keys.h"
#include "catalog/gs_column_keys.h"
#include "catalog/dependency.h"
#include "catalog/pg_authid.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_conversion.h"
#include "catalog/pg_conversion_fn.h"
#include "catalog/pg_language.h"
#include "catalog/pg_namespace.h"
#include "catalog/pg_opclass.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_opfamily.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_proc_ext.h"
#include "catalog/gs_package.h"
#include "catalog/gs_package_fn.h"
#include "catalog/pg_synonym.h"
#include "catalog/pg_ts_config.h"
#include "catalog/pg_ts_dict.h"
#include "catalog/pg_ts_parser.h"
#include "catalog/pg_ts_template.h"
#include "catalog/pg_type.h"
#include "catalog/pgxc_class.h"
#include "catalog/pgxc_group.h"
#include "catalog/indexing.h"
#include "catalog/gs_db_privilege.h"
#include "commands/dbcommands.h"
#include "commands/proclang.h"
#include "commands/tablecmds.h"
#include "funcapi.h"
#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "parser/parse_func.h"
#include "storage/ipc.h"
#include "storage/lmgr.h"
#include "storage/sinval.h"
#include "utils/acl.h"
#include "utils/builtins.h"
#include "utils/fmgrtab.h"
#include "utils/guc.h"
#include "utils/inval.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"
#include "utils/syscache.h"
#include "utils/snapmgr.h"
#include "utils/pl_package.h"
#include "tcop/utility.h"
#include "nodes/nodes.h"
#include "c.h"
#include "pgstat.h"
#include "catalog/pg_proc_fn.h"
#include "parser/parse_type.h"
#include "catalog/gs_collation.h"

#ifdef ENABLE_MULTIPLE_NODES
#include "streaming/planner.h"
#endif
#define MAXSTRLEN ((1 << 11) - 1)
/*
 * The namespace search path is a possibly-empty list of namespace OIDs.
 * In addition to the explicit list, implicitly-searched namespaces
 * may be included:
 *
 * 1. If a TEMP table namespace has been initialized in this session, it
 * is implicitly searched first.  (The only time this doesn't happen is
 * when we are obeying an override search path spec that says not to use the
 * temp namespace, or the temp namespace is included in the explicit list.)
 *
 * 2. The system catalog namespace is always searched.	If the system
 * namespace is present in the explicit path then it will be searched in
 * the specified order; otherwise it will be searched after TEMP tables and
 * *before* the explicit list.	(It might seem that the system namespace
 * should be implicitly last, but this behavior appears to be required by
 * SQL99.  Also, this provides a way to search the system namespace first
 * without thereby making it the default creation target namespace.)
 *
 * For security reasons, searches using the search path will ignore the temp
 * namespace when searching for any object type other than relations and
 * types.  (We must allow types since temp tables have rowtypes.)
 *
 * The default creation target namespace is always the first element of the
 * explicit list.  If the explicit list is empty, there is no default target.
 *
 * The textual specification of search_path can include "$user" to refer to
 * the namespace named the same as the current user, if any.  (This is just
 * ignored if there is no such namespace.)	Also, it can include "pg_temp"
 * to refer to the current backend's temp namespace.  This is usually also
 * ignorable if the temp namespace hasn't been set up, but there's a special
 * case: if "pg_temp" appears first then it should be the default creation
 * target.	We kluge this case a little bit so that the temp namespace isn't
 * set up until the first attempt to create something in it.  (The reason for
 * klugery is that we can't create the temp namespace outside a transaction,
 * but initial GUC processing of search_path happens outside a transaction.)
 * activeTempCreationPending is TRUE if "pg_temp" appears first in the string
 * but is not reflected in activeCreationNamespace because the namespace isn't
 * set up yet.
 *
 * In bootstrap mode, the search path is set equal to "pg_catalog", so that
 * the system namespace is the only one searched or inserted into.
 * initdb is also careful to set search_path to "pg_catalog" for its
 * post-bootstrap standalone backend runs.	Otherwise the default search
 * path is determined by GUC.  The factory default path contains the PUBLIC
 * namespace (if it exists), preceded by the user's personal namespace
 * (if one exists).
 *
 * We support a stack of "override" search path settings for use within
 * specific sections of backend code.  namespace_search_path is ignored
 * whenever the override stack is nonempty.  activeSearchPath is always
 * the actually active path; it points either to the search list of the
 * topmost stack entry, or to baseSearchPath which is the list derived
 * from namespace_search_path.
 *
 * If baseSearchPathValid is false, then baseSearchPath (and other
 * derived variables) need to be recomputed from namespace_search_path.
 * We mark it invalid upon an assignment to namespace_search_path or receipt
 * of a syscache invalidation event for pg_namespace.  The recomputation
 * is done during the next non-overridden lookup attempt.  Note that an
 * override spec is never subject to recomputation.
 *
 * Any namespaces mentioned in namespace_search_path that are not readable
 * by the current user ID are simply left out of baseSearchPath; so
 * we have to be willing to recompute the path when current userid changes.
 * namespaceUser is the userid the path has been computed for.
 *
 * Note: all data pointed to by these List variables is in t_thrd.top_mem_cxt.
 */

/* Catalog schema that TSQL uses */
 char* SYS_NAMESPACE_NAME = "sys";


/* Local functions */
static void InitTempTableNamespace(void);
static void RemoveTempRelations(Oid tempNamespaceId);
static void NamespaceCallback(Datum arg, int cacheid, uint32 hashvalue);
static bool MatchNamedCall(HeapTuple proctup, int nargs, List* argnames, int** argnumbers, bool include_out);
static bool CheckTSObjectVisible(NameData name, SysCacheIdentifier id, Oid nmspace);
static bool InitTempTblNamespace();
static void CheckTempTblAlias();
static Oid GetTSObjectOid(const char* objname, SysCacheIdentifier id);
static FuncCandidateList FuncnameAddCandidates(FuncCandidateList resultList, HeapTuple procTup, List* argNames,
    Oid namespaceId, Oid objNsp, int nargs, CatCList* catList, bool expandVariadic, bool expandDefaults,
    bool includeOut, Oid refSynOid, bool enable_outparam_override = false);
#ifdef ENABLE_UT
void dropExistTempNamespace(char* namespaceName);
#else
static void dropExistTempNamespace(char* namespaceName);
#endif

/* These don't really need to appear in any header file */
Datum pg_table_is_visible(PG_FUNCTION_ARGS);
Datum pg_type_is_visible(PG_FUNCTION_ARGS);
Datum pg_function_is_visible(PG_FUNCTION_ARGS);
Datum pg_operator_is_visible(PG_FUNCTION_ARGS);
Datum pg_opclass_is_visible(PG_FUNCTION_ARGS);
Datum pg_opfamily_is_visible(PG_FUNCTION_ARGS);
Datum pg_collation_is_visible(PG_FUNCTION_ARGS);
Datum pg_conversion_is_visible(PG_FUNCTION_ARGS);
Datum pg_ts_parser_is_visible(PG_FUNCTION_ARGS);
Datum pg_ts_dict_is_visible(PG_FUNCTION_ARGS);
Datum pg_ts_template_is_visible(PG_FUNCTION_ARGS);
Datum pg_ts_config_is_visible(PG_FUNCTION_ARGS);
Datum pg_my_temp_schema(PG_FUNCTION_ARGS);
Datum pg_is_other_temp_schema(PG_FUNCTION_ARGS);



/*
 * RangeVarGetRelid
 *		Given a RangeVar describing an existing relation,
 *		select the proper namespace and look up the relation OID.
 *
 * If the relation is not found, return InvalidOid if missing_ok = true,
 * otherwise raise an error.
 *
 * If nowait = true, throw an error if we'd have to wait for a lock.
 *
 * If isSupportSynonym = true, means that we regard it as a synonym to search
 * for its referenced object name when relation not found.
 *
 * Callback allows caller to check permissions or acquire additional locks
 * prior to grabbing the relation lock.
 *
 * target_is_partition: the operation assigned by caller of RangeVarGetRelidExtended(),
 *                             will be pushed on relation level or partition level?
 *
 * detailInfo: more detail infomation during synonym mapping and checking.
 *
 * refSynOid: store the referenced synonym Oid if mapping successfully.
 */


/*
 * RangeVarGetCreationNamespace
 *		Given a RangeVar describing a to-be-created relation,
 *		choose which namespace to create it in.
 *
 * Note: calling this may result in a CommandCounterIncrement operation.
 * That will happen on the first request for a temp table in any particular
 * backend run; we will need to either create or clean out the temp schema.
 */




/*
 * checking whether the user has create any permission
 */




/*
 * RangeVarGetAndCheckCreationNamespace
 *
 * This function returns the OID of the namespace in which a new relation
 * with a given name should be created.  If the user does not have CREATE
 * permission on the target namespace, this function will instead signal
 * an ERROR.
 *
 * If non-NULL, *existing_oid is set to the OID of any existing relation with
 * the same name which already exists in that namespace, or to InvalidOid if
 * no such relation exists.
 *
 * If lockmode != NoLock, the specified lock mode is acquire on the existing
 * relation, if any, provided that the current user owns the target relation.
 * However, if lockmode != NoLock and the user does not own the target
 * relation, we throw an ERROR, as we must not try to lock relations the
 * user does not have permissions on.
 *
 * As a side effect, this function acquires AccessShareLock on the target
 * namespace.  Without this, the namespace could be dropped before our
 * transaction commits, leaving behind relations with relnamespace pointing
 * to a no-longer-exstant namespace.
 *
 * As a further side-effect, if the select namespace is a temporary namespace,
 * we mark the RangeVar as RELPERSISTENCE_TEMP.
 */


/*
 * Adjust the relpersistence for an about-to-be-created relation based on the
 * creation namespace, and throw an error for invalid combinations.
 */



/*
 * RelnameGetRelid
 *		Try to resolve an unqualified relation name.
 *		Returns OID if relation found in search path, else InvalidOid.
 */


/*
 * RelnameGetRelidExtended
 *      Try to resolve an unqualified relation name.
 *      If not found, to search for whether it is a synonym object.
 *      Store OID if relation found in search path, else InvalidOid.
 *      Returns the details info of supported cases checking.
 */


/*
 * RelationIsVisible
 *		Determine whether a relation (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified relation name".
 */


/*
 * TypenameGetTypid
 *     Wrapper for binary compatibility.
 */




/*
 * TypenameGetTypidExtended
 *		Try to resolve an unqualified datatype name.
 *		Returns OID if type found in search path, else InvalidOid.
 *
 * This is essentially the same as RelnameGetRelid.
 */


/*
 * TypeIsVisible
 *		Determine whether a type (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified type name".
 */








/*
 * some procedure args have tableof variable,
 * when match the proc parameters' type,
 * we should change to its base type.
 */


#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * FuncnameGetCandidates
 *		Given a possibly-qualified function name and argument count,
 *		retrieve a list of the possible matches.
 *
 * If nargs is -1, we return all functions matching the given name,
 * regardless of argument count.  (argnames must be NIL, and expand_variadic
 * and expand_defaults must be false, in this case.)
 *
 * If argnames isn't NIL, we are considering a named- or mixed-notation call,
 * and only functions having all the listed argument names will be returned.
 * (We assume that length(argnames) <= nargs and all the passed-in names are
 * distinct.)  The returned structs will include an argnumbers array showing
 * the actual argument index for each logical argument position.
 *
 * If expand_variadic is true, then variadic functions having the same number
 * or fewer arguments will be retrieved, with the variadic argument and any
 * additional argument positions filled with the variadic element type.
 * nvargs in the returned struct is set to the number of such arguments.
 * If expand_variadic is false, variadic arguments are not treated specially,
 * and the returned nvargs will always be zero.
 *
 * If expand_defaults is true, functions that could match after insertion of
 * default argument values will also be retrieved.	In this case the returned
 * structs could have nargs > passed-in nargs, and ndargs is set to the number
 * of additional args (which can be retrieved from the function's
 * proargdefaults entry).
 *
 * It is not possible for nvargs and ndargs to both be nonzero in the same
 * list entry, since default insertion allows matches to functions with more
 * than nargs arguments while the variadic transformation requires the same
 * number or less.
 *
 * When argnames isn't NIL, the returned args[] type arrays are not ordered
 * according to the functions' declarations, but rather according to the call:
 * first any positional arguments, then the named arguments, then defaulted
 * arguments (if needed and allowed by expand_defaults).  The argnumbers[]
 * array can be used to map this back to the catalog information.
 * argnumbers[k] is set to the proargtypes index of the k'th call argument.
 *
 * We search a single namespace if the function name is qualified, else
 * all namespaces in the search path.  In the multiple-namespace case,
 * we arrange for entries in earlier namespaces to mask identical entries in
 * later namespaces.
 *
 * When expanding variadics, we arrange for non-variadic functions to mask
 * variadic ones if the expanded argument list is the same.  It is still
 * possible for there to be conflicts between different variadic functions,
 * however.
 *
 * It is guaranteed that the return list will never contain multiple entries
 * with identical argument lists.  When expand_defaults is true, the entries
 * could have more than nargs positions, but we still guarantee that they are
 * distinct in the first nargs positions.  However, if argnames isn't NIL or
 * either expand_variadic or expand_defaults is true, there might be multiple
 * candidate functions that expand to identical argument lists.  Rather than
 * throw error here, we report such situations by returning a single entry
 * with oid = 0 that represents a set of such conflicting candidates.
 * The caller might end up discarding such an entry anyway, but if it selects
 * such an entry it should react as though the call were ambiguous.
 */
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif




/*
 * MatchNamedCall
 * 		Given a pg_proc heap tuple and a call's list of argument names,
 * 		check whether the function could match the call.
 *
 * The call could match if all supplied argument names are accepted by
 * the function, in positions after the last positional argument, and there
 * are defaults for all unsupplied arguments.
 *
 * The number of positional arguments is nargs - list_length(argnames).
 * Note caller has already done basic checks on argument count.
 *
 * On match, return true and fill *argnumbers with a palloc'd array showing
 * the mapping from call argument positions to actual function argument
 * numbers.  Defaulted arguments are included in this map, at positions
 * after the last supplied argument.
 */


/*
 * FunctionIsVisible
 *		Determine whether a function (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified function name with exact argument matches".
 */
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * OpernameGetOprid
 *		Given a possibly-qualified operator name and exact input datatypes,
 *		look up the operator.  Returns InvalidOid if not found.
 *
 * Pass oprleft = InvalidOid for a prefix op, oprright = InvalidOid for
 * a postfix op.
 *
 * If the operator name is not schema-qualified, it is sought in the current
 * namespace search path.
 */


/*
 * OpernameGetCandidates
 *		Given a possibly-qualified operator name and operator kind,
 *		retrieve a list of the possible matches.
 *
 * If oprkind is '\0', we return all operators matching the given name,
 * regardless of arguments.
 *
 * We search a single namespace if the operator name is qualified, else
 * all namespaces in the search path.  The return list will never contain
 * multiple entries with identical argument lists --- in the multiple-
 * namespace case, we arrange for entries in earlier namespaces to mask
 * identical entries in later namespaces.
 *
 * The returned items always have two args[] entries --- one or the other
 * will be InvalidOid for a prefix or postfix oprkind.	nargs is 2, too.
 */
#define SPACE_PER_OP MAXALIGN(sizeof(struct _FuncCandidateList) + sizeof(Oid))

/*
 * OperatorIsVisible
 *		Determine whether an operator (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified operator name with exact argument matches".
 */


/*
 * OpclassnameGetOpcid
 *		Try to resolve an unqualified index opclass name.
 *		Returns OID if opclass found in search path, else InvalidOid.
 *
 * This is essentially the same as TypenameGetTypid, but we have to have
 * an extra argument for the index AM OID.
 */


/*
 * OpclassIsVisible
 *		Determine whether an opclass (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified opclass name".
 */


/*
 * OpfamilynameGetOpfid
 *		Try to resolve an unqualified index opfamily name.
 *		Returns OID if opfamily found in search path, else InvalidOid.
 *
 * This is essentially the same as TypenameGetTypid, but we have to have
 * an extra argument for the index AM OID.
 */


/*
 * OpfamilyIsVisible
 *		Determine whether an opfamily (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified opfamily name".
 */


/*
 * CollationGetCollid
 *		Try to resolve an unqualified collation name.
 *		Returns OID if collation found in search path, else InvalidOid.
 */


/*
 * CollationIsVisible
 *		Determine whether a collation (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified collation name".
 */


/*
 * ConversionGetConid
 *		Try to resolve an unqualified conversion name.
 *		Returns OID if conversion found in search path, else InvalidOid.
 *
 * This is essentially the same as RelnameGetRelid.
 */


/*
 * ConversionIsVisible
 *		Determine whether a conversion (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified conversion name".
 */


/*
 * get_ts_parser_oid - find a TS parser by possibly qualified name
 *
 * If not found, returns InvalidOid if missing_ok, else throws error
 */


/*
 * TSParserIsVisible
 *		Determine whether a parser (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified parser name".
 */


/*
 * get_ts_dict_oid - find a TS dictionary by possibly qualified name
 *
 * If not found, returns InvalidOid if failOK, else throws error
 */


/*
 * TSDictionaryIsVisible
 *		Determine whether a dictionary (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified dictionary name".
 */


/*
 * get_ts_template_oid - find a TS template by possibly qualified name
 *
 * If not found, returns InvalidOid if missing_ok, else throws error
 */


/*
 * TSTemplateIsVisible
 *		Determine whether a template (identified by OID) is visible in the
 *		current search path.  Visible means "would be found by searching
 *		for the unqualified template name".
 */


/*
 * get_ts_config_oid - find a TS config by possibly qualified name
 *
 * If not found, returns InvalidOid if missing_ok, else throws error
 */


/*
 * TSConfigIsVisible
 *		Determine whether a text search configuration (identified by OID)
 *		is visible in the current search path.	Visible means "would be found
 *		by searching for the unqualified text search configuration name".
 */


/*
 * DeconstructQualifiedName
 *		Given a possibly-qualified name expressed as a list of String nodes,
 *		extract the schema name and object name.
 *
 * *nspname_p is set to NULL if there is no explicit schema name.
 */

void DeconstructQualifiedName(const List* names, char** nspname_p, char** objname_p, char **pkgname_p, char** typname_p)
{
    char* catalogname = NULL;
    char* schemaname = NULL;
    char* objname = NULL;
    char* pkgname = NULL;
    Oid nspoid = InvalidOid;
    char* typname = NULL;
    switch (list_length(names)) {
        case 1:
            objname = strVal(linitial(names));
            break;
        case 2:
            {
                objname = strVal(lsecond(names));
                schemaname = strVal(linitial(names));
                if (nspname_p != NULL) {
                    nspoid = get_namespace_oid(schemaname, true);
                }   
                pkgname = strVal(linitial(names));
                if (!OidIsValid(PackageNameGetOid(pkgname, nspoid))) {
                    pkgname = strVal(lsecond(names));
                    if (OidIsValid(PackageNameGetOid(pkgname, nspoid))) {
                        schemaname = strVal(linitial(names));
                        objname = pkgname;
                        pkgname = NULL;
                    } else {
                        pkgname = NULL;
                    }
                } else {
                    schemaname = NULL;
                }
                /* Deconstruct typname */
                if (pkgname == NULL) {
                    typname = strVal(linitial(names));
                    Type typtup = LookupTypeName(NULL, makeTypeNameFromNameList(list_make1(makeString(typname))), NULL);
                    if (HeapTupleIsValid(typtup)) {
                        Form_pg_type typ_struct = (Form_pg_type)GETSTRUCT(typtup);
                        if (typ_struct->typtype == TYPTYPE_ABSTRACT_OBJECT) {
                            objname = strVal(lsecond(names));
                            schemaname = NULL;
                        } else {
                            typname = NULL;
                        }
                        ReleaseSysCache(typtup);
                    } else {
                        pkgname = NULL;
                        typname = NULL;
                    }
                }
            }
            break;
        case 3:
            {
                /* schemaname.pkgname.objname */
                objname = strVal(lthird(names));
                pkgname = strVal(lsecond(names));
                schemaname = strVal(linitial(names));
                if (nspname_p != NULL) {
                    nspoid = get_namespace_oid(schemaname, true);
                }
                typname = strVal(lsecond(names));
                if (nspoid != InvalidOid &&
                    SearchSysCacheExists2(TYPENAMENSP, PointerGetDatum(typname), ObjectIdGetDatum(nspoid))) {
                    /* schemaname.typname.objname */
                    objname = strVal(lthird(names));
                    schemaname = strVal(linitial(names));
                } else {
                    typname = NULL;
                }
                /* catalogname.schemaname.objname */
                if (typname == NULL && !OidIsValid(PackageNameGetOid(pkgname, nspoid))) {
                    catalogname = strVal(linitial(names));
                    schemaname = strVal(lsecond(names));
                    pkgname = NULL;
                    break;
                }
            }
            break;
        case 4:
            catalogname = strVal(linitial(names));
            schemaname = strVal(lsecond(names));
            pkgname = strVal(lthird(names));
            objname = strVal(lfourth(names));
            /*
             * We check the catalog name and then ignore it.
             */
            if (strcmp(catalogname, get_and_check_db_name(u_sess->proc_cxt.MyDatabaseId, true)) != 0)
                ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                        errmsg("cross-database references are not implemented: %s", NameListToString(names))));
            break;
        default:
            ereport(ERROR,
                (errcode(ERRCODE_SYNTAX_ERROR),
                    errmsg("improper qualified name (too many dotted names): %s", NameListToString(names))));
            break;
    }

    *nspname_p = schemaname;
    *objname_p = objname;
    if (pkgname_p != NULL)
        *pkgname_p = pkgname;
    if (typname_p != NULL)
        *typname_p = typname;
}

/*
 * @Description: check function is package function,if the function in package,still return true
 * @in funcname -function name
 * @return - function is package
 */
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * LookupNamespaceNoError
 *		Look up a schema name.
 *
 * Returns the namespace OID, or InvalidOid if not found.
 *
 * Note this does NOT perform any permissions check --- callers are
 * responsible for being sure that an appropriate check is made.
 * In the majority of cases LookupExplicitNamespace is preferable.
 */


/*
 * LookupExplicitNamespace
 *		Process an explicitly-specified schema name: look up the schema
 *		and verify we have USAGE (lookup) rights in it.
 *
 * Returns the namespace OID.  Raises ereport if any problem.
 */


/*
 * LookupCreationNamespace
 *		Look up the schema and verify we have CREATE rights on it.
 *
 * This is just like LookupExplicitNamespace except for the different
 * permission check, and that we are willing to create pg_temp if needed.
 *
 * Note: calling this may result in a CommandCounterIncrement operation,
 * if we have to create or clean out the temp namespace.
 */


/*
 * Common checks on switching namespaces.
 *
 * We complain if (1) the old and new namespaces are the same, (2) either the
 * old or new namespaces is a temporary schema (or temporary toast schema), or
 * (3) either the old or new namespaces is the TOAST schema.
 */


/*
 * QualifiedNameGetCreationNamespace
 *		Given a possibly-qualified name for an object (in List-of-Values
 *		format), determine what namespace the object should be created in.
 *		Also extract and return the object name (last component of list).
 *
 * Note: this does not apply any permissions check.  Callers must check
 * for CREATE rights on the selected namespace when appropriate.
 *
 * Note: calling this may result in a CommandCounterIncrement operation,
 * if we have to create or clean out the temp namespace.
 */





/*
 * get_namespace_oid - given a namespace name, look up the OID
 *
 * If missing_ok is false, throw an error if namespace name not found.	If
 * true, just return InvalidOid.
 */
Oid get_namespace_oid(const char* nspname, bool missing_ok)
{
    Oid oid = InvalidOid;

    oid = GetSysCacheOid1(NAMESPACENAME, CStringGetDatum(nspname));
    if (!OidIsValid(oid) && !missing_ok) {     
        if (enable_plpgsql_undefined_not_check_nspoid()) {
            ereport(LOG, (errmodule(MOD_PLSQL),
             (ERRCODE_UNDEFINED_SCHEMA), errmsg("%s does not exist. Ignore it.", nspname)));
             return oid;
        }
        char message[MAXSTRLEN]; 
        errno_t rc = sprintf_s(message, MAXSTRLEN, "schema \"%s\" does not exist", nspname);
        if (strlen(nspname) > MAXSTRLEN) {
            ereport(ERROR, (errcode(ERRCODE_UNDEFINED_SCHEMA), errmsg("The schema name exceeds the maximum length.")));
        }
        securec_check_ss(rc, "", "");
        InsertErrorMessage(message, u_sess->plsql_cxt.plpgsql_yylloc, true);
        ereport(ERROR, (errcode(ERRCODE_UNDEFINED_SCHEMA), errmsg("schema \"%s\" does not exist", nspname)));
    }

    return oid;
}


/*
 * makeRangeVarFromNameList
 *		Utility routine to convert a qualified-name list into RangeVar form.
 */


/*
 * NameListToString
 *		Utility routine to convert a qualified-name list into a string.
 *
 * This is used primarily to form error messages, and so we do not quote
 * the list elements, for the sake of legibility.
 *
 * In most scenarios the list elements should always be Value strings,
 * but we also allow A_Star for the convenience of ColumnRef processing.
 */
char* NameListToString(const List* names)
{
    StringInfoData string;
    ListCell* l = NULL;

    initStringInfo(&string);

    foreach (l, names) {
        Node* name = (Node*)lfirst(l);

        if (l != list_head(names))
            appendStringInfoChar(&string, '.');

        if (IsA(name, String))
            appendStringInfoString(&string, strVal(name));
        else if (IsA(name, A_Star))
            appendStringInfoString(&string, "*");
        else
            ereport(ERROR,
                (errcode(ERRCODE_UNEXPECTED_NODE_STATE),
                    errmsg("unexpected node type in name list: %d", (int)nodeTag(name))));
    }

    return string.data;
}

/*
 * NameListToQuotedString
 *		Utility routine to convert a qualified-name list into a string.
 *
 * Same as above except that names will be double-quoted where necessary,
 * so the string could be re-parsed (eg, by textToQualifiedNameList).
 */


/*
 * isTempNamespace - is the given namespace my temporary-table namespace?
 */


/*
 * isTempToastNamespace - is the given namespace my temporary-toast-table
 *		namespace?
 */


/*
 * isTempOrToastNamespace - is the given namespace my temporary-table
 *		namespace or my temporary-toast-table namespace?
 */


/*
 * isAnyTempNamespace - is the given namespace a temporary-table namespace
 * (either my own, or another backend's)?  Temporary-toast-table namespaces
 * are included, too.
 */


/*
 * isOtherTempNamespace - is the given namespace some other backend's
 * temporary-table namespace (including temporary-toast-table namespaces)?
 *
 * Note: for most purposes in the C code, this function is obsolete.  Use
 * RELATION_IS_OTHER_TEMP() instead to detect non-local temp relations.
 */


/*
 * GetTempToastNamespace - get the OID of my temporary-toast-table namespace,
 * which must already be assigned.	(This is only used when creating a toast
 * table for a temp table, so we must have already done InitTempTableNamespace)
 */




/*
 * GetOverrideSearchPath - fetch current search path definition in form
 * used by PushOverrideSearchPath.
 *
 * The result structure is allocated in the specified memory context
 * (which might or might not be equal to CurrentMemoryContext); but any
 * junk created by revalidation calculations will be in CurrentMemoryContext.
 */


/*
 * CopyOverrideSearchPath - copy the specified OverrideSearchPath.
 *
 * The result structure is allocated in CurrentMemoryContext.
 */


/*
 * OverrideSearchPathMatchesCurrent - does path match current setting?
 */


/*
 * PushOverrideSearchPath - temporarily override the search path
 *
 * We allow nested overrides, hence the push/pop terminology.  The GUC
 * search_path variable is ignored while an override is active.
 *
 * It's possible that newpath->useTemp is set but there is no longer any
 * active temp namespace, if the path was saved during a transaction that
 * created a temp namespace and was later rolled back.	In that case we just
 * ignore useTemp.	A plausible alternative would be to create a new temp
 * namespace, but for existing callers that's not necessary because an empty
 * temp namespace wouldn't affect their results anyway.
 *
 * It's also worth noting that other schemas listed in newpath might not
 * exist anymore either.  We don't worry about this because OIDs that match
 * no existing namespace will simply not produce any hits during searches.
 */


/*
 * PopOverrideSearchPath - undo a previous PushOverrideSearchPath
 *
 * Any push during a (sub)transaction will be popped automatically at abort.
 * But it's caller error if a push isn't popped in normal control flow.
 */


/*
 * Description: Add temp namespace ID to OverrideSearchPath.
 *
 * Parameters:
 * @in tmpnspId: temp namespace ID
 *
 * Returns: void
 */



/*
 * Description: Remove temp namespace ID from OverrideSearchPath and activeSearchPath.
 *
 * Parameters:
 * @in tmpnspId: temp namespace ID
 *
 * Returns: void
 */



/* If the collate string is in uppercase, change to lowercase and search it again */


/*
 * get_collation_oid - find a collation by possibly qualified name
 */
Oid get_collation_oid(List *name, bool missing_ok) { return -1; }


/*
 * get_conversion_oid - find a conversion by possibly qualified name
 */


/*
 * FindDefaultConversionProc - find default encoding conversion proc
 */
Oid FindDefaultConversionProc(int4 for_encoding, int4 to_encoding)
{
    Oid proc;
    ListCell* l = NULL;
    List* tempActiveSearchPath = NIL;

    recomputeNamespacePath();

    tempActiveSearchPath = list_copy(u_sess->catalog_cxt.activeSearchPath);
    foreach (l, tempActiveSearchPath) {
        Oid namespaceId = lfirst_oid(l);

        if (namespaceId == u_sess->catalog_cxt.myTempNamespace)
            continue; /* do not look in temp namespace */

        proc = FindDefaultConversion(namespaceId, for_encoding, to_encoding);
        if (OidIsValid(proc)) {
            list_free_ext(tempActiveSearchPath);
            return proc;
        }
    }

    list_free_ext(tempActiveSearchPath);

    /* Not found in path */
    return InvalidOid;
}

static void recomputeOverrideStackTempPath()
{
    ListCell *lc = NULL, *cell = NULL;
    OverrideStackEntry* entry = NULL;
    foreach (lc, u_sess->catalog_cxt.overrideStack) {
        entry = (OverrideStackEntry*)lfirst(lc);
        if (OidIsValid(u_sess->catalog_cxt.myTempNamespaceOld) && entry->searchPath) {
            foreach (cell, entry->searchPath) {
                if (lfirst_oid(cell) == u_sess->catalog_cxt.myTempNamespaceOld) {
                    lfirst_oid(cell) = u_sess->catalog_cxt.myTempNamespace;
                }
            }
        }
    }
}

/*
 * recomputeNamespacePath - recompute path derived variables if needed.
 */
void recomputeNamespacePath(StringInfo error_info)
{
    Oid roleid = GetUserId();
    char* rawname = NULL;
    List* namelist = NIL;
    List* oidlist = NIL;
    List* newpath = NIL;
    ListCell* l = NULL;
    bool temp_missing = false;
    Oid firstNS = InvalidOid;
    Oid firstOid = InvalidOid;
    MemoryContext oldcxt;

    ereport(DEBUG2, (errmodule(MOD_SCHEMA), errmsg("Recomputing namespacePath with namespace_search_path:%s", 
                        u_sess->attr.attr_common.namespace_search_path)));

    /* if sql advisor online model is running, we can't recompute search path. */
    if (u_sess->adv_cxt.isJumpRecompute) {
        ereport(DEBUG2, (errmodule(MOD_SCHEMA), 
                errmsg("recomputeNamespacePath is skipped because isJumpRecompute is on.")));
        return;
    }

    /* Do nothing if an override search spec is active. */
    if (u_sess->catalog_cxt.overrideStack && u_sess->catalog_cxt.overrideStackValid) {
        ereport(DEBUG2, (errmodule(MOD_SCHEMA), 
                errmsg("recomputeNamespacePath is skipped because overrideStack is valid.")));
        return;
    }

    /* Do nothing if path is already valid. */
    if (u_sess->catalog_cxt.baseSearchPathValid && u_sess->catalog_cxt.namespaceUser == roleid) {
        if (module_logging_is_on(MOD_SCHEMA)) {
            char* str = nodeToString(u_sess->catalog_cxt.baseSearchPath);
            ereport(DEBUG2, (errmodule(MOD_SCHEMA), 
                    errmsg("recomputeNamespacePath is skipped because baseSearchPath(%s) is valid.", str)));
            pfree(str);
        }
        return;
    }

    /* Need a modifiable copy of u_sess->attr.attr_common.namespace_search_path string */
    rawname = pstrdup(u_sess->attr.attr_common.namespace_search_path);

    /* Parse string into list of identifiers */
    if (!SplitIdentifierString(rawname, ',', &namelist)) {
        /* syntax error in name list */
        /* this should not happen if GUC checked check_search_path */
        ereport(ERROR, (errcode(ERRCODE_UNEXPECTED_NODE_STATE), errmsg("invalid list syntax")));
    }

    /*
     * Convert the list of names to a list of OIDs.  If any names are not
     * recognizable or we don't have read access, just leave them out of the
     * list.  (We can't raise an error, since the search_path setting has
     * already been accepted.)	Don't make duplicate entries, either.
     */
    oidlist = NIL;
    temp_missing = false;
    foreach (l, namelist) {
        char* curname = (char*)lfirst(l);
        Oid namespaceId;

        if (strcmp(curname, "$user") == 0) {
            /* $user --- substitute namespace matching user name, if any */
            HeapTuple tuple;

            tuple = SearchSysCache1(AUTHOID, ObjectIdGetDatum(roleid));
            if (HeapTupleIsValid(tuple)) {
                char* rname = NULL;

                rname = NameStr(((Form_pg_authid)GETSTRUCT(tuple))->rolname);
                namespaceId = get_namespace_oid(rname, true);
                ReleaseSysCache(tuple);
                if (OidIsValid(namespaceId) && !list_member_oid(oidlist, namespaceId) &&
                    pg_namespace_aclcheck(namespaceId, roleid, ACL_USAGE, false) == ACLCHECK_OK) {
                    oidlist = lappend_oid(oidlist, namespaceId);
                    if (firstOid == InvalidOid)
                        firstOid = namespaceId;
                }
            }
        } else if (strcmp(curname, "pg_temp") == 0) {
            /* pg_temp --- substitute temp namespace, if any */
            if (OidIsValid(u_sess->catalog_cxt.myTempNamespace)) {
                if (firstOid == InvalidOid)
                    firstOid = u_sess->catalog_cxt.myTempNamespace;
            } else {
                /* If it ought to be the creation namespace, set flag */
                if (oidlist == NIL)
                    temp_missing = true;
            }
        } else {
            /* normal namespace reference */
            namespaceId = get_namespace_oid(curname, true);
            if (!OidIsValid(namespaceId)) {
                if (error_info != NULL) {
                    if (error_info->len == 0)
                        appendStringInfo(error_info, "schema \"%s\" does not exist", curname);
                    else
                        appendStringInfo(error_info, "; schema \"%s\" does not exist", curname);
                }
            } else if (pg_namespace_aclcheck(namespaceId, roleid, ACL_USAGE) != ACLCHECK_OK) {
                if (error_info != NULL) {
                    if (error_info->len == 0)
                        appendStringInfo(error_info, "permission denied for schema %s", curname);
                    else
                        appendStringInfo(error_info, "; permission denied for schema %s", curname);
                }
            } else if (!list_member_oid(oidlist, namespaceId)) {
                if (namespaceId != PG_CATALOG_NAMESPACE)
                    oidlist = lappend_oid(oidlist, namespaceId);
                if (firstOid == InvalidOid)
                    firstOid = namespaceId;
            }
        }
    }

    /*
     * Remember the first member of the explicit list.	(Note: this is
     * nominally wrong if temp_missing, but we need it anyway to distinguish
     * explicit from implicit mention of pg_catalog.)
     */
    if (firstOid != InvalidOid)
        firstNS = firstOid;

    /*
     * Add any implicitly-searched namespaces to the list.	Note these go on
     * the front, not the back; also notice that we do not check USAGE
     * permissions for these.
     * Note: Change the behavior: Always put PG_CATALOG_NAMESPACE and
     * myTempNamespace in front of oidlist for compatilibility with
     * stored procedures.
     */
    oidlist = lcons_oid(PG_CATALOG_NAMESPACE, oidlist);

    if (DB_IS_CMPT(D_FORMAT)) {
        Oid sys_oid = get_namespace_oid(SYS_NAMESPACE_NAME, true);
        if (OidIsValid(sys_oid) && !list_member_oid(oidlist, sys_oid)) {
            oidlist = lcons_oid(sys_oid, oidlist);
        }
    }

    if (OidIsValid(u_sess->catalog_cxt.myTempNamespace))
        oidlist = lcons_oid(u_sess->catalog_cxt.myTempNamespace, oidlist);

    /*
     * Now that we've successfully built the new list of namespace OIDs, save
     * it in permanent storage.
     */
    oldcxt = MemoryContextSwitchTo(u_sess->cache_mem_cxt);
    newpath = list_copy(oidlist);
    MemoryContextSwitchTo(oldcxt);

    /* Now safe to assign to state variables. */
    list_free_ext(u_sess->catalog_cxt.baseSearchPath);
    u_sess->catalog_cxt.baseSearchPath = newpath;
    u_sess->catalog_cxt.baseCreationNamespace = firstNS;
    u_sess->catalog_cxt.baseTempCreationPending = temp_missing;

    /* Mark the path valid. */
    u_sess->catalog_cxt.baseSearchPathValid = true;
    u_sess->catalog_cxt.namespaceUser = roleid;

    /* And make it active. */
    u_sess->catalog_cxt.activeSearchPath = u_sess->catalog_cxt.baseSearchPath;
    u_sess->catalog_cxt.activeCreationNamespace = u_sess->catalog_cxt.baseCreationNamespace;
    u_sess->catalog_cxt.activeTempCreationPending = u_sess->catalog_cxt.baseTempCreationPending;

    if (u_sess->catalog_cxt.overrideStack) {
        recomputeOverrideStackTempPath();
        u_sess->catalog_cxt.myTempNamespaceOld = InvalidOid;
        u_sess->catalog_cxt.overrideStackValid = true;
    }
    /* Clean up. */
    pfree_ext(rawname);
    list_free_ext(namelist);
    list_free_ext(oidlist);
}

/*
 * InitTempTableNamespace
 *		Initialize temp table namespace on first use in a particular backend
 */
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * End-of-transaction cleanup for namespaces.
 */


/*
 * AtEOSubXact_Namespace
 *
 * At subtransaction commit, propagate the temp-namespace-creation
 * flag to the parent subtransaction.
 *
 * At subtransaction abort, forget the flag if we set it up.
 */

/*
 * Remove all relations in the specified temp namespace.
 *
 * This is called at backend shutdown (if we made any temp relations).
 * It is also called when we begin using a pre-existing temp namespace,
 * in order to clean out any relations that might have been created by
 * a crashed backend.
 */


/*
 * Remove all temp tables from the temporary namespace.
 */


/*
 * Routines for handling the GUC variable 'search_path'.
 */

/* check_hook: validate new search_path value */


/* assign_hook: do extra actions as needed */


/*
 * InitializeSearchPath: initialize module during InitPostgres.
 *
 * This is called after we are up enough to be able to do catalog lookups.
 */


/*
 * NamespaceCallback
 *		Syscache inval callback function
 */


/*
 * Fetch the active search path. The return value is a palloc'ed list
 * of OIDs; the caller is responsible for freeing this storage as
 * appropriate.
 *
 * The returned list includes the implicitly-prepended namespaces only if
 * includeImplicit is true.
 *
 * Note: calling this may result in a CommandCounterIncrement operation,
 * if we have to create or clean out the temp namespace.
 */


/*
 * Fetch the active search path into a caller-allocated array of OIDs.
 * Returns the number of path entries.	(If this is more than sarray_len,
 * then the data didn't fit and is not all stored.)
 *
 * The returned list always includes the implicitly-prepended namespaces,
 * but never includes the temp namespace.  (This is suitable for existing
 * users, which would want to ignore the temp namespace anyway.)  This
 * definition allows us to not worry about initializing the temp namespace.
 */




/*
 * Export the FooIsVisible functions as SQL-callable functions.
 *
 * Note: as of Postgres 8.4, these will silently return NULL if called on
 * a nonexistent object OID, rather than failing.  This is to avoid race
 * condition errors when a query that's scanning a catalog using an MVCC
 * snapshot uses one of these functions.  The underlying IsVisible functions
 * operate on SnapshotNow semantics and so might see the object as already
 * gone when it's still visible to the MVCC snapshot.  (There is no race
 * condition in the current coding because we don't accept sinval messages
 * between the SearchSysCacheExists test and the subsequent lookup.)
 */

#ifdef ENABLE_MULTIPLE_NODES
#endif





























/*
 * @Description: get the namespace's owner that has the same name as the namespace.
 * @in nspid : namespace oid.
 * @anyPriv : anyPriv is true, explain that the user has create any permission
 * @in is_securityadmin : whether the is a security administrator doing this.
 * @return : return InvalidOid if there is no appropriate role.
 *            return the owner's oid if the namespace has the same name as its owner.
 */


Oid getCurrentNamespace()
{
    return u_sess->catalog_cxt.activeCreationNamespace;
}

/*
 * Called by CreateSchemaCommand when creating a temporary schema.
 * Could run on DN or CN.
 */




/*
 * Called by set_config_option when setting search_path/current_schema
 * Setting myTempNamespace, myTempToastNamespace, baseSearchPathValid
 * if namelist contains temp namespace.
 * Only use first valid temp namespace.
 */


/*
 * @Description: temp table does not do redistribute when cluster resizing, so it will
 * still on old node group after redistributing. when the old node group is dropped, the
 * temp table must turn invalid. This function is for check if the node group is valid.
 * @in relid - oid of tmep table to be checked.
 * @in missing_ok - if it's invalid, should we raise an error?
 * @return: bool - If the node group where the temp table in is valid
 */


#ifdef ENABLE_UT
void
#else
#endif

/*
 * @Description: when datanode has restarted, the temp relaton turns invalid because
 * there is no WAL protection. So when we use a temp table's data, we should first
 * check pg_namespace if the temp namespace turn invalid because of restart.
 * @in tmepNspId - oid of tmep namespace which the temp relation used is in.
 * @return: bool - If the temp namespace is valid.
 */


/*
 * @Description: when datanode has restarted, the temp relaton turns invalid because
 * there is no WAL protection. So when we use a temp table's data, we should first
 * check pg_namespace if the temp namespace turn invalid because of restart.  Is so,
 * report an error.
 * @in rel - the relation to be checked.
 * @return: void
 */


/*
 * If it is in the path, it might still not be visible; it could be
 * hidden by another parser/template/dictionary/configuration of the
 * same name earlier in the path. So we must do a slow check for
 * conflicting configurations.
 */


/* Initialize temp namespace if first time through */








// end of file
