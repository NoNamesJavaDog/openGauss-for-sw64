/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - lappend
 * - new_list
 * - new_tail_cell
 * - check_list_invariants
 * - list_concat
 * - lcons
 * - new_head_cell
 * - list_member_oid
 * - lappend_oid
 * - lcons_oid
 * - list_copy
 * - list_copy_cell
 * - list_free
 * - list_free_private
 * - list_nth
 * - list_nth_cell
 * - list_nth_int
 * - list_free_deep
 * - list_delete_first
 * - list_delete_cell
 * - lappend3
 * - list_copy_tail
 * - list_truncate
 * - lappend_int
 * - lappend2
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * list.cpp
 *	  implementation for PostgreSQL generic linked list package
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/common/backend/nodes/list.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "nodes/pg_list.h"
#include "nodes/parsenodes.h"


/*
 * Routines to simplify writing assertions about the type of a list; a
 * NIL list is considered to be an empty list of any type.
 */
#define IsPointerList(l) ((l) == NIL || IsA((l), List))
#define IsIntegerList(l) ((l) == NIL || IsA((l), IntList))
#define IsOidList(l) ((l) == NIL || IsA((l), OidList))

#ifdef USE_ASSERT_CHECKING
/*
 * Check that the specified List is valid (so far as we can tell).
 */
static void check_list_invariants(const List* list)
{
    if (list == NIL) {
        return;
    }

    Assert(list->length > 0);
    Assert(list->head != NULL);
    Assert(list->tail != NULL);

    Assert(list->type == T_List || list->type == T_IntList || list->type == T_OidList);

    if (list->length == 1) {
        Assert(list->head == list->tail);
    }
    if (list->length == 2) {
        Assert(list->head->next == list->tail);
    }
    Assert(list->tail->next == NULL);
}
#else
#define check_list_invariants(l)
#endif /* USE_ASSERT_CHECKING */

#ifdef USE_ASSERT_CHECKING
/*
 * Check if a list is sorted in ascending order and unique.
 * @param[IN] list : list to be check
 */

#else
#define check_sorted_unique_list(l)
#endif /* USE_ASSERT_CHECKING */

/*
 * Return a freshly allocated List. Since empty non-NIL lists are
 * invalid, new_list() also allocates the head cell of the new list:
 * the caller should be sure to fill in that cell's data.
 */
static List* new_list(NodeTag type)
{
    List* newList = NIL;
    ListCell* newHead = NULL;

    /* new_head->data is left undefined! */
    newHead = (ListCell*)palloc(sizeof(*newHead));
    newHead->next = NULL;

    newList = (List*)palloc(sizeof(*newList));
    newList->type = type;
    newList->length = 1;
    newList->head = newHead;
    newList->tail = newHead;

    return newList;
}

// create a dlist with specfied type


/*
 * Allocate a new cell and make it the head of the specified
 * list. Assumes the list it is passed is non-NIL.
 *
 * The data in the new head cell is undefined; the caller should be
 * sure to fill it in
 */
static void new_head_cell(List* list)
{
    ListCell* new_head = NULL;

    new_head = (ListCell*)palloc(sizeof(*new_head));
    new_head->next = list->head;

    list->head = new_head;
    list->length++;
}

/*
 * Allocate a new cell and make it the tail of the specified
 * list. Assumes the list it is passed is non-NIL.
 *
 * The data in the new tail cell is undefined; the caller should be
 * sure to fill it in
 */
static void new_tail_cell(List* list)
{
    ListCell* new_tail = NULL;

    new_tail = (ListCell*)palloc(sizeof(*new_tail));
    new_tail->next = NULL;

    list->tail->next = new_tail;
    list->tail = new_tail;
    list->length++;
}

// create a DistCell and append to the DList tail


// append a datum to the dlist


/*
 * Append a pointer to the list. A pointer to the modified list is
 * returned. Note that this function may or may not destructively
 * modify the list; callers should always use this function's return
 * value, rather than continuing to use the pointer passed as the
 * first argument.
 */
List* lappend(List* list, void* datum)
{
    Assert(IsPointerList(list));

    if (list == NIL) {
        list = new_list(T_List);
    } else {
        new_tail_cell(list);
    }

    lfirst(list->tail) = datum;
    check_list_invariants(list);
    return list;
}

/*
 * Append to the specified list.
 */
