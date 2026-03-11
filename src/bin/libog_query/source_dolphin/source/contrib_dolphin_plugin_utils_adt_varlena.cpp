/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - SplitIdentifierString
 * - get_step_len
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * varlena.c
 *	  Functions for the variable-length built-in types.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/varlena.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include <limits.h>
#include <stdlib.h>
#include <cinttypes>
#include <cstring>
#include <cmath>

#ifdef DOLPHIN
#include <openssl/rand.h>
#include <openssl/err.h>
#include "plugin_utils/varbit.h"
#include "utils/float.h"
#endif

#include "access/hash.h"
#include "access/tuptoaster.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_type.h"
#include "common/int.h"
#include "lib/hyperloglog.h"
#include "libpq/md5.h"
#include "libpq/pqformat.h"
#include "miscadmin.h"
#include "plugin_parser/scansup.h"
#include "plugin_parser/parse_type.h"
#include "port/pg_bswap.h"
#include "regex/regex.h"
#include "utils/builtins.h"
#include "utils/bytea.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"
#include "utils/numeric.h"
#include "utils/pg_locale.h"
#include "plugin_parser/parser.h"
#include "utils/int8.h"
#include "utils/sortsupport.h"
#include "executor/node/nodeSort.h"
#include "plugin_utils/my_locale.h"
#include "pgxc/groupmgr.h"
#include "openssl/evp.h"
#include "catalog/gs_collation.h"
#include "catalog/pg_collation_fn.h"
#include "plugin_postgres.h"
#include "parser/parse_coerce.h"
#include "catalog/pg_type.h"
#include "workload/cpwlm.h"
#include "utils/varbit.h"
#include "plugin_commands/mysqlmode.h"
#include "plugin_utils/varbit.h"
#include "plugin_utils/timestamp.h"
#include "plugin_utils/date.h"
#include "plugin_utils/int16.h"
#include "libpq/libpq-int.h"
#include "plugin_utils/varlena.h"
#include "catalog/pg_cast.h"
#include "plugin_protocol/auth.h"

#define BETWEEN_AND_ARGC 3
#define SUBSTR_WITH_LEN_OFFSET 2
#define SUBSTR_A_CMPT_OFFSET 4
#define JUDGE_INPUT_VALID(X, Y) ((NULL == (X)) || (NULL == (Y)))
#define GET_POSITIVE(X) ((X) > 0 ? (X) : ((-1) * (X)))
#ifdef DOLPHIN
#define BYTE_4 0xFF000000L
#define BYTE_3 0xFF0000L
#define BYTE_2 0xFF00L
#define MAX_BINARY_LENGTH 255
#define MAX_VARBINARY_LENGTH 65535
#define isDigital(_ch) (((_ch) >= '0') && ((_ch) <= '9'))
#define TinyBlobMaxAllocSize ((Size)255) /* 255B */
#define MediumBlobMaxAllocSize ((Size)(16 * 1024 * 1024 - 1)) /* 16MB - 1 */
#define BlobMaxAllocSize ((Size)(1024 * 1024 * 1024 - 8203)) /* 1GB - 8203 */
#define LongBlobMaxAllocSize (((Size)4 * 1024 * 1024 * 1024 - 1)) /* 4GB - 1 */
#define SOUND_THRESHOLD 4
#define ANTI_CODE '7'
#define MAX_CHARA_THRESHOLD 256
#define MAX_CHARA_REMINDERS_LEN 10
#define CONV_MAX_CHAR_LEN 65 //max 64bit and 1 sign bit
#define MYSQL_SUPPORT_MINUS_MAX_LENGTH 65
#define MAX_UINT32_STR "0xffffffff"
#define MAXBI64LEN 25
#define BINARY_LEN(len) ((len - 2) / 2)

static long convert_bit_to_int (PG_FUNCTION_ARGS, int idx);
static TimestampTz temporal_to_timestamptz(Oid type, int index, PG_FUNCTION_ARGS);
#endif
static int getResultPostionReverse(text* textStr, text* textStrToSearch, int32 beginIndex, int occurTimes);
static int getResultPostion(text* textStr, text* textStrToSearch, int32 beginIndex, int occurTimes);

typedef struct varlena unknown;
typedef struct varlena VarString;

typedef struct {
    bool use_wchar;  /* T if multibyte encoding */
    char* str1;      /* use these if not use_wchar */
    char* str2;      /* note: these point to original texts */
    pg_wchar* wstr1; /* use these if use_wchar */
    pg_wchar* wstr2; /* note: these are palloc'd */
    int len1;        /* string lengths in logical characters */
    int len2;
    /* Skip table for Boyer-Moore-Horspool search algorithm: */
    int skiptablemask;  /* mask for ANDing with skiptable subscripts */
    int skiptable[256]; /* skip distance for given mismatched char */
} TextPositionState;

typedef struct {
    char* buf1; /* 1st string, or abbreviation original string
                 * buf */
    char* buf2; /* 2nd string, or abbreviation strxfrm() buf */
    int buflen1;
    int buflen2;
    int last_len1;     /* Length of last buf1 string/strxfrm() input */
    int last_len2;     /* Length of last buf2 string/strxfrm() blob */
    int last_returned; /* Last comparison result (cache) */
    bool cache_blob;   /* Does buf2 contain strxfrm() blob, etc? */
    bool collate_c;
    bool bpchar;                /* Sorting pbchar, not varchar/text/bytea? */
    bool estimating;            /* true if estimating cardinality
                                 * refer to NumericSortSupport */
    hyperLogLogState abbr_card; /* Abbreviated key cardinality state */
    /* hyperLogLogState full_card;  Full key cardinality state */
    /* Don't use abbr_card/full_card to evaluate weather abort
     * fast comparation or not, use abbr_card/input_count instead
     * like numeric_sortsupport does.
     */
    int64 input_count; /* number of non-null values seen */
    double prop_card;  /* Required cardinality proportion */
#ifdef HAVE_LOCALE_T
    pg_locale_t locale;
#endif
} VarStringSortSupport;

/*
 * This should be large enough that most strings will fit, but small enough
 * that we feel comfortable putting it on the stack
 */
#define TEXTBUFLEN 1024

