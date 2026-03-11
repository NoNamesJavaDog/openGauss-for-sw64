/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - EnablePortalManager
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * portalmem.c
 *	  backend portal memory management
 *
 * Portals are objects representing the execution state of a query.
 * This module provides memory management services for portals, but it
 * doesn't actually run the executor for them.
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 * IDENTIFICATION
 *	  src/backend/utils/mmgr/portalmem.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/xact.h"
#include "catalog/pg_type.h"
#include "commands/portalcmds.h"
#include "distributelayer/streamCore.h"
#include "distributelayer/streamMain.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/memutils.h"
#include "utils/plpgsql.h"
#include "utils/timestamp.h"
#include "utils/resowner.h"
#include "utils/palloc.h"
#include "nodes/execnodes.h"
#include "opfusion/opfusion.h"

#ifdef PGXC
#include "pgxc/pgxc.h"
#include "access/hash.h"
#include "catalog/pg_collation.h"
#include "utils/formatting.h"
#include "utils/lsyscache.h"
#endif


extern void ReleaseSharedCachedPlan(CachedPlan* plan, bool useResOwner);
static void CursorRecordTypeUnbind(const char* portal_name);
/*
 * Estimate of the maximum number of open portals a user would have,
 * used in initially sizing the PortalHashTable in EnablePortalManager().
 * Since the hash table can expand, there's no need to make this overly
 * generous, and keeping it small avoids unnecessary overhead in the
 * hash_seq_search() calls executed during transaction end.
 */
#define PORTALS_PER_USER 16

/* ----------------
 *		Global state
 * ----------------
 */

#define MAX_PORTALNAME_LEN NAMEDATALEN

typedef struct portalhashent {
    char portalname[MAX_PORTALNAME_LEN];
    Portal portal;
} PortalHashEnt;

#define PortalHashTableLookup(NAME, PORTAL)                                                              \
    do {                                                                                                 \
        PortalHashEnt* hentry = NULL;                                                                    \
                                                                                                         \
        hentry = (PortalHashEnt*)hash_search(u_sess->exec_cxt.PortalHashTable, (NAME), HASH_FIND, NULL); \
        if (hentry != NULL) {                                                                            \
            PORTAL = hentry->portal;                                                                     \
        } else {                                                                                         \
            PORTAL = NULL;                                                                               \
        }                                                                                                \
    } while (0)

#define PortalHashTableInsert(PORTAL, NAME)                                                                 \
    do {                                                                                                    \
        PortalHashEnt* hentry = NULL;                                                                       \
        bool found = false;                                                                                 \
                                                                                                            \
        hentry = (PortalHashEnt*)hash_search(u_sess->exec_cxt.PortalHashTable, (NAME), HASH_ENTER, &found); \
        if (found) {                                                                                        \
            ereport(ERROR, (errcode(ERRCODE_SYSTEM_ERROR), errmsg("duplicate portal name")));               \
        }                                                                                                   \
        hentry->portal = PORTAL;                                                                            \
        /* To avoid duplicate storage, make PORTAL->name point to htab entry */                             \
        (PORTAL)->name = hentry->portalname;                                                                \
    } while (0)

#define PortalHashTableDelete(PORTAL)                                                                              \
    do {                                                                                                           \
        PortalHashEnt* hentry = NULL;                                                                              \
                                                                                                                   \
        hentry = (PortalHashEnt*)hash_search(u_sess->exec_cxt.PortalHashTable, (PORTAL)->name, HASH_REMOVE, NULL); \
        if (hentry == NULL) {                                                                                      \
            ereport(WARNING, (errmsg("trying to delete portal name that does not exist")));                        \
        }                                                                                                          \
    } while (0)

#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/* -------------------portal_mem_cxt---------------------------------
 *				   public portal interface functions
 * ----------------------------------------------------------------
 */
/*
 * EnablePortalManager
 *		Enables the portal management module at backend startup.
 */
void EnablePortalManager(void)
{
    HASHCTL ctl;

    Assert(u_sess->top_portal_cxt == NULL);

    u_sess->top_portal_cxt = AllocSetContextCreate(u_sess->top_mem_cxt,
        "portal_mem_cxt",
        ALLOCSET_DEFAULT_MINSIZE,
        ALLOCSET_DEFAULT_INITSIZE,
        ALLOCSET_DEFAULT_MAXSIZE);

    ctl.keysize = MAX_PORTALNAME_LEN;
    ctl.entrysize = sizeof(PortalHashEnt);
    ctl.hcxt = SESS_GET_MEM_CXT_GROUP(MEMORY_CONTEXT_CBB);

    /*
     * use PORTALS_PER_USER as a guess of how many hash table entries to
     * create, initially
     */
    u_sess->exec_cxt.PortalHashTable = hash_create("Portal hash", PORTALS_PER_USER, &ctl, HASH_ELEM | HASH_CONTEXT);
}

/*
 * GetPortalByName
 *		Returns a portal given a portal name, or NULL if name not found.
 */


/*
 * PortalListGetPrimaryStmt
 *		Get the "primary" stmt within a portal, ie, the one marked canSetTag.
 *
 * Returns NULL if no such stmt.  If multiple PlannedStmt structs within the
 * portal are marked canSetTag, returns the first one.	Neither of these
 * cases should occur in present usages of this function.
 *
 * Copes if given a list of Querys --- can't happen in a portal, but this
 * code also supports plancache.c, which needs both cases.
 *
 * Note: the reason this is just handed a List is so that plancache.c
 * can share the code.	For use with a portal, use PortalGetPrimaryStmt
 * rather than calling this directly.
 */


