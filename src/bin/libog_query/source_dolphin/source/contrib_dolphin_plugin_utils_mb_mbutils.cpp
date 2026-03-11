/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - GetDatabaseEncoding
 * - pg_get_client_encoding
 * - pg_mblen
 * - pg_mbstrlen_with_len
 * - pg_mbcliplen
 * - pg_encoding_mbcliplen
 * - cliplen
 * - pg_server_to_client
 * - pg_server_to_any
 * - try_fast_encoding_conversion
 * - fast_encoding_conversion
 * - pg_do_encoding_conversion
 * - NoNeedToConvert
 * - perform_default_encoding_conversion
 * - GetCharsetConnection
 * - pg_mbstrlen_with_len
 *--------------------------------------------------------------------
 */

/*
 * This file contains public functions for conversion between
 * client encoding and server (database) encoding.
 *
 * Tatsuo Ishii
 *
 * src/backend/utils/mb/mbutils.c
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/xact.h"
#include "catalog/namespace.h"
#include "plugin_mb/pg_wchar.h"
#include "pgxc/execRemote.h"
#include "plugin_postgres.h"
#include "utils/builtins.h"
#include "utils/memutils.h"
#include "utils/syscache.h"
#include "storage/ipc.h"
#include "executor/executor.h"
#include "plugin_commands/mysqlmode.h"
/*
 * We maintain a simple linked list caching the fmgr lookup info for the
 * currently selected conversion functions, as well as any that have been
 * selected previously in the current session.	(We remember previous
 * settings because we must be able to restore a previous setting during
 * transaction rollback, without doing any fresh catalog accesses.)
 *
 * Since we'll never release this data, we just keep it in t_thrd.top_mem_cxt.
 */
typedef struct ConvProcInfo {
    int s_encoding; /* server and client encoding IDs */
    int c_encoding;
    FmgrInfo to_server_info; /* lookup info for conversion procs */
    FmgrInfo to_client_info;
} ConvProcInfo;

/* Internal functions */
static char* perform_default_encoding_conversion(const char* src, int len, bool is_client_to_server);
static int cliplen(const char* str, int len, int limit);

// Determine whether the current case needs to be converted
bool NoNeedToConvert(int srcEncoding, int destEncoding)
{
    if (srcEncoding == destEncoding) {
        return true;
    }
    if (srcEncoding == PG_SQL_ASCII || destEncoding == PG_SQL_ASCII) {
        return true;
    }
    if (srcEncoding == PG_GB18030_2022 && destEncoding == PG_GB18030) {
        return true;
    }
    if (srcEncoding == PG_GB18030 && destEncoding == PG_GB18030_2022) {
        return true;
    }
    return false;
}

;
extern "C" DLL_PUBLIC Datum pg_convert_to_text(PG_FUNCTION_ARGS);

/*
 * Prepare for a future call to SetClientEncoding.	Success should mean
 * that SetClientEncoding is guaranteed to succeed for this encoding request.
 *
 * (But note that success before u_sess->mb_cxt.backend_startup_complete does not guarantee
 * success after ...)
 *
 * Returns 0 if okay, -1 if not (bad encoding or can't support conversion)
 */


/*
 * Set the active client encoding and set up the conversion-function pointers.
 * PrepareClientEncoding should have been called previously for this encoding.
 *
 * Returns 0 if okay, -1 if not (bad encoding or can't support conversion)
 */


/*
 * Initialize client encoding conversions.
 *		Called from InitPostgres() once during backend startup.
 */


/*
 * returns the current client encoding
 */
int pg_get_client_encoding(void)
{
    Assert(u_sess->mb_cxt.ClientEncoding);
    return u_sess->mb_cxt.ClientEncoding->encoding;
}

/*
 * returns the current client encoding name
 */


/*
 * Apply encoding conversion on src and return it. The encoding
 * conversion function is chosen from the pg_conversion system catalog
 * marked as "default". If it is not found in the schema search path,
 * it's taken from pg_catalog schema. If it even is not in the schema,
 * warn and return src.
 *
 * If conversion occurs, a palloc'd null-terminated string is returned.
 * In the case of no conversion, src is returned.
 *
 * CAUTION: although the presence of a length argument means that callers
 * can pass non-null-terminated strings, care is required because the same
 * string will be passed back if no conversion occurs.	Such callers *must*
 * check whether result == src and handle that case differently.
 *
 * Note: we try to avoid raising error, since that could get us into
 * infinite recursion when this function is invoked during error message
 * sending.  It should be OK to raise error for overlength strings though,
 * since the recursion will come with a shorter message.
 */