#define DatumGetUnknownP(X) ((unknown*)PG_DETOAST_DATUM(X))
#define DatumGetUnknownPCopy(X) ((unknown*)PG_DETOAST_DATUM_COPY(X))
#define PG_GETARG_UNKNOWN_P(n) DatumGetUnknownP(PG_GETARG_DATUM(n))
#define PG_GETARG_UNKNOWN_P_COPY(n) DatumGetUnknownPCopy(PG_GETARG_DATUM(n))
#define PG_RETURN_UNKNOWN_P(x) PG_RETURN_POINTER(x)

static int varstrfastcmp_c(Datum x, Datum y, SortSupport ssup);
static int bpcharfastcmp_c(Datum x, Datum y, SortSupport ssup);
static int varstrfastcmp_builtin(Datum x, Datum y, SortSupport ssup);
static int bpvarstrfastcmp_builtin(Datum x, Datum y, SortSupport ssup);
static int varstrfastcmp_locale(Datum x, Datum y, SortSupport ssup);
static int varstrcmp_abbrev(Datum x, Datum y, SortSupport ssup);
static Datum varstr_abbrev_convert(Datum original, SortSupport ssup);
static bool varstr_abbrev_abort(int memtupcount, SortSupport ssup);
static int text_position(text* t1, text* t2);
static void text_position_setup(text* t1, text* t2, TextPositionState* state);
static int text_position_next(int start_pos, TextPositionState* state);
static void text_position_cleanup(TextPositionState* state);
static text* text_catenate(text* t1, text* t2);
static text* text_overlay(text* t1, text* t2, int sp, int sl);
static void appendStringInfoText(StringInfo str, const text* t);
static bytea* bytea_catenate(bytea* t1, bytea* t2);
static bytea* bytea_substring_orclcompat(Datum str, int S, int L, bool length_not_specified);
static bytea* bytea_overlay(bytea* t1, bytea* t2, int sp, int sl);
static StringInfo makeStringAggState(FunctionCallInfo fcinfo);

static Datum text_to_array_internal(PG_FUNCTION_ARGS);
static text* array_to_text_internal(FunctionCallInfo fcinfo, ArrayType* v, char* fldsep, char* null_string);

static bool text_format_parse_digits(const char** ptr, const char* end_ptr, int* value);
static const char* text_format_parse_format(
    const char* start_ptr, const char* end_ptr, int* argpos, int* widthpos, int* flags, int* width);
static void text_format_string_conversion(
    StringInfo buf, char conversion, FmgrInfo* typOutputInfo, Datum value, bool isNull, int flags, int width);

static void text_format_append_string(StringInfo buf, const char* str, int flags, int width);

// adapt A db's substrb
static text* get_substring_really(Datum str, int32 start, int32 length, bool length_not_specified);
static text* get_result_of_concat(text* result, FunctionCallInfo fcinfo);
#ifdef DOLPHIN
static void check_blob_size(Datum blob, int64 max_size);
static int32 anybinary_typmodin(ArrayType* ta, const char* typname, uint32 max);
static char* anybinary_typmodout(int32 typmod);
static Datum copy_binary(Datum source, int typmod, bool target_is_var, bool can_ignore);
static bytea* copy_blob(bytea* source, int64 max_size);
static bool is_unsigned_intType(Oid oid);
static uint64 parse_unsigned_val(Oid typeoid, char* str_val);
static CmpType agg_cmp_type(FunctionCallInfo fcinfo, int argc);
static bytea* binary_type_and(PG_FUNCTION_ARGS);
static uint64 bit_and_text_uint8(const char* s, uint64 max, int64 min, const char* typname);