List* lappend2(List* list, ListCell* cell)
{
    Assert(IsPointerList(list));
    Assert(cell != NULL);

    cell->next = NULL;

    if (list == NIL) {
        /* new_head->data is left undefined! */
        list = (List*)palloc0_noexcept(sizeof(*list));
        if (list == NULL) {
            return list;
        }

        list->type = T_List;
        list->length = 1;
        list->head = cell;
        list->tail = cell;
    } else {
        if (list->head == NULL) {
            Assert(list->length == 0);
            list->head = cell;
            list->tail = list->head;
        } else {
            list->tail->next = cell;
            list->tail = cell;
        }

        list->length++;
    }

    check_list_invariants(list);
    return list;
}

/*
 * Append listFrom to the specified list.
 */
List* lappend3(List* list, List* listFrom)
{
    Assert(IsPointerList(list) && IsPointerList(listFrom));

    if (listFrom == NULL || listFrom->length == 0) {
        return list;
    }

    if (list == NULL) {
        list = (List*)palloc0_noexcept(sizeof(*list));
        if (list == NULL) {
            return list;
        }

        list->type = listFrom->type;
    }

    if (list->head == NULL) {
        list->head = listFrom->head;
        list->tail = listFrom->tail;
    } else {
        list->tail->next = listFrom->head;
        list->tail = listFrom->tail;
    }

    list->length += listFrom->length;

    listFrom->head = NULL;
    listFrom->tail = NULL;
    listFrom->length = 0;
    return list;
}

/*
 * Append an integer to the specified list. See lappend()
 */
List* lappend_int(List* list, int datum)
{
    Assert(IsIntegerList(list));

    if (list == NIL) {
        list = new_list(T_IntList);
    } else {
        new_tail_cell(list);
    }

    lfirst_int(list->tail) = datum;
    check_list_invariants(list);
    return list;
}

/*
 * Append an OID to the specified list. See lappend()
 */
List* lappend_oid(List* list, Oid datum)
{
    Assert(IsOidList(list));

    if (list == NIL) {
        list = new_list(T_OidList);
    } else {
        new_tail_cell(list);
    }

    lfirst_oid(list->tail) = datum;
    check_list_invariants(list);
    return list;
}

/*
 * Add a new cell to the list, in the position after 'prev_cell'. The
 * data in the cell is left undefined, and must be filled in by the
 * caller. 'list' is assumed to be non-NIL, and 'prev_cell' is assumed
 * to be non-NULL and a member of 'list'.
 */


/*
 * Add a new cell to the specified list (which must be non-NIL);
 * it will be placed after the list cell 'prev' (which must be
 * non-NULL and a member of 'list'). The data placed in the new cell
 * is 'datum'. The newly-constructed cell is returned.
 */






/*
 * delete the list cell for the match cases
 */


/*
 * handle each data in list
 */


/*
 * Prepend a new element to the list. A pointer to the modified list
 * is returned. Note that this function may or may not destructively
 * modify the list; callers should always use this function's return
 * value, rather than continuing to use the pointer passed as the
 * second argument.
 *
 * Caution: before Postgres 8.0, the original List was unmodified and
 * could be considered to retain its separate identity.  This is no longer
 * the case.
 */
List* lcons(void* datum, List* list)
{
    Assert(IsPointerList(list));

    if (list == NIL) {
        list = new_list(T_List);
    } else {
        new_head_cell(list);
    }

    lfirst(list->head) = datum;
    check_list_invariants(list);
    return list;
}

/*
 * Prepend an integer to the list. See lcons()
 */


/*
 * Prepend an OID to the list. See lcons()
 */
List* lcons_oid(Oid datum, List* list)
{
    Assert(IsOidList(list));

    if (list == NIL) {
        list = new_list(T_OidList);
    } else {
        new_head_cell(list);
    }

    lfirst_oid(list->head) = datum;
    check_list_invariants(list);
    return list;
}

/*
 * Concatenate list2 to the end of list1, and return list1. list1 is
 * destructively changed. Callers should be sure to use the return
 * value as the new pointer to the concatenated list: the 'list1'
 * input pointer may or may not be the same as the returned pointer.
 *
 * The nodes in list2 are merely appended to the end of list1 in-place
 * (i.e. they aren't copied; the two lists will share some of the same
 * storage). Therefore, invoking list_free() on list2 will also
 * invalidate a portion of list1.
 */
