/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - current_schema
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * name.c
 *	  Functions for the built-in type "name".
 *
 * name replaces char16 and is carefully implemented so that it
 * is a string of physical length NAMEDATALEN.
 * DO NOT use hard-coded constants anywhere
 * always use NAMEDATALEN as the symbolic constant!   - jolly 8/21/95
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/name.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "catalog/namespace.h"
#include "catalog/pg_type.h"
#include "libpq/pqformat.h"
#include "plugin_mb/pg_wchar.h"
#include "miscadmin.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "plugin_postgres.h"

/*****************************************************************************
 *	 USER I/O ROUTINES (none)												 *
 *****************************************************************************/

/*
 *		namein	- converts "..." to internal representation
 *
 *		Note:
 *				[Old] Currently if strlen(s) < NAMEDATALEN, the extra chars are nulls
 *				Now, always NULL terminated
 */


/*
 *		nameout - converts internal representation to "..."
 */


/*
 *		namerecv			- converts external binary format to name
 */


/*
 *		namesend			- converts name to binary format
 */


/*****************************************************************************
 *	 PUBLIC ROUTINES														 *
 *****************************************************************************/

/*
 *		nameeq	- returns 1 iff arguments are equal
 *		namene	- returns 1 iff arguments are not equal
 *
 *		BUGS:
 *				Assumes that "xy\0\0a" should be equal to "xy\0b".
 *				If not, can do the comparison backwards for efficiency.
 *
 *		namelt	- returns 1 iff a < b
 *		namele	- returns 1 iff a <= b
 *		namegt	- returns 1 iff a > b
 *		namege	- returns 1 iff a >= b
 *
 */














/* (see char.c for comparison/operation routines) */

#ifdef NOT_USED
int namecmp(Name n1, Name n2)
{
    return strncmp(NameStr(*n1), NameStr(*n2), NAMEDATALEN);
}
#endif







/*
 * SQL-functions CURRENT_USER, SESSION_USER
 */






/*
 * SQL-functions CURRENT_SCHEMA, CURRENT_SCHEMAS
 */
Datum current_schema(FunctionCallInfo fcinfo) {return 0;}




;
extern "C" DLL_PUBLIC Datum get_b_database(PG_FUNCTION_ARGS);


#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum get_b_schema(PG_FUNCTION_ARGS);

#endif

#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum get_b_session_user(PG_FUNCTION_ARGS);

#endif