unsigned char* pg_do_encoding_conversion(unsigned char* src, int len, int src_encoding, int dest_encoding)
{
    unsigned char* result = NULL;
    Oid proc;

    if (!IsTransactionState()) {
        return src;
    }
    if (NoNeedToConvert(src_encoding, dest_encoding)) {
        return src;
    }
    if (len <= 0) {
        return src;
    }
    proc = FindDefaultConversionProc(src_encoding, dest_encoding);
    if (!OidIsValid(proc)) {
        ereport(DEBUG2,
            (errcode(ERRCODE_UNDEFINED_FUNCTION),
                errmsg("default conversion function for encoding \"%s\" to \"%s\" does not exist",
                    pg_encoding_to_char(src_encoding),
                    pg_encoding_to_char(dest_encoding))));
        return src;
    }

    /*
     * XXX we should avoid throwing errors in OidFunctionCall. Otherwise we
     * are going into infinite loop!  So we have to make sure that the
     * function exists before calling OidFunctionCall.
     */
    if (!SearchSysCacheExists1(PROCOID, ObjectIdGetDatum(proc))) {
        ereport(LOG, (errmsg("cache lookup failed for function %u", proc)));
        return src;
    }

    /*
     * Allocate space for conversion result, being wary of integer overflow
     */
    if ((Size)len >= (MaxAllocSize / (Size)MAX_CONVERSION_GROWTH)) {
        ereport(ERROR,
            (errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
                errmsg("out of memory"),
                errdetail("String of %d bytes is too long for encoding conversion.", len)));
    }
    result = (unsigned char*)palloc(len * MAX_CONVERSION_GROWTH + 1);

    OidFunctionCall5(proc,
        Int32GetDatum(src_encoding),
        Int32GetDatum(dest_encoding),
        CStringGetDatum(src),
        CStringGetDatum(result),
        Int32GetDatum(len));
    return result;
}



static char* fast_encoding_conversion(char* src, int len, int src_encoding, int dest_encoding, FmgrInfo* convert_finfo)
{
    if (len <= 0) {
        return src;
    }

    char* result = NULL;
    Assert(convert_finfo != NULL);
    Assert(OidIsValid(convert_finfo->fn_oid));

    /*
     * Allocate space for conversion result, being wary of integer overflow
     */
    if ((Size)len >= (MaxAllocSize / (Size)MAX_CONVERSION_GROWTH)) {
        ereport(ERROR,
            (errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
                errmsg("out of memory"),
                errdetail("String of %d bytes is too long for encoding conversion.", len)));
    }
    result = (char*)palloc(len * MAX_CONVERSION_GROWTH + 1);

    FunctionCall5(convert_finfo,
        Int32GetDatum(src_encoding),
        Int32GetDatum(dest_encoding),
        CStringGetDatum(src),
        CStringGetDatum(result),
        Int32GetDatum(len));
    return result;
}

char* try_fast_encoding_conversion(char* src, int len, int src_encoding, int dest_encoding, void* convert_finfo)
{
    if (unlikely(!OidIsValid(((FmgrInfo*)convert_finfo)->fn_oid))) {
        return (char*)pg_do_encoding_conversion((unsigned char*)src, len, src_encoding, dest_encoding);
    }

    return fast_encoding_conversion(src, len, src_encoding, dest_encoding, (FmgrInfo*)convert_finfo);
}

/*
 * Convert string using encoding_name. The source
 * encoding is the DB encoding.
 *
 * BYTEA convert_to(TEXT string, NAME encoding_name) */


/*
 * pg_convert_to_text is designed to be compatible with the function convert('str' using 'encoding') in Mysql
 * TEXT pg_convert_to_text(TEXT string, NAME dest_encoding_name)
 * If there is no corresponding encoding in the destination encoding, the character is replaced with '?'.
 * It should be noted that when the client encoding does not match the dest encoding,
 * the results cannot be displayed correctly on the client side. This is not a bug.
 * We need to set b_format_behavior_compat_options = enable_multi_charset to enable the correct display of results.
*/


/* for GBK order */


/*
 * Convert string using encoding_name. The destination
 * encoding is the DB encoding.
 *
 * TEXT convert_from(BYTEA string, NAME encoding_name) */


/*
 * Convert string using encoding_names.
 *
 * BYTEA convert(BYTEA string, NAME src_encoding_name, NAME dest_encoding_name)
 */




/*
 * get the length of the string considered as text in the specified
 * encoding. Raises an error if the data is not valid in that
 * encoding.
 *
 * INT4 length (BYTEA string, NAME src_encoding_name)
 */




/*
 * convert client encoding to server encoding.
 */




/*
 * convert any encoding to server encoding.
 */


/*
 * convert any encoding to client encoding.
 */


/*
 * convert client encoding to encoding.
 */


/*
 * convert server encoding to client encoding.
 */
char* pg_server_to_client(const char* s, int len){return (char*)s;}


/*
 * Preheck if pg_server_to_any is really gonna do a conversion. That makes a difference
 * in COPY TO FILE, which is weird and not logical. Yet HandleCopyDataRow are not to
 * be changed and this function is added instead.
 */