List* list_concat(List* list1, List* list2)
{
    if (list1 == NIL) {
        return list2;
    }
    if (list2 == NIL) {
        return list1;
    }
    if (list1 == list2) {
        ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg("cannot list_concat() a list to itself")));
    }

    Assert(list1->type == list2->type);

    list1->length += list2->length;
    list1->tail->next = list2->head;
    list1->tail = list2->tail;

    check_list_invariants(list1);
    return list1;
}

/* make new listcell to concat data of list2, not distory the struction of list2. */


/*
 * Truncate 'list' to contain no more than 'new_size' elements. This
 * modifies the list in-place! Despite this, callers should use the
 * pointer returned by this function to refer to the newly truncated
 * list -- it may or may not be the same as the pointer that was
 * passed.
 *
 * Note that any cells removed by list_truncate() are NOT pfree'd.
 */
List* list_truncate(List* list, int new_size)
{
    ListCell* cell = NULL;
    int n;

    if (new_size <= 0) {
        return NIL; /* truncate to zero length */
    }

    /* If asked to effectively extend the list, do nothing */
    if (new_size >= list_length(list)) {
        return list;
    }

    n = 1;
    foreach (cell, list) {
        if (n == new_size) {
            cell->next = NULL;
            list->tail = cell;
            list->length = new_size;
            check_list_invariants(list);
            return list;
        }
        n++;
    }

    /* keep the compiler quiet; never reached */
    Assert(false);
    return list;
}

/*
 * Locate the n'th cell (counting from 0) of the list.  It is an assertion
 * failure if there is no such cell.
 */
ListCell* list_nth_cell(const List* list, int n)
{
    ListCell* match = NULL;

    Assert(list != NIL);
    Assert(n >= 0);
    Assert(n < list->length);
    check_list_invariants(list);

    /* Does the caller actually mean to fetch the tail? */
    if (n == list->length - 1) {
        return list->tail;
    }

    for (match = list->head; n-- > 0; match = match->next) {}

    return match;
}

/*
 * Return the data value contained in the n'th element of the
 * specified list. (List elements begin at 0.)
 */
void* list_nth(const List* list, int n)
{
    Assert(IsPointerList(list));
    return lfirst(list_nth_cell(list, n));
}

/*
 * Return the integer value contained in the n'th element of the
 * specified list.
 */
int list_nth_int(const List* list, int n)
{
    Assert(IsIntegerList(list));
    return lfirst_int(list_nth_cell(list, n));
}

/*
 * Return the OID value contained in the n'th element of the specified
 * list.
 */


/*
 * Return true iff 'datum' is a member of the list. Equality is
 * determined via equal(), so callers should ensure that they pass a
 * Node as 'datum'.
 */


/*
 * Return true iff 'datum' is a member of the list. Equality is
 * determined by using simple pointer comparison.
 */


/*
 * Return true iff the integer 'datum' is a member of the list.
 */


/*
 * Return true iff the OID 'datum' is a member of the list.
 */
bool list_member_oid(const List* list, Oid datum)
{
    const ListCell* cell = NULL;

    Assert(IsOidList(list));
    check_list_invariants(list);

    foreach (cell, list) {
        if (lfirst_oid(cell) == datum) {
            return true;
        }
    }

    return false;
}

// delete a cell in a dlist and return the dlist


/*
 * Delete 'cell' from 'list'; 'prev' is the previous element to 'cell'
 * in 'list', if any (i.e. prev == NULL iff list->head == cell)
 *
 * The cell is pfree'd, as is the List header if this was the last member.
 */
List* list_delete_cell(List* list, ListCell* cell, ListCell* prev)
{
    check_list_invariants(list);
    Assert(prev != NULL ? lnext(prev) == cell : list_head(list) == cell);

    /*
     * If we're about to delete the last node from the list, free the whole
     * list instead and return NIL, which is the only valid representation of
     * a zero-length list.
     */
    if (list->length == 1) {
        list_free(list);
        return NIL;
    }

    /*
     * Otherwise, adjust the necessary list links, deallocate the particular
     * node we have just removed, and return the list we were given.
     */
    list->length--;

    if (prev != NULL) {
        prev->next = cell->next;
    } else {
        list->head = cell->next;
    }

    if (list->tail == cell) {
        list->tail = prev;
    }

    cell->next = NULL;
    pfree(cell);
    return list;
}

/*
 * Delete 'cell' from 'list'
 */


/*
 * Delete the first cell in list that matches datum, if any.
 * Equality is determined via equal().
 */


/* As above, but use simple pointer equality */


/* As above, but for integers */


/* As above, but for OIDs */


/* As above, but for name */


