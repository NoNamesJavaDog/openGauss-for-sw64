/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - DLInitElem
 * - DllistWithLock::DllistWithLock
 * - DLInitList
 * - DLInitElem
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * dllist.cpp
 *	  this is a simple doubly linked list implementation
 *	  the elements of the lists are void*
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/common/backend/lib/dllist.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "lib/dllist.h"
#include "miscadmin.h"





void DLInitList(Dllist* list)
{
    list->dll_head = NULL;
    list->dll_tail = NULL;
    list->dll_len = 0;
}

/*
 * free up a list and all the nodes in it --- but *not* whatever the nodes
 * might point to!
 */




void DLInitElem(Dlelem* e, void* val)
{
    e->dle_next = NULL;
    e->dle_prev = NULL;
    e->dle_val = val;
    e->dle_list = NULL;
}













/* Same as DLRemove followed by DLAddHead, but faster */


/*
 * double-linked list length
 */


DllistWithLock::DllistWithLock()
{
    DLInitList(&m_list);
    SpinLockInit(&m_lock);
}

DllistWithLock::~DllistWithLock()
{
    SpinLockFree(&m_lock);
}




