;
extern "C" DLL_PUBLIC Datum binary_typmodin(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum varbinary_typmodin(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum binary_typmodout(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bytea2binary(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bytea2var(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum tinyblob_rawin(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum dolphin_blob_rawout(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum mediumblob_rawin(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum longblob_rawin(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum tinyblob_recv(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum mediumblob_recv(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum longblob_recv(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum longblob2tinyblob(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum longblob2mediumblob(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum gs_interval(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum gs_strcmp(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bytea_left(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bytea_right(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum elt_integer(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum elt_string(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum elt_bit(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum field(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum find_in_set_integer(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum find_in_set_string(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum between_and(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum sym_between_and(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum not_between_and(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum not_sym_between_and(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum soundex(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum soundex_bool(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum soundex_bit(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum space_integer(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum space_string(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum space_bit(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum m_char(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum boolcharlen(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum text_insert(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum soundex_difference(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum make_set(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum uint1_list_agg_transfn(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum uint2_list_agg_transfn(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum uint4_list_agg_transfn(PG_FUNCTION_ARGS);
;
extern "C" DLL_PUBLIC Datum uint8_list_agg_transfn(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum uint1_list_agg_noarg2_transfn(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum uint2_list_agg_noarg2_transfn(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum uint4_list_agg_noarg2_transfn(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum uint8_list_agg_noarg2_transfn(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum int_to_hex(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum text_to_hex(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bytea_to_hex(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum bit_to_hex(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum db_b_format(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum db_b_format_locale(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum textxor(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum ord_text(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum ord_numeric(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum ord_bit(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index_bool_1(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index_bool_2(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index_2bool(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index_numeric(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum substring_index_text(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Float8(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Numeric(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Bpchar(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Varchar(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Text(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum Varlena2Bit(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum dolphin_binaryout(PG_FUNCTION_ARGS);

;
extern "C" DLL_PUBLIC Datum binary_varbinary(PG_FUNCTION_ARGS);

extern BpChar* bpchar_input(const char* s, size_t len, int32 atttypmod);
extern VarChar* varchar_input(const char* s, size_t len, int32 atttypmod);

#endif

/*****************************************************************************
 *	 CONVERSION ROUTINES EXPORTED FOR USE BY C CODE							 *
 *****************************************************************************/

#define TEXTISORANULL(t) ((t) == NULL || VARSIZE_ANY_EXHDR(t) == 0)

/*
 * cstring_to_text
 *
 * Create a text value from a null-terminated C string.
 *
 * The new text value is freshly palloc'd with a full-size VARHDR.
 */


/*
 * cstring_to_text_with_len
 *
 * Same as cstring_to_text except the caller specifies the string length;
 * the string need not be null_terminated.
 */






/*
 * text_to_cstring
 *
 * Create a palloc'd, null-terminated C string from a text value.
 *
 * We support being passed a compressed or toasted text value.
 * This is a bit bogus since such values shouldn't really be referred to as
 * "text *", but it seems useful for robustness.  If we didn't handle that
 * case here, we'd need another routine that did, anyway.
 */




/*
 * text_to_cstring_buffer
 *
 * Copy a text value into a caller-supplied buffer of size dst_len.
 *
 * The text string is truncated if necessary to fit.  The result is
 * guaranteed null-terminated (unless dst_len == 0).
 *
 * We support being passed a compressed or toasted text value.
 * This is a bit bogus since such values shouldn't really be referred to as
 * "text *", but it seems useful for robustness.  If we didn't handle that
 * case here, we'd need another routine that did, anyway.
 */


/*****************************************************************************
 *	 USER I/O ROUTINES														 *
 *****************************************************************************/
#define VAL(CH) ((CH) - '0')
#define DIG(VAL) ((VAL) + '0')

/*
 *		byteain			- converts from printable representation of byte array
 *
 *		Non-printable characters must be passed as '\nnn' (octal) and are
 *		converted to internal form.  '\' must be passed as '\\'.
 *		ereport(ERROR, ...) if bad form.
 *
 *		BUGS:
 *				The input is scanned twice.
 *				The error checking of input is minimal.
 */


#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum dolphin_binaryin(PG_FUNCTION_ARGS);

#endif
/*
 *		byteaout		- converts to printable representation of byte array
 *
 *		In the traditional escaped format, non-printable characters are
 *		printed as '\nnn' (octal) and '\' as '\\'.
 */


// input interface of RAW type




// output interface of RAW type


// Implements interface of rawtohex(text)


// Implements interface of hextoraw(raw)


/*
 *		bytearecv			- converts external binary format to bytea
 */


/*
 *		byteasend			- converts bytea to binary format
 *
 * This is a special case: just copy the input...
 */






#ifdef DOLPHIN
#else
#endif

/*
 *		textin			- converts "..." to internal representation
 */


/*
 *		textout			- converts internal representation to "..."
 */


/*
 *		textrecv			- converts external binary format to text
 */


/*
 *		textsend			- converts text to binary format
 */


/*
 *		unknownin			- converts "..." to internal representation
 */


/*
 *		unknownout			- converts internal representation to "..."
 */


/*
 *		unknownrecv			- converts external binary format to unknown
 */


/*
 *		unknownsend			- converts unknown to binary format
 */












/* ========== PUBLIC ROUTINES ========== */

/*
 * textlen -
 *	  returns the logical length of a text*
 *	   (which is less than the VARSIZE of the text*)
 */
#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN




int get_step_len(unsigned char ch)
{
    int step_len = 0;
    if (ch >= 0xFC && ch < 0xFE)
        step_len = 6;
    else if (ch >= 0xF8)
        step_len = 5;
    else if (ch >= 0xF0)
        step_len = 4;
    else if (ch >= 0xE0)
        step_len = 3;
    else if (ch >= 0xC0)
       step_len = 2;
    else if (0 == (ch & 0x80))
        step_len = 1;
    return step_len;
}
#endif

/*
 * text_length -
 *	Does the real work for textlen()
 *
 *	This is broken out so it can be called directly by other string processing
 *	functions.	Note that the argument is passed as a Datum, to indicate that
 *	it may still be in compressed form.  We can avoid decompressing it at all
 *	in some cases.
 */




/*
 * textoctetlen -
 *	  returns the physical length of a text*
 *	   (which is less than the VARSIZE of the text*)
 */


/*
 * textcat -
 *	  takes two text* and returns a text* that is the concatenation of
 *	  the two.
 *
 * Rewritten by Sapa, sapa@hq.icb.chel.su. 8-Jul-96.
 * Updated by Thomas, Thomas.Lockhart@jpl.nasa.gov 1997-07-10.
 * Allocate space for output in all cases.
 * XXX - thomas 1997-07-10
 */


/*
 * text_catenate
 *	Guts of textcat(), broken out so it can be used by other functions
 *
 * Arguments can be in short-header form, but not compressed or out-of-line
 */
#ifdef ENABLE_MULTIPLE_NODES
#endif

/*
 * charlen_to_bytelen()
 *	Compute the number of bytes occupied by n characters starting at *p
 *
 * It is caller's responsibility that there actually are n characters;
 * the string need not be null-terminated.
 */


/*
 * text_substr()
 * Return a substring starting at the specified position.
 * - thomas 1997-12-31
 *
 * Input:
 *	- string
 *	- starting position (is one-based)
 *	- string length
 *
 * If the starting position is zero or less, then return from the start of the string
 *	adjusting the length to be consistent with the "negative start" per SQL92.
 * If the length is less than zero, return the remaining string.
 *
 * Added multibyte support.
 * - Tatsuo Ishii 1998-4-21
 * Changed behavior if starting position is less than one to conform to SQL92 behavior.
 * Formerly returned the entire string; now returns a portion.
 * - Thomas Lockhart 1998-12-10
 * Now uses faster TOAST-slicing interface
 * - John Gray 2002-02-22
 * Remove "#ifdef MULTIBYTE" and test for encoding_max_length instead. Change
 * behaviors conflicting with SQL92 to meet SQL92 (if E = S + L < S throw
 * error; if E < 1, return '', not entire string). Fixed MB related bug when
 * S > LC and < LC + 4 sometimes garbage characters are returned.
 * - Joe Conway 2002-08-10
 */


/*
 * text_substr_null's function is same to text_substr, only with different return empty values.
 * when return value is a empty values, then return NULL to adapt NULL test.
 */
#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN
#else
#endif
/*
 * text_substr_no_len -
 *	  Wrapper to avoid opr_sanity failure due to
 *	  one function accepting a different number of args.
 */


/*
 * text_substring -
 *	Does the real work for text_substr() and text_substr_no_len()
 *
 *	This is broken out so it can be called directly by other string processing
 *	functions.	Note that the argument is passed as a Datum, to indicate that
 *	it may still be in compressed/toasted form.  We can avoid detoasting all
 *	of it in some cases.
 *
 *	The result is always a freshly palloc'd datum.
 */




// adapt A db's substr(text str,integer start,integer length)
// when start<0, amend the sartPosition to abs(start) from last char,
// then search backward


// adapt A db's substr(text str,integer start)
// when start<0, amend the sartPosition to abs(start) from last char,
// then search backward

#ifdef DOLPHIN
/*
 * text_insert
 */





#endif

/*
 * textoverlay
 *	Replace specified substring of first string with second
 *
 * The SQL standard defines OVERLAY() in terms of substring and concatenation.
 * This code is a direct implementation of what the standard says.
 */






/*
 * textpos -
 *	  Return the position of the specified substring.
 *	  Implements the SQL92 POSITION() function.
 *	  Ref: A Guide To The SQL Standard, Date & Darwen, 1997
 * - thomas 1997-07-27
 */


/*
 * text_position -
 *	Does the real work for textpos()
 *
 * Inputs:
 *		t1 - string to be searched
 *		t2 - pattern to match within t1
 * Result:
 *		Character index of the first matched char, starting from 1,
 *		or 0 if no match.
 *
 *	This is broken out so it can be called directly by other string processing
 *	functions.
 */


/*
 * text_position_setup, text_position_next, text_position_cleanup -
 *	Component steps of text_position()
 *
 * These are broken out so that a string can be efficiently searched for
 * multiple occurrences of the same pattern.  text_position_next may be
 * called multiple times with increasing values of start_pos, which is
 * the 1-based character position to start the search from.  The "state"
 * variable is normally just a local variable in the caller.
 */







/* varstr_cmp()
 * Comparison function for text strings with given lengths.
 * Includes locale support, but must copy strings to temporary memory
 *	to allow null-termination for inputs to strcoll().
 * Returns an integer less than, equal to, or greater than zero, indicating
 * whether arg1 is less than, equal to, or greater than arg2.
 */
#ifdef HAVE_LOCALE_T
#endif
#ifdef HAVE_LOCALE_T
#endif
#ifdef WIN32
#ifdef HAVE_LOCALE_T
#endif
#endif /* WIN32 */
#ifdef HAVE_LOCALE_T
#endif

/* text_cmp()
 * Internal comparison function for text strings.
 * Returns -1, 0 or 1
 */
#ifdef DOLPHIN
#else
#endif



/*
 * Comparison functions for text strings.
 *
 * Note: btree indexes need these routines not to leak memory; therefore,
 * be careful to free working copies of toasted datums.  Most places don't
 * need to be so careful.
 */

#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN
#else
#endif

/*
 * The internal realization of function vtextne.
 */
#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#else
#endif











#ifdef DOLPHIN
#else
#endif

/*
 * Generic sortsupport interface for character type's operator classes.
 * Includes locale support, and support for BpChar semantics (i.e. removing
 * trailing spaces before comparison).
 *
 * Relies on the assumption that text, VarChar, BpChar, and bytea all have the
 * same representation.  Callers that always use the C collation (e.g.
 * non-collatable type callers like bytea) may have NUL bytes in their strings;
 * this will not work with any other collation, though.
 */
#ifdef HAVE_LOCALE_T
#endif
#ifdef WIN32
#endif
#ifdef HAVE_LOCALE_T
#endif
#ifdef HAVE_LOCALE_T
#endif

/*
 * sortsupport comparison func (for C locale case)
 */


/*
 * sortsupport comparison func (for C locale case)
 * for characher(n)
 */


/*
 * sortsupport comparison func (for builtin locale case)
 */



/*
 * sortsupport comparison func (for locale case)
 */
#ifdef HAVE_LOCALE_T
#endif

/*
 * Abbreviated key comparison func
 */


/*
 * Conversion routine for sortsupport.  Converts original to abbreviated key
 * representation.  Our encoding strategy is simple -- pack the first 8 bytes
 * of a strxfrm() blob into a Datum (on little-endian machines, the 8 bytes are
 * stored in reverse order), and treat it as an unsigned integer.  When the "C"
 * locale is used, or in case of bytea, just memcpy() from original instead.
 */
#ifdef HAVE_LOCALE_T
#endif
#if SIZEOF_DATUM == 8
#else /* SIZEOF_DATUM != 8 */
#endif

/*
 * Callback for estimating effectiveness of abbreviated key optimization, using
 * heuristic rules.  Returns value indicating if the abbreviation optimization
 * should be aborted, based on its projected effectiveness.
 */
#ifdef TRACE_SORT
#endif
#ifdef TRACE_SORT
#endif
#ifdef TRACE_SORT
#endif





/*
 * The following operators support character-by-character comparison
 * of text datums, to allow building indexes suitable for LIKE clauses.
 * Note that the regular texteq/textne comparison operators are assumed
 * to be compatible with these!
 */

#ifdef DOLPHIN
#else
#endif











/* -------------------------------------------------------------
 * byteaoctetlen
 *
 * get the number of bytes contained in an instance of type 'bytea'
 * -------------------------------------------------------------
 */


/*
 * byteacat -
 *	  takes two bytea* and returns a bytea* that is the concatenation of
 *	  the two.
 *
 * Cloned from textcat and modified as required.
 */


/*
 * bytea_catenate
 *	Guts of byteacat(), broken out so it can be used by other functions
 *
 * Arguments can be in short-header form, but not compressed or out-of-line
 */
#ifdef ENABLE_MULTIPLE_NODES
#endif

#define PG_STR_GET_BYTEA(str_) DatumGetByteaP(DirectFunctionCall1(byteain, CStringGetDatum(str_)))

/*
 * bytea_substr()
 * Return a substring starting at the specified position.
 * Cloned from text_substr and modified as required.
 *
 * Input:
 *	- string
 *	- starting position (is one-based)
 *	- string length (optional)
 *
 * If the starting position is zero or less, then return from the start of the string
 * adjusting the length to be consistent with the "negative start" per SQL92.
 * If the length is less than zero, an ERROR is thrown. If no third argument
 * (length) is provided, the length to the end of the string is assumed.
 */


/*
 * bytea_substr_no_len -
 *	  Wrapper to avoid opr_sanity failure due to
 *	  one function accepting a different number of args.
 */




// adapt A db's substr(bytea str,integer start,integer length)
// when start<0, amend the sartPosition to abs(start) from last char,
// then search backward
#ifdef DOLPHIN
#else
#endif

// adapt A db's substr(bytea x,integer y)
// when start<0, amend the sartPosition to abs(start) from last char,
// then search backward
#ifdef DOLPHIN
#endif

// Does the real work for bytea_substr_orclcompat() and bytea_substr_no_len_orclcompat().
// The result is always a freshly palloc'd datum.
// when length_not_specified==false, the param length is not used,
// usually called by text_substr_no_len_orclcompat().
// when length_not_specified==true, the param length is not used,
// sually called by text_substr_orclcompat()
#ifndef DOLPHIN
#endif

/*
 * byteaoverlay
 *	Replace specified substring of first string with second
 *
 * The SQL standard defines OVERLAY() in terms of substring and concatenation.
 * This code is a direct implementation of what the standard says.
 */






/*
 * byteapos -
 *	  Return the position of the specified substring.
 *	  Implements the SQL92 POSITION() function.
 * Cloned from textpos and modified as required.
 */


/* -------------------------------------------------------------
 * byteaGetByte
 *
 * this routine treats "bytea" as an array of bytes.
 * It returns the Nth byte (a number between 0 and 255).
 * -------------------------------------------------------------
 */


/* -------------------------------------------------------------
 * byteaGetBit
 *
 * This routine treats a "bytea" type like an array of bits.
 * It returns the value of the Nth bit (0 or 1).
 *
 * -------------------------------------------------------------
 */


/* -------------------------------------------------------------
 * byteaSetByte
 *
 * Given an instance of type 'bytea' creates a new one with
 * the Nth byte set to the given value.
 *
 * -------------------------------------------------------------
 */


/* -------------------------------------------------------------
 * byteaSetBit
 *
 * Given an instance of type 'bytea' creates a new one with
 * the Nth bit set to the given value.
 *
 * -------------------------------------------------------------
 */


#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum dolphin_attname_eq(PG_FUNCTION_ARGS);

#endif

/* text_name()
 * Converts a text type to a Name type.
 */


/* name_text()
 * Converts a Name type to a text type.
 */


/*
 * textToQualifiedNameList - convert a text object to list of names
 *
 * This implements the input parsing needed by nextval() and other
 * functions that take a text parameter representing a qualified name.
 * We split the name at dots, downcase if not double-quoted, and
 * truncate names if they're too long.
 */


/*
 * SplitIdentifierString --- parse a string containing identifiers
 *
 * This is the guts of textToQualifiedNameList, and is exported for use in
 * other situations such as parsing GUC variables.	In the GUC case, it's
 * important to avoid memory leaks, so the API is designed to minimize the
 * amount of stuff that needs to be allocated and freed.
 *
 * Inputs:
 *	rawstring: the input string; must be overwritable!	On return, it's
 *			   been modified to contain the separated identifiers.
 *	separator: the separator punctuation expected between identifiers
 *			   (typically '.' or ',').	Whitespace may also appear around
 *			   identifiers.
 * Outputs:
 *	namelist: filled with a palloc'd list of pointers to identifiers within
 *			  rawstring.  Caller should list_free_ext() this even on error return.
 *
 * Returns TRUE if okay, FALSE if there is a syntax error in the string.
 *
 * Note that an empty string is considered okay here, though not in
 * textToQualifiedNameList.
 */
bool SplitIdentifierString(char* rawstring, char separator, List** namelist, bool downCase, bool truncateToolong)
{
    char* nextp = rawstring;
    bool done = false;
    errno_t ss_rc = 0;

    *namelist = NIL;

    while (isspace((unsigned char)*nextp))
        nextp++; /* skip leading whitespace */

    if (*nextp == '\0')
        return true; /* allow empty string */

    /* At the top of the loop, we are at start of a new identifier. */
    char* curname = NULL;
    char* endp = NULL;
    char* downname = NULL;
#ifdef DOLPHIN
    char quote = GET_QUOTE();
    do {
        if (*nextp == quote) {
            /* Quoted name --- collapse quote-quote pairs, no downcasing */
            curname = nextp + 1;
            for (;;) {
                endp = strchr(nextp + 1, quote);
                if (endp == NULL)
                    return false; /* mismatched quotes */
                if (endp[1] != quote)
#else
    do {
        if (*nextp == '\"') {
            /* Quoted name --- collapse quote-quote pairs, no downcasing */
            curname = nextp + 1;
            for (;;) {
                endp = strchr(nextp + 1, '\"');
                if (endp == NULL)
                    return false; /* mismatched quotes */
                if (endp[1] != '\"')
#endif
                    break; /* found end of quoted name */
                /* Collapse adjacent quotes into one quote, and look again */
                if (strlen(endp) > 0) {
                    ss_rc = memmove_s(endp, strlen(endp), endp + 1, strlen(endp));
                    securec_check(ss_rc, "\0", "\0");
                }
                nextp = endp;
            }
            /* endp now points at the terminating quote */
            nextp = endp + 1;
        } else {
            /* Unquoted name --- extends to separator or whitespace */
            int len;

            curname = nextp;
            while (*nextp && *nextp != separator && !isspace((unsigned char)*nextp))
                nextp++;
            endp = nextp;
            if (curname == nextp)
                return false; /* empty unquoted name not allowed */

            /*
             * Downcase the identifier, using same code as main lexer does.
             *
             * XXX because we want to overwrite the input in-place, we cannot
             * support a downcasing transformation that increases the string
             * length.	This is not a problem given the current implementation
             * of downcase_truncate_identifier, but we'll probably have to do
             * something about this someday.
             */
            len = endp - curname;

            /*
             * If downCase is false need not to convert to lowercase when this function is called by
             * get_typeoid_with_namespace, because this curname is come from datanode's system table, if we will it
             * alter to lower that will this schema or columnName can not be found.
             */
            if (downCase) {
                downname = downcase_truncate_identifier(curname, len, false);

                Assert(strlen(downname) <= (unsigned int)(len));
                strncpy(curname, downname, len);
                pfree_ext(downname);
            }
        }

        while (isspace((unsigned char)*nextp))
            nextp++; /* skip trailing whitespace */

        if (*nextp == separator) {
            nextp++;
            while (isspace((unsigned char)*nextp))
                nextp++; /* skip leading whitespace for next */
                         /* we expect another name, so done remains false */
        } else if (*nextp == '\0')
            done = true;
        else
            return false; /* invalid syntax */

        /* Now safe to overwrite separator with a null */
        *endp = '\0';
        if (truncateToolong == true) {
            /* Truncate name if it's overlength */
            truncate_identifier(curname, strlen(curname), false);
        }
        /*
         * Finished isolating current name --- add it to list
         */
        *namelist = lappend(*namelist, curname);

        /* Loop back if we didn't reach end of string */
    } while (!done);

    return true;
}



/*****************************************************************************
 *	Comparison Functions used for bytea
 *
 * Note: btree indexes need these routines not to leak memory; therefore,
 * be careful to free working copies of toasted datums.  Most places don't
 * need to be so careful.
 *****************************************************************************/

































/*
 * appendStringInfoText
 *
 * Append a text to str.
 * Like appendStringInfoString(str, text_to_cstring(t)) but faster.
 */


/*
 * replace_text_with_two_args
 * replace all occurrences of 'old_sub_str' in 'orig_str'
 * with '' to form 'new_str'
 *
 * the effect is equivalent to
 *
 * delete all occurrences of 'old_sub_str' in 'orig_str'
 * to form 'new_str'
 *
 * returns 'orig_str' if 'old_sub_str' == '' or 'orig_str' == ''
 * otherwise returns 'new_str'
 */
#ifdef DOLPHIN
#else
#endif

/*
 * replace_text
 * replace all occurrences of 'old_sub_str' in 'orig_str'
 * with 'new_sub_str' to form 'new_str'
 *
 * returns 'orig_str' if 'old_sub_str' == '' or 'orig_str' == ''
 * otherwise returns 'new_str'
 */
#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#endif

/*
 * check_replace_text_has_escape_char
 *
 * check whether replace_text contains escape char.
 */


/*
 * appendStringInfoRegexpSubstr
 *
 * Append replace_text to str, substituting regexp back references for
 * \n escapes.	start_ptr is the start of the match in the source string,
 * at logical character position data_pos.
 */


#define REGEXP_REPLACE_BACKREF_CNT 10

/*
 * replace_text_regexp
 *
 * replace text that matches to regexp in src_text to replace_text.
 *
 * Note: to avoid having to include regex.h in builtins.h, we declare
 * the regexp argument as void *, but really it's regex_t *.
 * occur : the n-th matched occurrence, start from 1.
 */


/*
 * split_text
 * parse input string
 * return ord item (1 based)
 * based on provided field separator
 */


/*
 * Convenience function to return true when two text params are equal.
 */


/*
 * text_to_array
 * parse input string and return text array of elements,
 * based on provided field separator
 */


/*
 * text_to_array_null
 * parse input string and return text array of elements,
 * based on provided field separator and null string
 *
 * This is a separate entry point only to prevent the regression tests from
 * complaining about different argument sets for the same internal function.
 */


/*
 * common code for text_to_array and text_to_array_null functions
 *
 * These are not strict so we have to test for null inputs explicitly.
 */


/*
 * array_to_text
 * concatenate Cstring representation of input array elements
 * using provided field separator
 */


/*
 * array_to_text_null
 * concatenate Cstring representation of input array elements
 * using provided field separator and null string
 *
 * This version is not strict so we have to test for null inputs explicitly.
 */


/*
 * common code for array_to_text and array_to_text_null functions
 */


#define HEXBASE 16
#ifdef DOLPHIN
#define HEX_CHARS "0123456789ABCDEF"
#else
#define HEX_CHARS "0123456789abcdef"
#endif
/*
 * Convert a int32 to a string containing a base 16 (hex) representation of
 * the number.
 */


/*
 * Convert a int64 to a string containing a base 16 (hex) representation of
 * the number.
 */

#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum uint1_to_hex(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum uint2_to_hex(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum uint4_to_hex(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum uint8_to_hex(PG_FUNCTION_ARGS);


#define HEX_BUF_LEN 32
/*
 * Convert an int128 to a string containing a base 16 (hex) representation of the input.
 * The input parameter must be integer.
 */







#endif
/*
 * Create an md5 hash of a text string and return it as hex
 *
 * md5 produces a 16 byte (128 bit) hash; double it for hex
 */
#define MD5_HASH_LEN 32



/*
 * Create an md5 hash of a bytea field and return it as a hex string:
 * 16-byte md5 digest is represented in 32 hex characters.
 */


/*
 * Create an sha/sha1/sha2 hash of a text string and return it as hex
 */

/* SHA-1 Various Length Definitions */
#define SHA1_ALG_TYPE 160
#define SHA1_DIGEST_LENGTH 20

/* SHA-224 / SHA-256 /SHA-384 / SHA-512 Various Length Definitions */
#define SHA224_ALG_TYPE 224
#define SHA224_DIGEST_LENGTH 28
#define SHA256_ALG_TYPE 256
#define SHA256_DIGEST_LENGTH 32
#define SHA384_ALG_TYPE 384
#define SHA384_DIGEST_LENGTH 48
#define SHA512_ALG_TYPE 512
#define SHA512_DIGEST_LENGTH 64

#define SHA_ERROR_LENGTH (GS_UINT32)(-1)



/* Converts binary character streams to hexadecimal return */


/* get_hash_digest_length: choose the unsigned char sha length by secure hash algorithm */


/* get_evp_md: choose EVP_MD value by secure hash algorithm */


/* function to generate sha /sha1 /sha2 */


/*
 * Create an sha1 hash of a bytea field and return it as a hex string:
 * 20-byte sha1 digest is represented in 40 hex characters.
 */
#ifdef DOLPHIN
#else
#endif

/*
 * the calculation of sha and sha1 is the same.
 */



/*
 * Create an sha2 hash of a bytea field and return it as a hex string:
 * 28-byte sha224 digest is represented in 56 hex characters.
 * 32-byte sha256 or sha0 digest is represented in 64 hex characters.
 * 48-byte sha384 digest is represented in 96s hex characters.
 * 64-byte sha512 digest is represented in 96s hex characters.
 */







#define b64_from_24bit(B2, B1, B0, N)                                                                                  \
    {                                                                                                                  \
        uint32_t w = ((B2) << 16) | ((B1) << 8) | (B0);                                                                \
        int      n = (N);                                                                                              \
        while (--n >= 0 && ctbufflen > 0)                                                                              \
        {                                                                                                              \
            *p++ = b64t[w & 0x3f];                                                                                     \
            w >>= 6;                                                                                                   \
            ctbufflen--;                                                                                               \
        }                                                                                                              \
    }

#define ROUNDS    "rounds="
#define ROUNDSLEN (sizeof(ROUNDS) - 1)



#define MIN(A, B) ((B) < (A) ? (B) : (A))
#define MAX(A, B) ((B) > (A) ? (B) : (A))



/*
 * Return the size of a datum, possibly compressed
 *
 * Works on any data type
 */


/*
 * @Description: This function is used to calculate the size of a datum
 *
 * @IN PG_FUNCTION_ARGS: any data type
 * @return: the byte size of the data
 */


/*
 * string_agg - Concatenates values and returns string.
 *
 * Syntax: string_agg(value text, delimiter text) RETURNS text
 *
 * Note: Any NULL values are ignored. The first-call delimiter isn't
 * actually used at all, and on subsequent calls the delimiter precedes
 * the associated value.
 */

/* subroutine to initialize state */






/*
 * checksumtext_agg_transfn - sum the oldsumofhashvalue and the value of text input and returns numeric.
 *
 * Syntax: checksumtext_agg_transfn(numeric, text) RETURNS numeric
 *
 */








#ifdef DOLPHIN
;
;
extern "C" DLL_PUBLIC Datum int1_list_agg_transfn(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum int1_list_agg_noarg2_transfn(PG_FUNCTION_ARGS);




#endif











































/*
 * Implementation of both concat() and concat_ws().
 *
 * sepstr/seplen describe the separator.  argidx is the first argument
 * to concatenate (counting from zero).
 */
#ifdef DOLPHIN
#else
#endif



/*
 * Concatenate all arguments with separator. NULL arguments are ignored.
 */

/*
 * Concatenate all arguments with separator. NULL arguments are ignored.
 */


/*
 * Concatenate all arguments. NULL arguments are ignored.
 */


/*
 * Concatenate all but first argument value with separators. The first
 * parameter is used as the separator. NULL arguments are ignored.
 */


#ifdef DOLPHIN
#else
#endif
#ifdef DOLPHIN
#else
#endif

/*
 * Return first n characters in the string. When n is negative,
 * return all but last |n| characters.
 */


#ifdef DOLPHIN


;
extern "C" DLL_PUBLIC Datum text_left_numeric(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum text_left_text(PG_FUNCTION_ARGS);

#endif

/*
 * Return last n characters in the string. When n is negative,
 * return all but first |n| characters.
 */


#ifdef DOLPHIN
;
extern "C" DLL_PUBLIC Datum text_right_numeric(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum text_right_text(PG_FUNCTION_ARGS);

#endif

/*
 * Return reversed string
 */


/*
 * Support macros for text_format()
 */
#define TEXT_FORMAT_FLAG_MINUS 0x0001 /* is minus flag present? */

#define ADVANCE_PARSE_POINTER(ptr, end_ptr)                                                                          \
    do {                                                                                                             \
        if (++(ptr) >= (end_ptr))                                                                                    \
            ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE), errmsg("unterminated conversion specifier"))); \
    } while (0)

#ifdef DOLPHIN
#define MAX_PRECISION 32
#define THOUS_INTERVAL 3
#define ASCII_9 57
#define ASCII_5 53
#define ASCII_0 48
#define ASCII_ADD 43
#define ASCII_MINUS 45
#define ASCII_DOT 46



/*
 * Format input float8 number keeping specific number of decimal in specific locale style
 * e.g. db_b_format_transfer(1234.456, 2 ,'en_US') -> 1,234.46
 *      db_b_format_transfer(1234.456, 5 ,'de_DE') -> 1.234,45600
 */

#endif
/*
 * Returns a formated string
 */

#ifdef DOLPHIN
/*
 * Check whether a cstring can transform to float8
 */




/*
 * Transform an any type input into char*. If the input is not a valid float number, or cannot be transformed to
 * a valid float number, set it to 0
 */





#endif

/*
 * Parse contiguous digits as a decimal number.
 *
 * Returns true if some digits could be parsed.
 * The value is returned into *value, and *ptr is advanced to the next
 * character to be parsed.
 *
 * Note parsing invariant: at least one character is known available before
 * string end (end_ptr) at entry, and this is still true at exit.
 */


/*
 * Parse a format specifier (generally following the SUS printf spec).
 *
 * We have already advanced over the initial '%', and we are looking for
 * [argpos][flags][width]type (but the type character is not consumed here).
 *
 * Inputs are start_ptr (the position after '%') and end_ptr (string end + 1).
 * Output parameters:
 *	argpos: argument position for value to be printed.  -1 means unspecified.
 *	widthpos: argument position for width.  Zero means the argument position
 *			was unspecified (ie, take the next arg) and -1 means no width
 *			argument (width was omitted or specified as a constant).
 *	flags: bitmask of flags.
 *	width: directly-specified width value.  Zero means the width was omitted
 *			(note it's not necessary to distinguish this case from an explicit
 *			zero width value).
 *
 * The function result is the next character position to be parsed, ie, the
 * location where the type character is/should be.
 *
 * Note parsing invariant: at least one character is known available before
 * string end (end_ptr) at entry, and this is still true at exit.
 */


/*
 * Format a %s, %I, or %L conversion
 */


/*
 * Append str to buf, padding as directed by flags/width
 */


/*
 * text_format_nv - nonvariadic wrapper for text_format function.
 *
 * note: this wrapper is necessary to pass the sanity check in opr_sanity,
 * which checks that all built-in functions that share the implementing C
 * function take the same number of arguments.
 */


// scan the source string forward


// scan the source string backward


// start from beginIndex, find the first position of textStrToSearch in textStr


// search from beginIndex,return the position of textStrToSearch when finding occurTimes in textStr


// instr(varchar string, varchar string_to_search, integer beg_index)
// start from position beg_index, get the index of the first match of string_to_search in string
// character sets considered, different character sets, different characters in the number of bytes
// for example,a Chinese character stored in three bytes



// adapt A db's empty_blob ()


// adapt A db's substrb(text str,integer start,integer length)


// adapt A db's substr(text str,integer start)






/*
 * This function does the real work for substrb_with_lenth() and substrb_without_lenth().
 */










#ifdef DOLPHIN








/* common code for bpchartypmodout and varchartypmodout */














#ifdef DOLPHIN
#else
#endif



#ifdef DOLPHIN
#else
#endif

#ifdef DOLPHIN
#else
#endif











// return the first non ' ' index or the '\0' index


// truncate a cstring to be a valid numeric cstring(could be 0 length cstring)


//Get float8 value for parameters of gs_interval














;
extern "C" DLL_PUBLIC Datum bytea_right_numeric(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum blob_right_numeric(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bytea_left_numeric(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum blob_left_numeric(PG_FUNCTION_ARGS);






#define storeBytes1(BYTES, T, A) \
    if (1 == BYTES) { \
        T[0] = (char)A; \
    }
#define storeBytes2(BYTES, T, A) \
    if (2 == BYTES) { \
        unsigned int tmpValue = (unsigned int) (A) ; \
        ((unsigned char*) (T))[1] = (unsigned char)(tmpValue); \
        ((unsigned char*) (T))[0] = (unsigned char)(tmpValue >> 8); \
    }
#define storeBytes3(BYTES, T, A) \
    if (3 == BYTES) { \
        unsigned long int tmpValue = (unsigned long int) (A); \
        ((unsigned char*) (T))[2] = (unsigned char) (tmpValue); \
        ((unsigned char*) (T))[1] = (unsigned char) (tmpValue >> 8); \
        ((unsigned char*) (T))[0] = (unsigned char) (tmpValue >> 16); \
    }
#define storeBytes4(BYTES, T, A) \
    if (4 == BYTES) { \
        unsigned long int tmpValue = (unsigned long int) (A); \
        ((unsigned char*) (T))[3] = (unsigned char) (tmpValue); \
        ((unsigned char*) (T))[2] = (unsigned char) (tmpValue >> 8); \
        ((unsigned char*) (T))[1] = (unsigned char) (tmpValue >> 16); \
        ((unsigned char*) (T))[0] = (unsigned char) (tmpValue >> 24); \
    }

























/**
  Convert pg_tm value to cstring in YYYYMMDDHHmmSS/YYYYMMDD/HHmmSS format
  @param tm  The pg_tm value to convert.
  @param type The oid of the type of timestamp/date/time
  @return         A cstring in format YYYYMMDDHHmmSS/YYYYMMDD/HHmmSS.
*/


















/*
 * Soundex
 */





static void set_sound(const char* arg, char* result, int size);

/* ABCDEFGHIJKLMNOPQRSTUVWXYZ */

















extern "C" Datum uint8in(PG_FUNCTION_ARGS);
extern "C" DLL_PUBLIC Datum bittouint8(PG_FUNCTION_ARGS);

enum CmpMode {BETWEEN_AND, SYMMETRIC_BETWEEN_AND, NOT_BETWEEN_AND, NOT_SYMMETRIC_BETWEEN_AND};





static CmpType agg_cmp_type(FunctionCallInfo fcinfo, int argc)
{
    bool unsigned_flag = false;
    CmpType agg_type = map_oid_to_cmp_type(fcinfo->argTypes[0], &unsigned_flag);
    for (int i = 1; i < argc; i++) {
        agg_type = agg_cmp_type(agg_type, map_oid_to_cmp_type(fcinfo->argTypes[i], &unsigned_flag));
    }
    return agg_type;
}
























































;
extern "C" DLL_PUBLIC Datum blobxor(PG_FUNCTION_ARGS);



;
extern "C" DLL_PUBLIC Datum blob_xor_blob(PG_FUNCTION_ARGS);





;
extern "C" DLL_PUBLIC Datum bloband(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum binaryand(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinaryand(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinary_and_binary(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinary_and_tinyblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinary_and_blob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinary_and_mediumblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varbinary_and_longblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum text_and_uint8(PG_FUNCTION_ARGS);




;
extern "C" DLL_PUBLIC Datum boolxor(PG_FUNCTION_ARGS);



/*
 * Return the code for the leftmost character if that character 
 * is a multi-byte (sequence of one or more bytes) one
 */


/*
 * Return the code for the leftmost character if that character 
 * is a multi-byte (sequence of one or more bytes) one
 */








/*
 * function for dolphin--1.0
 */


/*
 * function for dolphin--1.0
 */


/*
 * function for dolphin--1.0
 */


/*
 * function for dolphin--1.0
 */


/*
 * function for dolphin--1.1
 */


/*
 * function for dolphin--1.1
 */


;
extern "C" DLL_PUBLIC Datum text_any_value(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bytea_any_value(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum blob_any_value(PG_FUNCTION_ARGS);


















;
extern "C" DLL_PUBLIC Datum bittotinyblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittomediumblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittoblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bittolongblob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum uint8_xor_text(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum random_bytes(PG_FUNCTION_ARGS);
#define MAX_RANDOM_BYTES_LEN 1024


;
extern "C" DLL_PUBLIC Datum rand_seed(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum binary_length(PG_FUNCTION_ARGS);


/*
 * The binaryout need to handle some unprintable chars like chinese.
 * The unprintable chars like chinese cannot be restored at printtup when we use byteaout function to
 * handle the binary array, but mysql can
 * so wo just need to handle the \0 char, for other char just need to keep same with input array here
 */




;
extern "C" DLL_PUBLIC Datum bit_left(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum blob_left(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum concat_blob(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum instr_bit(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum instr_binary(PG_FUNCTION_ARGS);




;
extern "C" DLL_PUBLIC Datum varlena_larger(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum varlena_smaller(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bit_substr(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum bit_substr_no_len(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum blob_to_float8(PG_FUNCTION_ARGS);


;
extern "C" DLL_PUBLIC Datum text_to_blob(PG_FUNCTION_ARGS);


/**
 * convert text to bit.
*/
;
extern "C" DLL_PUBLIC Datum text_to_bit(PG_FUNCTION_ARGS);

#endif
