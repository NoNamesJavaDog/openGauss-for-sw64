/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - bittoint4
 * - bittoint
 * - bittobigint
 * - GetLeadingZeroLen
 * - bit_in
 * - bit_bin_in
 * - bit_in_internal
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * varbit.c
 *	  Functions for the SQL datatypes BIT() and BIT VARYING().
 *
 * Code originally contributed by Adriaan Joubert.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/varbit.c
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/htup.h"
#include "common/int.h"
#include "libpq/pqformat.h"
#include "nodes/nodeFuncs.h"
#include "nodes/supportnodes.h"
#include "utils/array.h"
#include "access/tuptoaster.h"
#include "plugin_postgres.h"
#ifdef DOLPHIN
#include "utils/float.h"
#include "plugin_commands/mysqlmode.h"
#include "plugin_utils/int8.h"
#include "plugin_utils/year.h"
#include "plugin_utils/varbit.h"
#define BYTE_SIZE 8
#define SMALL_SIZE 16
#define M_BIT_LEN 64
#else
#include "utils/varbit.h"
#endif


#define HEXDIG(z) ((z) < 10 ? ((z) + '0') : ((z)-10 + 'A'))

static VarBit* bit_catenate(VarBit* arg1, VarBit* arg2);
static VarBit* bitsubstring(VarBit* arg, int32 s, int32 l, bool length_not_specified);
static VarBit* bit_overlay(VarBit* t1, VarBit* t2, int sp, int sl);