/*
 * convert server encoding to any encoding.
 */
char* pg_server_to_any(const char* s, int len, int encoding, void *convert_finfo)
{
    Assert(u_sess->mb_cxt.DatabaseEncoding);
    Assert(u_sess->mb_cxt.ClientEncoding);

    if (len <= 0) {
        return (char*)s;
    }
    if (encoding == u_sess->mb_cxt.DatabaseEncoding->encoding || encoding == PG_SQL_ASCII) {
        return (char*)s; /* assume data is valid */
    }
    if (u_sess->mb_cxt.DatabaseEncoding->encoding == PG_SQL_ASCII) {
        /* No conversion is possible, but we must validate the result */
        (void) pg_verify_mbstr(encoding, s, len, false);
        return (char*)s;
    }
    if (u_sess->mb_cxt.ClientEncoding->encoding == encoding) {
        return perform_default_encoding_conversion(s, len, false);
    } else if (convert_finfo != NULL) {
        return try_fast_encoding_conversion(
            (char*)s, len, u_sess->mb_cxt.DatabaseEncoding->encoding, encoding, convert_finfo);
    } else {
        return (char*)pg_do_encoding_conversion(
            (unsigned char*)s, len, u_sess->mb_cxt.DatabaseEncoding->encoding, encoding);
    }
}

/*
 *	Perform default encoding conversion using cached FmgrInfo. Since
 *	this function does not access database at all, it is safe to call
 *	outside transactions.  If the conversion has not been set up by
 *	SetClientEncoding(), no conversion is performed.
 */
static char* perform_default_encoding_conversion(const char* src, int len, bool is_client_to_server)
{
    char* result = NULL;
    int src_encoding, dest_encoding;
    FmgrInfo* flinfo = NULL;

    if (is_client_to_server) {
        src_encoding = u_sess->mb_cxt.ClientEncoding->encoding;
        dest_encoding = u_sess->mb_cxt.DatabaseEncoding->encoding;
        flinfo = u_sess->mb_cxt.ToServerConvProc;
    } else {
        src_encoding = u_sess->mb_cxt.DatabaseEncoding->encoding;
        dest_encoding = u_sess->mb_cxt.ClientEncoding->encoding;
        flinfo = u_sess->mb_cxt.ToClientConvProc;
    }

    if (flinfo == NULL) {
        return (char*)src;
    }
    /*
     * Allocate space for conversion result, being wary of integer overflow
     */
    if ((Size)len >= (MaxAllocSize / (Size)MAX_CONVERSION_GROWTH)) {
        ereport(ERROR,
            (errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
                errmsg("out of memory"),
                errdetail("String of %d bytes is too long for encoding conversion.", len)));
    }
    result = (char*)palloc(len * MAX_CONVERSION_GROWTH + 1);

    FunctionCall5(flinfo,
        Int32GetDatum(src_encoding),
        Int32GetDatum(dest_encoding),
        CStringGetDatum(src),
        CStringGetDatum(result),
        Int32GetDatum(len));
    return result;
}

/* convert a multibyte string to a wchar */


/* convert a multibyte string to a wchar with a limited length */


/* same, with any encoding */


/* convert a wchar string to a multibyte */


/* convert a wchar string to a multibyte with a limited length */


/* same, with any encoding */


/* returns the byte length of a multibyte character */
int pg_mblen(const char* mbstr)
{
    return ((*pg_wchar_table[u_sess->mb_cxt.DatabaseEncoding->encoding].mblen)((const unsigned char*)mbstr));
}

/* returns the display length of a multibyte character */


/* returns the length (counted in wchars) of a multibyte string */


/* returns the length (counted in wchars) of a multibyte string
 * (not necessarily NULL terminated)
 */
int pg_mbstrlen_with_len(const char* mbstr, int limit)
{
    int len = 0;

    /* optimization for single byte encoding */
    if (pg_database_encoding_max_length() == 1) {
        return limit;
    }
    while (limit > 0 && *mbstr) {
        int l = pg_mblen(mbstr);

        limit -= l;
        mbstr += l;
        len++;
    }
    return len;
}

/* returns the length (counted in wchars) of a multibyte string
 * (not necessarily NULL terminated)
 */


/* returns the length (counted in wchars) of a multibyte string
 * with fixed encoding.
 */




/*
 * returns the byte length of a multibyte string
 * (not necessarily NULL terminated)
 * that is no longer than limit.
 * this function does not break multibyte character boundary.
 */
int pg_mbcliplen(const char* mbstr, int len, int limit)
{
    return pg_encoding_mbcliplen(u_sess->mb_cxt.DatabaseEncoding->encoding, mbstr, len, limit);
}

/*
 * pg_mbcliplen with specified encoding
 */
