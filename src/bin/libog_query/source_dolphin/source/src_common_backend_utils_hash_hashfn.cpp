/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - uint32_hash
 * - string_hash
 * - tag_hash
 * - uint32_hash
 * - tag_hash
 * - string_hash
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/hash.h"

/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
uint32 string_hash(const void* key, Size keysize)
{
    /*
     * If the string exceeds keysize-1 bytes, we want to hash only that many,
     * because when it is copied into the hash table it will be truncated at
     * that length.
     */
    Size s_len = strlen((const char*)key);

    s_len = Min(s_len, keysize - 1);
    return DatumGetUInt32(hash_any((const unsigned char*)key, (int)s_len));
}

/*
 * tag_hash: hash function for fixed-size tag values
 */
uint32 tag_hash(const void* key, Size keysize)
{
    return DatumGetUInt32(hash_any((const unsigned char*)key, (int)keysize));
}

/*
 * uint32_hash: hash function for keys that are uint32 or int32
 *
 * (tag_hash works for this case too, but is slower)
 */
uint32 uint32_hash(const void *key, Size keysize)
{
	Assert(keysize == sizeof(uint32));
	return DatumGetUInt32(hash_uint32(*((const uint32 *) key)));
}

/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */


/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */


/*
 * bitmap_match: match function to use with bitmap_hash
 */

