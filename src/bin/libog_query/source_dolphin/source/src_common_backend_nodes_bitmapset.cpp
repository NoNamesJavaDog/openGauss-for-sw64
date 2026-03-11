/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - bms_copy
 * - bms_first_member
 * - bms_free
 * - bms_hash_value
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * bitmapset.cpp
 *	  openGauss generic bitmap set package
 *
 * A bitmap set can represent any set of nonnegative integers, although
 * it is mainly intended for sets where the maximum value is not large,
 * say at most a few hundred.  By convention, a NULL pointer is always
 * accepted by all operations to represent the empty set.  (But beware
 * that this is not the only representation of the empty set.  Use
 * bms_is_empty() in preference to testing for NULL.)
 *
 *
 * Copyright (c) 2003-2012, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  src/common/backend/nodes/bitmapset.cpp
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"
#include "port/pg_bitutils.h"

#include "access/hash.h"

#define WORDNUM(x) ((x) / BITS_PER_BITMAPWORD)
#define BITNUM(x) ((x) % BITS_PER_BITMAPWORD)

#define BITMAPSET_SIZE(nwords) (offsetof(Bitmapset, words) + (nwords) * sizeof(bitmapword))

/* ----------
 * This is a well-known cute trick for isolating the rightmost one-bit
 * in a word.  It assumes two's complement arithmetic.  Consider any
 * nonzero value, and focus attention on the rightmost one.  The value is
 * then something like
 *				xxxxxx10000
 * where x's are unspecified bits.  The two's complement negative is formed
 * by inverting all the bits and adding one.  Inversion gives
 *				yyyyyy01111
 * where each y is the inverse of the corresponding x.	Incrementing gives
 *				yyyyyy10000
 * and then ANDing with the original value gives
 *				00000010000
 * This works for all cases except original value = zero, where of course
 * we get zero.
 * ----------
 */
#define RIGHTMOST_ONE(x) ((signedbitmapword)(x) & -((signedbitmapword)(x)))

#define HAS_MULTIPLE_ONES(x) ((bitmapword)RIGHTMOST_ONE(x) != (x))

/* Select appropriate bit-twiddling functions for bitmap word size */
#if BITS_PER_BITMAPWORD == 32
#define bmw_leftmost_one_pos(w) pg_leftmost_one_pos32(w)
#define bmw_rightmost_one_pos(w) pg_rightmost_one_pos32(w)
#define bmw_popcount(w) pg_popcount32(w)
#elif BITS_PER_BITMAPWORD == 64
#define bmw_leftmost_one_pos(w) pg_leftmost_one_pos64(w)
#define bmw_rightmost_one_pos(w) pg_rightmost_one_pos64(w)
#define bmw_popcount(w) pg_popcount64(w)
#else
#error "invalid BITS_PER_BITMAPWORD"
#endif

/*
 * bms_copy - make a palloc'd copy of a bitmapset
 */
Bitmapset* bms_copy(const Bitmapset* a)
{
    Bitmapset* result = NULL;
    size_t size;

    if (a == NULL) {
        return NULL;
    }
    size = BITMAPSET_SIZE(a->nwords);
    result = (Bitmapset*)palloc(size);
    errno_t rc = memcpy_s(result, size, a, size);
    securec_check(rc, "\0", "\0");
    return result;
}

/*
 * bms_equal - are two bitmapsets equal?
 *
 * This is logical not physical equality; in particular, a NULL pointer will
 * be reported as equal to a palloc'd value containing no members.
 */


/*
 * bms_make_singleton - build a bitmapset containing a single member
 */


/*
 * bms_free - free a bitmapset
 *
 * Same as pfree except for allowing NULL input
 */
void bms_free(Bitmapset* a)
{
    if (a != NULL) {
        pfree_ext(a);
    }
}

/*
 * These operations all make a freshly palloc'd result,
 * leaving their inputs untouched
 */

/*
 * bms_union - set union
 */


/*
 * bms_intersect - set intersection
 */


/*
 * bms_difference - set difference (ie, A without members of B)
 */


/*
 * bms_is_subset - is A a subset of B?
 */


/*
 * bms_subset_compare - compare A and B for equality/subset relationships
 *
 * This is more efficient than testing bms_is_subset in both directions.
 */


/*
 * bms_is_member - is X a member of A?
 */


/*
 * bms_overlap - do sets overlap (ie, have a nonempty intersection)?
 */