#ifdef DOLPHIN
extern int GetLeadingZeroLen(VarBit* arg);
extern Datum dolphin_bitposition(PG_FUNCTION_ARGS);
static int32 bit_cmp(VarBit* arg1, VarBit* arg2, int leadingZeroLen1 = -1, int leadingZeroLen2 = -1);
extern "C" Datum ui8toi1(PG_FUNCTION_ARGS);
extern "C" Datum ui8toi2(PG_FUNCTION_ARGS);
extern "C" Datum ui8toi4(PG_FUNCTION_ARGS);
extern "C" Datum ui8toui1(PG_FUNCTION_ARGS);
extern "C" Datum ui8toui2(PG_FUNCTION_ARGS);
extern "C" Datum ui8toui4(PG_FUNCTION_ARGS);
extern "C" Datum date_int8(PG_FUNCTION_ARGS);
extern "C" Datum datetime_float(PG_FUNCTION_ARGS);
extern "C" Datum timestamptz_int8(PG_FUNCTION_ARGS);
extern "C" Datum time_int8(PG_FUNCTION_ARGS);
extern "C" Datum year_integer(PG_FUNCTION_ARGS);
extern "C" Datum uint8out(PG_FUNCTION_ARGS);
extern "C" Datum dolphin_binaryin(PG_FUNCTION_ARGS);
Datum bittobigint(VarBit* arg, bool isUnsigned, bool canIgnore = false);
;
extern "C" DLL_PUBLIC Datum cot_bit(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum bit_bin_in(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum acos_bit(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum cos_bit(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum asin_bit(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum atan_bit(PG_FUNCTION_ARGS);
#endif


;
extern "C" DLL_PUBLIC Datum c_bitoctetlength(PG_FUNCTION_ARGS);

/*
 * common code for bittypmodin and varbittypmodin
 */


/*
 * common code for bittypmodout and varbittypmodout
 */


/* ----------
 *	attypmod -- contains the length of the bit string in bits, or for
 *			   varying bits the maximum length.
 *
 *	The data structure contains the following elements:
 *	  header  -- length of the whole data structure (incl header)
 *				 in bytes. (as with all varying length datatypes)
 *	  data section -- private data section for the bits data structures
 *		bitlength -- length of the bit string in bits
 *		bitdata   -- bit string, most significant byte first
 *
 *	The length of the bitdata vector should always be exactly as many
 *	bytes as are needed for the given bitlength.  If the bitlength is
 *	not a multiple of 8, the extra low-order padding bits of the last
 *	byte must be zeroes.
 * ----------
 */

/*
 * bit_in -
 *	  converts a char string to the internal representation of a bitstring.
 *		  The length is determined by the number of bits required plus
 *		  VARHDRSZ bytes or from atttypmod.
 */
#ifdef DOLPHIN
Datum bit_in_internal(char* input_string, int32 atttypmod, bool can_ignore)
{
    VarBit* result = NULL;    /* The resulting bit string			  */
    char* sp = NULL;          /* pointer into the character string  */
    bits8* r = NULL;          /* pointer into the result */
    int len,                  /* Length of the whole data structure */
        bitlen,               /* Number of bits in the bit string   */
        slen;                 /* Length of the input string		  */
    bool bit_not_hex = false; /* false = hex string  true = bit string */
    int bc;
    bits8 x = 0;
    
    /* Check that the first character is a b or an x */
    if (input_string[0] == 'b' || input_string[0] == 'B') {
        bit_not_hex = true;
        sp = input_string + 1;
    } else if (input_string[0] == 'x' || input_string[0] == 'X') {
        bit_not_hex = false;
        sp = input_string + 1;
    } else {
        /*
         * Otherwise it's binary.  This allows things like cast('1001' as bit)
         * to work transparently.
         */
        bit_not_hex = true;
        sp = input_string;
    }

    /*
     * Determine bitlength from input string.  MaxAllocSize ensures a regular
     * input is small enough, but we must check hex input.
     */
    slen = strlen(sp);
    if (bit_not_hex)
        bitlen = slen;
    else {
        if (slen > VARBITMAXLEN / 4)
            ereport(ERROR,
                (errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
                    errmsg("bit string length exceeds the maximum allowed (%d)", VARBITMAXLEN)));
        bitlen = slen * 4;
    }

    /*
     * Sometimes atttypmod is not supplied. If it is supplied we need to make
     * sure that the bitstring fits.
     */
    if (atttypmod <= 0)
        atttypmod = bitlen;
    else if (bitlen != atttypmod)
        ereport(ERROR,
            (errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
                errmsg("bit string length %d does not match type bit(%d)", bitlen, atttypmod)));

    len = VARBITTOTALLEN(atttypmod);
    /* set to 0 so that *r is always initialised and string is zero-padded */
    result = (VarBit*)palloc0(len);
    SET_VARSIZE(result, len);
    VARBITLEN(result) = atttypmod;

    r = VARBITS(result);
    if (bit_not_hex) {
        /* Parse the bit representation of the string */
        /* We know it fits, as bitlen was compared to atttypmod */
        x = HIGHBIT;
        for (; *sp; sp++) {
            if (*sp == '1')
                *r |= x;
            else if (*sp != '0') {
                ereport(can_ignore ? WARNING : ERROR,
                    (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("\"%c\" is not a valid binary digit", *sp)));
                /* if invalid input erro is ignorable, report warning and return a empty varbit */
                PG_RETURN_DATUM((Datum)DirectFunctionCall3(bit_in, CStringGetDatum(""), ObjectIdGetDatum(0), Int32GetDatum(-1)));
            }

            x >>= 1;
            if (x == 0) {
                x = HIGHBIT;
                r++;
            }
        }
    } else {
        /* Parse the hex representation of the string */
        for (bc = 0; *sp; sp++) {
            if (*sp >= '0' && *sp <= '9')
                x = (bits8)(*sp - '0');
            else if (*sp >= 'A' && *sp <= 'F')
                x = (bits8)(*sp - 'A') + 10;
            else if (*sp >= 'a' && *sp <= 'f')
                x = (bits8)(*sp - 'a') + 10;
            else {
                ereport(can_ignore ? WARNING : ERROR,
                    (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                        errmsg("\"%c\" is not a valid hexadecimal digit", *sp)));
                /* if invalid input erro is ignorable, report warning and return a empty varbit */
                PG_RETURN_DATUM((Datum)DirectFunctionCall3(bit_in, CStringGetDatum(""), ObjectIdGetDatum(0), Int32GetDatum(-1)));
            }

            if (bc) {
                *r++ |= x;
                bc = 0;
            } else {
                *r = x << 4;
                bc = 1;
            }
        }
    }

    PG_RETURN_VARBIT_P(result);
}
#endif

Datum bit_in(PG_FUNCTION_ARGS)
{
    char* input_string = PG_GETARG_CSTRING(0);

#ifdef NOT_USED
    Oid typelem = PG_GETARG_OID(1);
#endif
    int32 atttypmod = PG_GETARG_INT32(2);
    bool can_ignore = fcinfo->can_ignore;
    VarBit* result = NULL;    /* The resulting bit string			  */
    char* sp = NULL;          /* pointer into the character string  */
    int len,                  /* Length of the whole data structure */
        bitlen,               /* Number of bits in the bit string   */
        slen;                 /* Length of the input string		  */

#ifdef DOLPHIN
    if (ENABLE_B_CMPT_MODE) {
        sp = input_string;
        slen = strlen(sp);
        if (slen == 0) {
            PG_RETURN_DATUM((Datum)DirectFunctionCall3(bit_bin_in, CStringGetDatum(""),
                                                       ObjectIdGetDatum(0), Int32GetDatum(-1)));
        }
        if (slen > VARBITMAXLEN / BYTE_SIZE) {
            ereport(ERROR,
                (errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
                    errmsg("bit string length exceeds the maximum allowed (%d)", VARBITMAXLEN)));
        }
        bitlen = slen * BYTE_SIZE;

        if (atttypmod <= 0) {
            atttypmod = bitlen;
        } else if (bitlen != atttypmod) {
            ereport(ERROR,
                (errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
                    errmsg("bit string length %d does not match type bit(%d)", bitlen, atttypmod)));
        }

        len = VARBITTOTALLEN(atttypmod);
        /* set to 0 so that *r is always initialised and string is zero-padded */
        result = (VarBit*)palloc0(len);
        SET_VARSIZE(result, len);
        VARBITLEN(result) = atttypmod;

        errno_t ss_rc = 0;
        ss_rc = memcpy_s(VARBITS(result), slen, sp, slen);
        securec_check(ss_rc, "\0", "\0");

        PG_RETURN_VARBIT_P(result);
    }
#endif

    PG_RETURN_VARBIT_P(bit_in_internal(input_string, atttypmod, can_ignore));
}











#ifdef DOLPHIN
#endif

/*
 *		bit_recv			- converts external binary format to bit
 */
#ifdef NOT_USED
#endif
#ifdef DOLPHIN
#else
#endif

/*
 *		bit_send			- converts bit to binary format
 */


/*
 * bit()
 * Converts a bit() type to a specific internal length.
 * len is the bitlength specified in the column definition.
 *
 * If doing implicit cast, raise error when source data is wrong length.
 * If doing explicit cast, silently truncate or zero-pad to specified length.
 */
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#else
#endif





/*
 * varbit_in -
 *	  converts a string to the internal representation of a bitstring.
 *		This is the same as bit_in except that atttypmod is taken as
 *		the maximum length, not the exact length to force the bitstring to.
 */
#ifdef NOT_USED
#endif

/*
 * varbit_out -
 *	  Prints the string as bits to preserve length accurately
 *
 * XXX varbit_recv() and hex input to varbit_in() can load a value that this
 * cannot emit.  Consider using hex output for such values.
 */


/*
 *		varbit_recv			- converts external binary format to varbit
 *
 * External format is the bitlen as an int32, then the byte array.
 */
#ifdef NOT_USED
#endif

/*
 *		varbit_send			- converts varbit to binary format
 */


/*
 * varbit_support()
 *
 * Planner support function for the varbit() length coercion function.
 *
 * Currently, the only interesting thing we can do is flatten calls that set
 * the new maximum length >= the previous maximum length.  We can ignore the
 * isExplicit argument, since that only affects truncation cases.
 */


/*
 * varbit()
 * Converts a varbit() type to a specific internal length.
 * len is the maximum bitlength specified in the column definition.
 *
 * If doing implicit cast, raise error when source data is too long.
 * If doing explicit cast, silently truncate to max length.
 */






/*
 * Comparison operators
 *
 * We only need one set of comparison operators for bitstrings, as the lengths
 * are stored in the same way for zero-padded and varying bit strings.
 *
 * Note that the standard is not unambiguous about the comparison between
 * zero-padded bit strings and varying bitstrings. If the same value is written
 * into a zero padded bitstring as into a varying bitstring, but the zero
 * padded bitstring has greater length, it will be bigger.
 *
 * Zeros from the beginning of a bitstring cannot simply be ignored, as they
 * may be part of a bit string and may be significant.
 *
 * Note: btree indexes need these routines not to leak memory; therefore,
 * be careful to free working copies of toasted datums.  Most places don't
 * need to be so careful.
 */
#ifdef DOLPHIN
/* Get leading zero length of a varbit */
int GetLeadingZeroLen(VarBit* arg)
{
    int leadingZeroLen = 0;
    int i;
    /*
     * example: for a value b'1' in bit(5), it will be b'00001000', the first 4 zero is left padding,
     * the last 3 zero are not the real value, should be ignored. So we get the leading zero by:
     * 1. traverse the all bytes
     * 2. for every single byte, loop from the highest bit, check whether it's 0, end loop when meet 1
     */
    for (bits8* r = VARBITS(arg); r < VARBITEND(arg) && leadingZeroLen < VARBITLEN(arg); r++) {
        for (i = BITS_PER_BYTE - 1; i >= 0 && leadingZeroLen < VARBITLEN(arg); i--) {
            if (*r >> i == 1) {
                break;
            }
            leadingZeroLen++;
        }
        if (i != -1) {
            break;
        }
    }

    return leadingZeroLen;
}
#endif
/*
 * bit_cmp
 *
 * Compares two bitstrings and returns <0, 0, >0 depending on whether the first
 * string is smaller, equal, or bigger than the second. All bits are considered
 * but additional zero bits won't affect the result.
 */
#ifdef DOLPHIN
#else
#endif















/*
 * bitcat
 * Concatenation of bit strings
 */




/*
 * bitsubstr
 * retrieve a substring from the bit string.
 * Note, s is 1-based.
 * SQL draft 6.10 9)
 */






/*
 * bitoverlay
 *	Replace specified substring of first string with second
 *
 * The SQL standard defines OVERLAY() in terms of substring and concatenation.
 * This code is a direct implementation of what the standard says.
 */






/*
 * bitlength, bitoctetlength
 * Return the length of a bit string
 */
#ifdef DOLPHIN
#else
#endif





/*
 * bit_and
 * perform a logical AND on two bit strings.
 */


/*
 * bit_or
 * perform a logical OR on two bit strings.
 */


/*
 * bitxor
 * perform a logical XOR on two bit strings.
 */
#ifdef DOLPHIN
#else   
#endif

/*
 * bitnot
 * perform a logical NOT on a bit string.
 */


/*
 * bitshiftleft
 * do a left shift (i.e. towards the beginning of the string)
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#else
#endif

/*
 * bitshiftright
 * do a right shift (i.e. towards the end of the string)
 */
#ifdef DOLPHIN
#endif
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN





Datum bittoint(VarBit* arg, bool isUnsigned)
{
    uint32 result;
    bits8* r = NULL;
    int errlevel = SQL_MODE_STRICT() ? ERROR : WARNING;

    if (ENABLE_B_CMPT_MODE) {
        result = isUnsigned ? (uint32)DirectFunctionCall1(ui8toui4, Int64GetDatum(bittobigint(arg, true))) :
                 (uint32)DirectFunctionCall1(ui8toi4, Int64GetDatum(bittobigint(arg, true)));
        PG_RETURN_INT32(result);
    }

    /* Check that the bit string is not too long */
    if ((uint32)VARBITLEN(arg) - GetLeadingZeroLen(arg) > sizeof(result) * BITS_PER_BYTE) {
        if (isUnsigned) {
            ereport(errlevel, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE), errmsg("integer unsigned out of range")));
            PG_RETURN_UINT32(PG_UINT32_MAX);
        } else {
            ereport(errlevel, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE), errmsg("integer out of range")));
            PG_RETURN_UINT32(PG_INT32_MAX);
        }
    }

    result = 0;
    for (r = VARBITS(arg); r < VARBITEND(arg); r++) {
        result <<= BITS_PER_BYTE;
        result |= *r;
    }
    /* Now shift the result to take account of the padding at the end */
    result >>= VARBITPAD(arg);
    if (isUnsigned)
        PG_RETURN_UINT32(result);
    PG_RETURN_INT32(result);
}

Datum bittobigint(VarBit* arg, bool isUnsigned, bool canIgnore)
{
    uint64 result;
    bits8* r = NULL;
    int errlevel = !canIgnore && SQL_MODE_STRICT() ? ERROR : WARNING;

    /* Check that the bit string is not too long */
    if ((uint32)VARBITLEN(arg) - GetLeadingZeroLen(arg) > sizeof(result) * BITS_PER_BYTE) {
        if (isUnsigned) {
            ereport(errlevel, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE), errmsg("bigint unsigned out of range")));
            PG_RETURN_UINT64(PG_UINT64_MAX);
        } else {
            ereport(errlevel, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE), errmsg("bigint out of range")));
            PG_RETURN_INT64(PG_INT64_MAX);
        }
    }

    result = 0;
    for (r = VARBITS(arg); r < VARBITEND(arg); r++) {
        result <<= BITS_PER_BYTE;
        result |= *r;
    }
    /* Now shift the result to take account of the padding at the end */
    result >>= VARBITPAD(arg);
    if (isUnsigned)
        PG_RETURN_UINT64(result);
    PG_RETURN_INT64(result);
}









