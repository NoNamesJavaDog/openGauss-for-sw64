/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - pg_ltoa
 * - pg_ltoa_n
 * - pg_ultoa_n
 * - decimalLength32
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * numutils.c
 *	  utility functions for I/O of built-in numeric types.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/numutils.c
 *
 * -------------------------------------------------------------------------
 */
#include "utils/numutils.h"

#include "postgres.h"
#include "knl/knl_variable.h"
#include "port/pg_bitutils.h"

#include <math.h>
#include <limits>
#include <ctype.h>

#include "common/int.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include "plugin_commands/mysqlmode.h"

static inline int
decimalLength32(const uint32 v)
{
    int t;
    static const uint32 PowersOfTen[] = {
        1, 10, 100,
        1000, 10000, 100000,
        1000000, 10000000, 100000000,
        1000000000
    };

    /*
     * Compute base-10 logarithm by dividing the base-2 logarithm by a
     * good-enough approximation of the base-2 logarithm of 10
     */
    t = (pg_leftmost_one_pos32(v) + 1) * 1233 / 4096;
    return t + (v >= PowersOfTen[t]);
}



/*
 * pg_atoi: convert string to integer
 *
 * allows any number of leading or trailing whitespace characters.
 *
 * 'size' is the sizeof() the desired integral result (1, 2, or 4 bytes).
 *
 * c, if not 0, is a terminator character that may appear after the
 * integer (plus whitespace).  If 0, the string must end after the integer.
 *
 * can_ignore, if is true, means the input s will be truncated when its value
 * is invalid for integer.
 *
 * Unlike plain atoi(), this will throw ereport() upon bad input format or
 * overflow.
 */


#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#endif
#if defined(HAVE_LONG_INT_64)
#endif
#ifdef DOLPHIN
#else
#endif

/*
 * Convert input string to a signed 16 bit integer.
 *
 * Allows any number of leading or trailing whitespace characters. Will throw
 * ereport() upon bad input format or overflow.
 *
 * NB: Accumulate input as a negative number, to deal with two's complement
 * representation of the most negative number, which can't be represented as a
 * positive number.
 */
#ifdef DOLPHIN
#endif

/*
 * Convert input string to a signed 32 bit integer.
 *
 * Allows any number of leading or trailing whitespace characters. Will throw
 * ereport() upon bad input format or overflow.
 *
 * NB: Accumulate input as a negative number, to deal with two's complement
 * representation of the most negative number, which can't be represented as a
 * positive number.
 */
#ifdef DOLPHIN
#endif

/*
 * Converts a unsigned 8-bit integer to its string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result
 * (at least 5 bytes, counting a leading sign and trailing NUL).
 *
 * It doesn't seem worth implementing this separately.
 */
#ifdef DOLPHIN

#else
void pg_ctoa(uint8 c, char* a)
{
    pg_ltoa((int32)c, a);
}
#endif

/*
 * Converts a signed 16-bit integer to its string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result
 * (at least 7 bytes, counting a leading sign and trailing NUL).
 *
 * It doesn't seem worth implementing this separately.
 */


/*
 * Converts an unsigned 32-bit integer to its string representation,
 * not NUL-terminated, and returns the length of that string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result (at
 * least 10 bytes)
 */
static inline int pg_ultoa_n(uint32 value, char* a)
{
    int i = 0;

    /* Degenerate case */
    if (value == 0) {
        *a = '0';
        return 1;
    }

    int olength = decimalLength32(value);

    /*
     * Compute the result string. Use memcpy instead of memcpy_s/memcpy_sp for 
     * better performance.
     */
    while (value >= 10000) {
        const uint32 c = value - 10000 * (value / 10000);
        const uint32 c0 = (c % 100) << 1;
        const uint32 c1 = (c / 100) << 1;

        char* pos = a + olength - i;

        value /= 10000;

        memcpy(pos - 2, DIGIT_TABLE + c0, 2);
        memcpy(pos - 4, DIGIT_TABLE + c1, 2);
        i += 4;
    }
    if (value >= 100) {
        const uint32 c = (value % 100) << 1;

        char* pos = a + olength - i;

        value /= 100;

        memcpy(pos - 2, DIGIT_TABLE + c, 2);
        i += 2;
    }
    if (value >= 10) {
        const uint32 c = value << 1;

        char* pos = a + olength - i;

        memcpy(pos - 2, DIGIT_TABLE + c, 2);
    } else {
        *a = (char)('0' + value);
    }

    return olength;
}