/*
 * Delete the first element of the list.
 *
 * This is useful to replace the Lisp-y code "list = lnext(list);" in cases
 * where the intent is to alter the list rather than just traverse it.
 * Beware that the removed cell is freed, whereas the lnext() coding leaves
 * the original list head intact if there's another pointer to it.
 */
List* list_delete_first(List* list)
{
    check_list_invariants(list);

    if (list == NIL) {
        return NIL; /* would an error be better? */
    }

    return list_delete_cell(list, list_head(list), NULL);
}

/* delete the first element of the list without freeing the list */


/*
 * Generate the union of two lists. This is calculated by copying
 * list1 via list_copy(), then adding to it all the members of list2
 * that aren't already in list1.
 *
 * Whether an element is already a member of the list is determined
 * via equal().
 *
 * The returned list is newly-allocated, although the content of the
 * cells is the same (i.e. any pointed-to objects are not copied).
 *
 * NB: this function will NOT remove any duplicates that are present
 * in list1 (so it only performs a "union" if list1 is known unique to
 * start with).  Also, if you are about to write "x = list_union(x, y)"
 * you probably want to use list_concat_unique() instead to avoid wasting
 * the list cells of the old x list.
 *
 * This function could probably be implemented a lot faster if it is a
 * performance bottleneck.
 */


/*
 * This variant of list_union() determines duplicates via simple
 * pointer comparison.
 */


/*
 * This variant of list_union() operates upon lists of integers.
 */


/*
 * This variant of list_union() operates upon lists of OIDs.
 */


/*
 * Return a list that contains all the cells that are in both list1 and
 * list2.  The returned list is freshly allocated via palloc(), but the
 * cells themselves point to the same objects as the cells of the
 * input lists.
 *
 * Duplicate entries in list1 will not be suppressed, so it's only a true
 * "intersection" if list1 is known unique beforehand.
 *
 * This variant works on lists of pointers, and determines list
 * membership via equal().	Note that the list1 member will be pointed
 * to in the result.
 */


#ifdef PGXC
/*
 * This variant of list_intersection() operates upon lists of integers.
 */

#endif

/*
 * Return a list that contains all the cells in list1 that are not in
 * list2. The returned list is freshly allocated via palloc(), but the
 * cells themselves point to the same objects as the cells of the
 * input lists.
 *
 * This variant works on lists of pointers, and determines list
 * membership via equal()
 */


/*
 * This variant of list_difference() determines list membership via
 * simple pointer equality.
 */


/*
 * This variant of list_difference() operates upon lists of integers.
 */


/*
 * This variant of list_difference() operates upon lists of OIDs.
 */


/*
 * Append datum to list, but only if it isn't already in the list.
 *
 * Whether an element is already a member of the list is determined
 * via equal().
 */


/*
 * This variant of list_append_unique() determines list membership via
 * simple pointer equality.
 */


/*
 * This variant of list_append_unique() operates upon lists of integers.
 */


/*
 * This variant of list_append_unique() operates upon lists of OIDs.
 */


/*
 * Append to list1 each member of list2 that isn't already in list1.
 *
 * Whether an element is already a member of the list is determined
 * via equal().
 *
 * This is almost the same functionality as list_union(), but list1 is
 * modified in-place rather than being copied.	Note also that list2's cells
 * are not inserted in list1, so the analogy to list_concat() isn't perfect.
 */


/*
 * This variant of list_concat_unique() determines list membership via
 * simple pointer equality.
 */


/*
 * This variant of list_concat_unique() operates upon lists of integers.
 */


/*
 * This variant of list_concat_unique() operates upon lists of OIDs.
 */


/*
 * Free all storage in a list, and optionally the pointed-to elements
 */
static void list_free_private(List* list, bool deep)
{
    ListCell* cell = NULL;

    check_list_invariants(list);

    cell = list_head(list);
    while (cell != NULL) {
        ListCell* tmp = cell;

        cell = lnext(cell);
        if (deep) {
            pfree(lfirst(tmp));
        }
        pfree(tmp);
    }

    if (list != NULL) {
        pfree(list);
    }
}

// free a dlist


/*
 * Free all the cells of the list, as well as the list itself. Any
 * objects that are pointed-to by the cells of the list are NOT
 * free'd.
 *
 * On return, the argument to this function has been freed, so the
 * caller would be wise to set it to NIL for safety's sake.
 */
void list_free(List* list)
{
    list_free_private(list, false);
}