/*
 * CreatePortal
 *		Returns a new portal given a name.
 *
 * allowDup: if true, automatically drop any pre-existing portal of the
 * same name (if false, an error is raised).
 *
 * dupSilent: if true, don't even emit a WARNING.
 */
#ifndef ENABLE_MULTIPLE_NODES
#endif
#ifdef PGXC
#endif

/*
 * CreateNewPortal
 *		Create a new portal, assigning it a random nonconflicting name.
 */


/*
 * PortalDefineQuery
 *		A simple subroutine to establish a portal's query.
 *
 * Notes: as of PG 8.4, caller MUST supply a sourceText string; it is not
 * allowed anymore to pass NULL.  (If you really don't have source text,
 * you can pass a constant string, perhaps "(query not available)".)
 *
 * commandTag shall be NULL if and only if the original query string
 * (before rewriting) was an empty string.	Also, the passed commandTag must
 * be a pointer to a constant string, since it is not copied.
 *
 * If cplan is provided, then it is a cached plan containing the stmts, and
 * the caller must have done GetCachedPlan(), causing a refcount increment.
 * The refcount will be released when the portal is destroyed.
 *
 * If cplan is NULL, then it is the caller's responsibility to ensure that
 * the passed plan trees have adequate lifetime.  Typically this is done by
 * copying them into the portal's heap context.
 *
 * The caller is also responsible for ensuring that the passed prepStmtName
 * (if not NULL) and sourceText have adequate lifetime.
 *
 * NB: this function mustn't do much beyond storing the passed values; in
 * particular don't do anything that risks elog(ERROR).  If that were to
 * happen here before storing the cplan reference, we'd leak the plancache
 * refcount that the caller is trying to hand off to us.
 */


/*
 * PortalReleaseCachedPlan
 *		Release a portal's reference to its cached plan, if any.
 */


/*
 * PortalCreateHoldStore
 *		Create the tuplestore for a portal.
 */
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif

/*
 * PinPortal
 *		Protect a portal from dropping.
 *
 * A pinned portal is still unpinned and dropped at transaction or
 * subtransaction abort.
 */




/*
 * MarkPortalActive
 *		Transition a portal from READY to ACTIVE state.
 *
 * NOTE: never set portal->status = PORTAL_ACTIVE directly; call this instead.
 */


/*
 * MarkPortalDone
 *		Transition a portal from ACTIVE to DONE state.
 *
 * NOTE: never set portal->status = PORTAL_DONE directly; call this instead.
 */


/*
 * MarkPortalFailed
 *		Transition a portal into FAILED state.
 *
 * NOTE: never set portal->status = PORTAL_FAILED directly; call this instead.
 */




/*
 * PortalDrop
 *		Destroy the portal.
 */
#ifdef USE_SPQ
#endif /* USE_SPQ */
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#else
#endif
#ifndef ENABLE_MULTIPLE_NODES
#endif

/*
 * Delete all declared cursors.
 *
 * Used by commands: CLOSE ALL, DISCARD ALL
 */


/*
 * "Hold" a portal.  Prepare it for access by later transactions.
 */


/*
 * Pre-commit processing for portals.
 *
 * Holdable cursors created in this transaction need to be converted to
 * materialized form, since we are going to close down the executor and
 * release locks.  Non-holdable portals created in this transaction are
 * simply removed.	Portals remaining from prior transactions should be
 * left untouched.
 *
 * Returns TRUE if any portals changed state (possibly causing user-defined
 * code to be run), FALSE if not.
 */


/*
 * Abort processing for portals.
 *
 * At this point we reset "active" status and run the cleanup hook if
 * present, but we can't release the portal's memory until the cleanup call.
 *
 * The reason we need to reset active is so that we can replace the unnamed
 * portal, else we'll fail to execute ROLLBACK when it arrives.
 * At this point we run the cleanup hook if present, but we can't release the
 * portal's memory until the cleanup call.
 */
#ifndef ENABLE_MULTIPLE_NODES
#endif

/*
 * Post-abort cleanup for portals.
 *
 * Delete all portals not held over from prior transactions.  */
#ifdef PGXC
#endif

/*
 * Portal-related cleanup when we return to the main loop on error.
 *
 * This is different from the cleanup at transaction abort.  Auto-held portals
 * are cleaned up on error but not on transaction abort.
 */


/*
 * Pre-subcommit processing for portals.
 *
 * Reassign the portals created in the current subtransaction to the parent
 * subtransaction.
 */


/*
 * Subtransaction abort handling for portals.
 *
 * Deactivate portals created or used during the failed subtransaction.
 * Note that per AtSubCommit_Portals, this will catch portals created/used
 * in descendants of the subtransaction too.
 *
 * We don't destroy any portals here; that's done in AtSubCleanup_Portals.
 */


/*
 * Post-subabort cleanup for portals.
 *
 * Drop all portals created in the failed subtransaction (but note that
 * we will not drop any that were reassigned to the parent above).
 */


/* Find all available cursors */




/*
 * Description: reset cursor's option which is opened under current subtransaction
 *              when current transaction is abort.
 * Parameters:
 * @in mySubid: current transaction Id.
 * @in funOid: function oid, 0 if in exception block.
 * @in funUseCount：times of function had been used,  0 if in exception block.
 * @in reset: wether reset cursor's option or not.
 * Return: void
 */

/*
 * Hold all pinned portals.
 *
 * A procedural language implementation that uses pinned portals for its
 * internally generated cursors can call this in its COMMIT command to convert
 * those cursors to held cursors, so that they survive the transaction end.
 * We mark those portals as "auto-held" so that exception exit knows to clean
 * them up.  (In normal, non-exception code paths, the PL needs to clean those
 * portals itself, since transaction end won't do it anymore, but that should
 * be normal practice anyway.)
 */

/* Release the dependency between CURSOR and ROW type */