/*
 * Converts a signed 32-bit integer to its string representation,
 * not NUL-terminated, and returns the length of that string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result (at
 * least 11 bytes)
 */
static inline int pg_ltoa_n(int32 value, char* a)
{
    uint32 uvalue = (uint32)value;
    int len = 0;

    if (value < 0) {
        uvalue = (uint32)0 - uvalue;
        a[len++] = '-';
    }

    len += pg_ultoa_n(uvalue, a + len);

    return len;
}

/*
 * Converts a signed 32-bit integer to its string representation
 *
 * @param a The buffer to output conversion result. Caller must ensure
 * that `a` points to enough memory to hold the result (at least 12
 * bytes, counting a leading sign and trailing NUL).
 */
void pg_ltoa(int32 value, char* a)
{
    int len = pg_ltoa_n(value, a);
    a[len] = '\0';
}

void pg_ltoa(int32 value, char* a, int* len)
{
    *len = pg_ltoa_n(value, a);
    a[*len] = '\0';
}

/*
 * Get the decimal representation, not NUL-terminated, and return the length of
 * same. Caller must ensure that a points to at least MAXINT8LEN bytes.
 */


/*
 * Get the decimal representation, not NUL-terminated, and return the length of
 * same.  Caller must ensure that a points to at least MAXINT8LEN bytes.
 */
static inline int pg_lltoa_n(int64 value, char* a) { return 0; }


/*
 * Convert a signed 64-bit integer to its string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result
 * (at least MAXINT8LEN+1 bytes, counting a leading sign and trailing NUL).
 */


void pg_lltoa(int64 value, char* a, int* len)
{
    *len = pg_lltoa_n(value, a);
    a[*len] = '\0';
}

/*
 * pg_lltoa: convert a signed 128-bit integer to its string representation
 *
 * Caller must ensure that 'a' points to enough memory to hold the result
 * (at least MAXINT16LEN+1 bytes, counting a leading sign and trailing NUL).
 */


/*
 * pg_ultostr
 *		Converts 'value' into a decimal string representation stored at 'str'.
 *
 * Returns the ending address of the string result (the last character written
 * plus 1).  Note that no NUL terminator is written.
 *
 * The intended use-case for this function is to build strings that contain
 * multiple individual numbers, for example:
 *
 *	str = pg_ultostr(str, a);
 *	*str++ = ' ';
 *	str = pg_ultostr(str, b);
 *	*str = '\0';
 *
 * Note: Caller must ensure that 'str' points to enough memory to hold the
 * result.
 */


/*
 * Converts 'value' into a decimal string representation stored at 'str'.
 * 'min_width' specifies the minimum width of the result; any extra space
 * is filled up by prefixing the number with zeros.
 *
 * Returns the ending address of the string result (the last character written
 * plus 1).  Note that no NUL terminator is written.
 *
 * The intended use-case for this function is to build strings that contain
 * multiple individual numbers, for example:
 *
 * ```cpp
 * str = pg_ultostr_zeropad(str, hours, 2);
 * *str++ = ':';
 * str = pg_ultostr_zeropad(str, mins, 2);
 * *str++ = ':';
 * str = pg_ultostr_zeropad(str, secs, 2);
 * *str = '\0';
 * ```
 *
 * Note: Caller must ensure that 'str' points to enough memory to hold the
 * result
 */


/*
 * pg_strtouint64
 *		Converts 'str' into an unsigned 64-bit integer.
 *
 * This has the identical API to strtoul(3), except that it will handle
 * 64-bit ints even where "long" is narrower than that.
 *
 * For the moment it seems sufficient to assume that the platform has
 * such a function somewhere; let's not roll our own.
 */
#ifdef _MSC_VER /* MSVC only */
#else
#endif

#ifdef DOLPHIN

#endif