/*
 * Free all the cells of the list, the list itself, and all the
 * objects pointed-to by the cells of the list (each element in the
 * list must contain a pointer to a palloc()'d region of memory!)
 *
 * On return, the argument to this function has been freed, so the
 * caller would be wise to set it to NIL for safety's sake.
 */
void list_free_deep(List* list)
{
    /*
     * A "deep" free operation only makes sense on a list of pointers.
     */
    Assert(IsPointerList(list));
    list_free_private(list, true);
}

void list_copy_cell(ListCell* oldlist_cur, List* newlist)
{
    ListCell* newlist_prev = newlist->head;

    while (oldlist_cur != NULL) {
        ListCell* newlist_cur = NULL;

        newlist_cur = (ListCell*)palloc(sizeof(*newlist_cur));
        newlist_cur->data = oldlist_cur->data;
        newlist_prev->next = newlist_cur;

        newlist_prev = newlist_cur;
        oldlist_cur = oldlist_cur->next;
    }

    newlist_prev->next = NULL;
    newlist->tail = newlist_prev;

    check_list_invariants(newlist);
}

/*
 * Return a shallow copy of the specified list.
 */
List* list_copy(const List* oldlist)
{
    List* newlist = NIL;
    ListCell* oldlist_cur = NULL;

    if (oldlist == NIL) {
        return NIL;
    }

    newlist = new_list(oldlist->type);
    newlist->length = oldlist->length;

    /*
     * Copy over the data in the first cell; new_list() has already allocated
     * the head cell itself
     */
    newlist->head->data = oldlist->head->data;

    oldlist_cur = oldlist->head->next;
    list_copy_cell(oldlist_cur, newlist);
    return newlist;
}

/*
 * Return a shallow copy of the specified list, without the first N elements.
 */
List* list_copy_tail(const List* oldlist, int nskip)
{
    List* newlist = NIL;
    ListCell* oldlist_cur = NULL;

    if (nskip < 0) {
        nskip = 0; /* would it be better to elog? */
    }

    if (oldlist == NIL || nskip >= oldlist->length) {
        return NIL;
    }

    newlist = new_list(oldlist->type);
    newlist->length = oldlist->length - nskip;

    /*
     * Skip over the unwanted elements.
     */
    oldlist_cur = oldlist->head;
    while (nskip-- > 0) {
        oldlist_cur = oldlist_cur->next;
    }

    /*
     * Copy over the data in the first remaining cell; new_list() has already
     * allocated the head cell itself
     */
    newlist->head->data = oldlist_cur->data;

    oldlist_cur = oldlist_cur->next;
    list_copy_cell(oldlist_cur, newlist);
    return newlist;
}

/*
 * pg_list.h defines inline versions of these functions if allowed by the
 * compiler; in which case the definitions below are skipped.
 */
#ifndef USE_INLINE

ListCell* list_head(const List* l)
{
    return l ? l->head : NULL;
}

ListCell* list_tail(const List* l)
{
    return l ? l->tail : NULL;
}

int list_length(const List* l)
{
    return l ? l->length : 0;
}

DListCell* dlist_head_cell(const DList* l)
{
    return l ? l->head : NULL;
}

DListCell* dlist_tail_cell(DList* l)
{
    return l ? l->tail : NULL;
}

#endif /* ! USE_INLINE */

/*
 * Temporary compatibility functions
 *
 * In order to avoid warnings for these function definitions, we need
 * to include a prototype here as well as in pg_list.h. That's because
 * we don't enable list API compatibility in list.c, so we
 * don't see the prototypes for these functions.
 */

/*
 * Given a list, return its length. This is merely defined for the
 * sake of backward compatibility: we can't afford to define a macro
 * called "length", so it must be a function. New code should use the
 * list_length() macro in order to avoid the overhead of a function
 * call.
 */
int length(const List* list);



// delete a listcell from the list with the special postitio


// return the number of cell in dlist


// add cell to dlist tail
// the cell here is already in dlist. So we just need to move it to the tail


/*
 * @Description: is list1 subset of list2.
 * @in list1: Sub list.
 * @in list2: Over list.
 * @return: If list1 is subset of list2 return true else return false.
 */


/*
 * @Description: is list1 subset of list2.
 * @in list1: Sub int list.
 * @in list2: Over int list.
 * @return: If list1 is subset of list2 return true else return false.
 */


/*
 * Merge sort two sorted and unique int list.
 *
 * @param[IN] list1: sorted list.
 * @param[IN] list2: sorted list.
 * return List*: merged list.
 */