/*
 * bms_nonempty_difference - do sets have a nonempty difference?
 */


/*
 * bms_singleton_member - return the sole integer member of set
 *
 * Raises error if |a| is not 1.
 */


/*
 * bms_num_members - count members of set
 */


/*
 * bms_membership - does a set have zero, one, or multiple members?
 *
 * This is faster than making an exact count with bms_num_members().
 */


/*
 * bms_is_empty - is a set empty?
 *
 * This is even faster than bms_membership().
 */


/*
 * These operations all "recycle" their non-const inputs, ie, either
 * return the modified input or pfree it if it can't hold the result.
 *
 * These should generally be used in the style
 *
 *		foo = bms_add_member(foo, x);
 */

/*
 * bms_add_member - add a specified member to set
 *
 * Input set is modified or recycled!
 */


/*
 * bms_del_member - remove a specified member from set
 *
 * No error if x is not currently a member of set
 *
 * Input set is modified in-place!
 */


/*
 * bms_add_members - like bms_union, but left input is recycled
 */


/*
 * bms_int_members - like bms_intersect, but left input is recycled
 */


/*
 * bms_del_members - like bms_difference, but left input is recycled
 */


/*
 * bms_join - like bms_union, but *both* inputs are recycled
 */


/* ----------
 * bms_first_member - find and remove first member of a set
 *
 * Returns -1 if set is empty.	NB: set is destructively modified!
 *
 * This is intended as support for iterating through the members of a set.
 * The typical pattern is
 *
 *			tmpset = bms_copy(inputset);
 *			while ((x = bms_first_member(tmpset)) >= 0)
 *				process member x;
 *			bms_free_ext(tmpset);
 * ----------
 */
int bms_first_member(Bitmapset* a)
{
    int nwords;
    int wordnum;

    if (a == NULL) {
        return -1;
    }
    nwords = a->nwords;
    for (wordnum = 0; wordnum < nwords; wordnum++) {
        bitmapword w = a->words[wordnum];

        if (w != 0) {
            int result;

            w = RIGHTMOST_ONE(w);
            a->words[wordnum] &= ~w;

            result = wordnum * BITS_PER_BITMAPWORD;
            result += bmw_rightmost_one_pos(w);
            return result;
        }
    }
    return -1;
}

/*
 * bms_hash_value - compute a hash key for a Bitmapset
 *
 * Note: we must ensure that any two bitmapsets that are bms_equal() will
 * hash to the same value; in practice this means that trailing all-zero
 * words must not affect the result.  Hence we strip those before applying
 * hash_any().
 */
uint32 bms_hash_value(const Bitmapset* a)
{
    int lastword;

    if (a == NULL) {
        return 0; /* All empty sets hash to 0 */
    }
    for (lastword = a->nwords; --lastword >= 0;) {
        if (a->words[lastword] != 0) {
            break;
        }
    }
    if (lastword < 0) {
        return 0; /* All empty sets hash to 0 */
    }
    return DatumGetUInt32(hash_any((const unsigned char*)a->words, (lastword + 1) * sizeof(bitmapword)));
}

/*
 * bms_next_member - find next member of a set
 *
 * Returns smallest member greater than "prevbit", or -2 if there is none.
 * "prevbit" must NOT be less than -1, or the behavior is unpredictable.
 *
 * This is intended as support for iterating through the members of a set.
 * The typical pattern is
 *
 *			x = -1;
 *			while ((x = bms_next_member(inputset, x)) >= 0)
 *				process member x;
 *
 * Notice that when there are no more members, we return -2, not -1 as you
 * might expect.  The rationale for that is to allow distinguishing the
 * loop-not-started state (x == -1) from the loop-completed state (x == -2).
 * It makes no difference in simple loop usage, but complex iteration logic
 * might need such an ability.
 */


#ifndef ENABLE_MULTIPLE_NODES
/*
 * bms_get_singleton_member
 *
 * Test whether the given set is a singleton.
 * If so, set *member to the value of its sole member, and return true.
 * If not, return false, without changing *member.
 *
 * This is more convenient and faster than calling bms_membership() and then
 * bms_singleton_member(), if we don't care about distinguishing empty sets
 * from multiple-member sets.
 */


/*
 * bms_member_index
 *		determine 0-based index of member x in the bitmap
 *
 * Returns (-1) when x is not a member.
 */

#endif