;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
extern "C" DLL_PUBLIC Datum bitfromint1(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittoint1(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromint2(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittoint2(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromuint1(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittouint1(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromuint2(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittouint2(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromuint4(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittouint4(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromuint8(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittouint8(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromnumeric(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromfloat4(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromfloat8(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromdate(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromdatetime(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromtimestamp(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromtime(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bitfromyear(PG_FUNCTION_ARGS);








































#endif

/*
 * This is not defined in any standard. We retain the natural ordering of
 * bits here, as it just seems more intuitive.
 */
#ifdef DOLPHIN
#else
#endif

Datum bittoint4(PG_FUNCTION_ARGS)
{
    VarBit* arg = PG_GETARG_VARBIT_P(0);
#ifdef DOLPHIN
    return bittoint(arg, false);
#else
    uint32 result;
    bits8* r = NULL;

    /* Check that the bit string is not too long */
    if ((uint32)VARBITLEN(arg) > sizeof(result) * BITS_PER_BYTE)
        ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE), errmsg("integer out of range")));

    result = 0;
    for (r = VARBITS(arg); r < VARBITEND(arg); r++) {
        result <<= BITS_PER_BYTE;
        result |= *r;
    }
    /* Now shift the result to take account of the padding at the end */
    result >>= VARBITPAD(arg);

    PG_RETURN_INT32(result);
#endif
}

#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN
#else
#endif

/*
 * Determines the position of S2 in the bitstring S1 (1-based string).
 * If S2 does not appear in S1 this function returns 0.
 * If S2 is of length 0 this function returns 1.
 * Compatible in usage with POSITION() functions for other data types.
 */
#ifdef DOLPHIN
#else
#endif

/*
 * bitsetbit
 *
 * Given an instance of type 'bit' creates a new one with
 * the Nth bit set to the given value.
 *
 * The bit location is specified left-to-right in a zero-based fashion
 * consistent with the other get_bit and set_bit functions, but
 * inconsistent with the standard substring, position, overlay functions
 */


/*
 * bitgetbit
 *
 * returns the value of the Nth bit of a bit array (0 or 1).
 *
 * The bit location is specified left-to-right in a zero-based fashion
 * consistent with the other get_bit and set_bit functions, but
 * inconsistent with the standard substring, position, overlay functions
 */








#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum bit_bool(PG_FUNCTION_ARGS);



;
extern "C" DLL_PUBLIC Datum bitlike(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bitnlike(PG_FUNCTION_ARGS);







;
extern "C" DLL_PUBLIC Datum bittotext(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittobpchar(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittovarchar(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittobinary(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittovarbinary(PG_FUNCTION_ARGS);


Datum bit_bin_in(PG_FUNCTION_ARGS)
{
    char* input_string = PG_GETARG_CSTRING(0);
#ifdef NOT_USED
    Oid typelem = PG_GETARG_OID(1);
#endif
    int32 atttypmod = PG_GETARG_INT32(2);
    bool can_ignore = fcinfo->can_ignore;
    
    PG_RETURN_VARBIT_P(bit_in_internal(input_string, atttypmod, can_ignore));
}

;
extern "C" DLL_PUBLIC Datum bool_bit(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum dolphin_bitnot(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum mp_bit_length_binary(PG_FUNCTION_ARGS);






/**
 * cast bit value to charater.
 * - if the length of bit value is not a multiple of 8, pad left 0.
 * - do not ignore '\0'
 */


/**
 * cast bit as char and substr.
 * - first pad left 0 if bit length is not multi 8,
 * - then substr like char, and don't ignore '\0'
 * - start index from 1
 * - length can't be negative
 * - when start<0, amend the sartPosition to abs(start) from last char
 */












/**
 * search binary text position, ignore '\0' in text.
 * - if string lenght is illegal, return -1
*/






;
extern "C" DLL_PUBLIC Datum varbit_larger(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbit_smaller(PG_FUNCTION_ARGS);


#endif