int pg_encoding_mbcliplen(int encoding, const char* mbstr, int len, int limit)
{
    mblen_converter mblen_fn;
    int clen = 0;
    int l;

    /* optimization for single byte encoding */
    if (pg_encoding_max_length(encoding) == 1) {
        return cliplen(mbstr, len, limit);
    }
    mblen_fn = pg_wchar_table[encoding].mblen;

    while (len > 0 && *mbstr) {
        l = (*mblen_fn)((const unsigned char*)mbstr);
        if ((clen + l) > limit) {
            break;
        }
        clen += l;
        if (clen == limit) {
            break;
        }
        len -= l;
        mbstr += l;
    }
    return clen;
}

/**
 * calculate the length of mbstr
 * @tparam calCharLength true for the character length, false for the byte length
 * @param mbstr mbstr
 * @param len length of mbstr
 * @param limit limit of mbstr
 * @return the length of mbstr  
 */


/*
 * Similar to pg_mbcliplen except the limit parameter specifies the
 * byte length, not the character length.
 */

/*
 * Description	: Similar to pg_mbcliplen except the limit parameter specifies
 * 				  the character length, not the byte length.
 * Notes		:
 */


/* mbcliplen for any single-byte encoding */
static int cliplen(const char* str, int len, int limit)
{
#ifdef DOLPHIN
    int characterLength = 0;
    int byteLength = 0;
    while (characterLength < limit && byteLength < len && str[byteLength]) {
        byteLength += get_step_len(str[byteLength]);
        characterLength++;
    }
    return byteLength;
#else
    int l = 0;
    len = Min(len, limit);
    while (l < len && str[l]) {
        l++;
    }
    return l;
#endif
}



#if defined(ENABLE_NLS)
/*
 * Make one bind_textdomain_codeset() call, translating a pg_enc to a gettext
 * codeset.  Fails for MULE_INTERNAL, an encoding unknown to gettext; can also
 * fail for gettext-internal causes like out-of-memory.
 */
static bool
raw_pg_bind_textdomain_codeset(const char *domainname, int encoding)
{
    bool		elog_ok = (CurrentMemoryContext != NULL);
    int			i;

    for (i = 0; pg_enc2gettext_tbl[i].name != NULL; i++)
    {
        if (pg_enc2gettext_tbl[i].encoding == encoding)
        {
            if (bind_textdomain_codeset(domainname,
                                        pg_enc2gettext_tbl[i].name) != NULL)
                return true;

            if (elog_ok)
                elog(LOG, "bind_textdomain_codeset failed");
            else
                write_stderr("bind_textdomain_codeset failed");

            break;
        }
    }

    return false;
}
#endif

/*
 * Bind gettext to the codeset equivalent with the database encoding.
 */
#if defined(ENABLE_NLS)
#ifndef WIN32
#endif
#endif

int GetDatabaseEncoding(void)
{
    Assert(u_sess->mb_cxt.DatabaseEncoding);
    return u_sess->mb_cxt.DatabaseEncoding->encoding;
}



int GetCharsetConnection(void)
{
    Assert(u_sess->mb_cxt.character_set_connection);
    return u_sess->mb_cxt.character_set_connection->encoding;
}



#ifdef DOLPHIN
#else
#endif







#ifdef WIN32

/*
 * Result is palloc'ed null-terminated utf16 string. The character length
 * is also passed to utf16len if not null. Returns NULL iff failed.
 */
WCHAR* pgwin32_toUTF16(const char* str, int len, int* utf16_len)
{
    WCHAR* utf16 = NULL;
    int dst_len;

    /*
     * Use MultiByteToWideChar directly if there is a corresponding codepage,
     * or double conversion through UTF8 if not.
     */
    UINT codepage = pg_enc2name_tbl[GetDatabaseEncoding()].codepage;
    if (codepage != 0) {
        utf16 = (WCHAR*)palloc(sizeof(WCHAR) * (len + 1));
        dst_len = MultiByteToWideChar(codepage, 0, str, len, utf16, len);
        utf16[dst_len] = (WCHAR)0;
    } else {
        char* utf8 = NULL;
        utf8 = (char*)pg_do_encoding_conversion((unsigned char*)str, len, GetDatabaseEncoding(), PG_UTF8);
        if (utf8 != str) {
            len = strlen(utf8);
        }
        utf16 = (WCHAR*)palloc(sizeof(WCHAR) * (len + 1));
        dst_len = MultiByteToWideChar(CP_UTF8, 0, utf8, len, utf16, len);
        utf16[dst_len] = (WCHAR)0;

        if (utf8 != str) {
            pfree(utf8);
        }
    }

    if (dst_len == 0 && len > 0) {
        pfree(utf16);
        return NULL; /* error */
    }

    if (utf16_len != NULL) {
        *utf16_len = dst_len;
    }
    return utf16;
}

#endif
