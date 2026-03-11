/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - errcode
 * - errmsg_internal
 * - in_error_recursion_trouble
 * - write_stderr
 * - freeSecurityFuncSpace
 * - elog_start
 * - elog_finish
 * - errdetail
 * - expand_fmt_string
 * - useful_strerror
 * - err_gettext
 * - errposition
 * - errhint
 * - errmodule
 * - errcause
 * - erraction
 * - errstart
 * - is_log_level_output
 * - errfinish
 * - UpdateErrorData
 * - pg_re_throw
 * - handle_in_client
 * - EmitErrorReport
 * - output_backtrace_to_log
 * - send_message_to_server_log
 * - errmsg
 * - initErrorDataArea
 *--------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------
 *
 * elog.c
 *	  error logging and reporting
 *
 * Because of the extremely high rate at which log messages can be generated,
 * we need to be mindful of the performance cost of obtaining any information
 * that may be logged.	Also, it's important to keep in mind that this code may
 * get called from within an aborted transaction, in which case operations
 * such as syscache lookups are unsafe.
 *
 * Some notes about recursion and errors during error processing:
 *
 * We need to be robust about recursive-error scenarios --- for example,
 * if we run out of memory, it's important to be able to report that fact.
 * There are a number of considerations that go into this.
 *
 * First, distinguish between re-entrant use and actual recursion.	It
 * is possible for an error or warning message to be emitted while the
 * parameters for an error message are being computed.	In this case
 * errstart has been called for the outer message, and some field values
 * may have already been saved, but we are not actually recursing.	We handle
 * this by providing a (small) stack of ErrorData records.	The inner message
 * can be computed and sent without disturbing the state of the outer message.
 * (If the inner message is actually an error, this isn't very interesting
 * because control won't come back to the outer message generator ... but
 * if the inner message is only debug or log data, this is critical.)
 *
 * Second, actual recursion will occur if an error is reported by one of
 * the elog.c routines or something they call.	By far the most probable
 * scenario of this sort is "out of memory"; and it's also the nastiest
 * to handle because we'd likely also run out of memory while trying to
 * report this error!  Our escape hatch for this case is to reset the
 * ErrorContext to empty before trying to process the inner error.	Since
 * ErrorContext is guaranteed to have at least 8K of space in it (see mcxt.c),
 * we should be able to process an "out of memory" message successfully.
 * Since we lose the prior error state due to the reset, we won't be able
 * to return to processing the original error, but we wouldn't have anyway.
 * (NOTE: the escape hatch is not used for recursive situations where the
 * inner message is of less than ERROR severity; in that case we just
 * try to process it and return normally.  Usually this will work, but if
 * it ends up in infinite recursion, we will PANIC due to error stack
 * overflow.)
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 * Portions Copyright (c) 2021, openGauss Contributors
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/error/elog.c
 *
 * -------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include <fcntl.h>
#ifdef HAVE_SYSLOG
#include <syslog.h>
#endif

#include "nodes/parsenodes.h"
#include "access/transam.h"
#include "access/xact.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "instruments/instr_slow_query_log.h"
#include "parser/gramparse.h"
#include "parser/parser.h"
#include "postmaster/postmaster.h"
#include "postmaster/syslogger.h"
#include "storage/ipc.h"
#include "storage/proc.h"
#include "storage/dss/fio_dss.h"
#include "tcop/tcopprot.h"
#include "utils/be_module.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "utils/selfuncs.h"
#include "auditfuncs.h"
#include "utils/elog.h"
#ifdef PGXC
#include "pgxc/pgxc.h"
#include "pgxc/execRemote.h"
#endif
#include "executor/exec/execStream.h"
#include "executor/executor.h"
#include "workload/workload.h"
#include "../bin/gsqlerr/errmsg.h"
#include "optimizer/randomplan.h"
#include <execinfo.h>

#include "tcop/stmt_retry.h"
#include "replication/walsender.h"
#include "nodes/pg_list.h"
#include "utils/builtins.h"
#include "funcapi.h"
#undef _
#define _(x) err_gettext(x)

static const char* err_gettext(const char* str)
    /* This extension allows gcc to check the format string for consistency with
       the supplied arguments. */
    __attribute__((format_arg(1)));

#ifndef ENABLE_LLT
extern void clean_ec_conn();
extern void delete_ec_ctrl();
#endif

#ifdef HAVE_SYSLOG

/*
 * Max string length to send to syslog().  Note that this doesn't count the
 * sequence-number prefix we add, and of course it doesn't count the prefix
 * added by syslog itself.	Solaris and sysklogd truncate the final message
 * at 1024 bytes, so this value leaves 124 bytes for those prefixes.  (Most
 * other syslog implementations seem to have limits of 2KB or so.)
 */
#ifndef PG_SYSLOG_LIMIT
#define PG_SYSLOG_LIMIT 900
#endif

static void write_syslog(int level, const char* line);
#endif

static void write_console(const char* line, int len);

#ifdef WIN32
extern THR_LOCAL char* g_instance.attr.attr_common.event_source;
static void write_eventlog(int level, const char* line, int len);
#endif





/* Macro for checking t_thrd.log_cxt.errordata_stack_depth is reasonable */
#define CHECK_STACK_DEPTH()                                               \
    do {                                                                  \
        if (t_thrd.log_cxt.errordata_stack_depth < 0) {                   \
            t_thrd.log_cxt.errordata_stack_depth = -1;                    \
            ereport(ERROR, (errmsg_internal("errstart was not called"))); \
        }                                                                 \
    } while (0)

static void log_line_prefix(StringInfo buf, ErrorData* edata);
static void send_message_to_server_log(ErrorData* edata);
static char* expand_fmt_string(const char* fmt, ErrorData* edata);
static const char* useful_strerror(int errnum);
static const char* error_severity(int elevel);
static void append_with_tabs(StringInfo buf, const char* str);
static bool is_log_level_output(int elevel, int log_min_level);
static void write_pipe_chunks(char* data, int len, int dest);
static void write_csvlog(ErrorData* edata);
static void setup_formatted_log_time(void);
static void setup_formatted_start_time(void);
extern void send_only_message_to_frontend(const char* message, bool is_putline);
static char* mask_Password_internal(const char* query_string);
static void truncate_identified_by(char* query_string, int query_len);
static char* mask_execute_direct_cmd(const char* query_string);
static bool is_execute_cmd(const char* query_string);
static void tolower_func(char *convert_query);
static void mask_espaced_character(char* source_str);
static void eraseSingleQuotes(char* query_string);
static int output_backtrace_to_log(StringInfoData* pOutBuf);
static void write_asp_chunks(char *data, int len, bool end);
static void write_asplog(char *data, int len, bool end);


#define MASK_OBS_PATH()                                                                                 \
    do {                                                                                                \
        char* childStmt = mask_funcs3_parameters(yylval.str);                                           \
        if (childStmt != NULL) {                                                                        \
            if (mask_string == NULL) {                                                                  \
                mask_string = MemoryContextStrdup(                                                      \
                    SESS_GET_MEM_CXT_GROUP(MEMORY_CONTEXT_SECURITY), query_string);                     \
                vol_mask_string = mask_string;                                                          \
            }                                                                                           \
            if (unlikely(yyextra.literallen != (int)strlen(childStmt))) {                               \
                ereport(ERROR,                                                                          \
                    (errcode(ERRCODE_SYNTAX_ERROR),                                                     \
                    errmsg("parse error on statement %s.", childStmt)));                                \
            }                                                                                           \
            rc = memcpy_s(mask_string + yylloc + 1, yyextra.literallen, childStmt, yyextra.literallen); \
            securec_check(rc, "\0", "\0");                                                              \
            rc = memset_s(childStmt, yyextra.literallen, 0, yyextra.literallen);                        \
            securec_check(rc, "", "");                                                                  \
            pfree(childStmt);                                                                           \
        }                                                                                               \
    } while (0)

/*
 * in_error_recursion_trouble --- are we at risk of infinite error recursion?
 *
 * This function exists to provide common control of various fallback steps
 * that we take if we think we are facing infinite error recursion.  See the
 * callers for details.
 */
bool in_error_recursion_trouble(void)
{
    /* Pull the plug if recurse more than once */
    return (t_thrd.log_cxt.recursion_depth > 2);
}

/*
 * One of those fallback steps is to stop trying to localize the error
 * message, since there's a significant probability that that's exactly
 * what's causing the recursion.
 */
static inline const char* err_gettext(const char* str)
{
#ifdef ENABLE_NLS
    if ((!u_sess->attr.attr_common.enable_nls) || in_error_recursion_trouble())
        return str;
    else
        return gettext(str);
#else
    return str;
#endif
}

/*
 * errstart --- begin an error-reporting cycle
 *
 * Create a stack entry and store the given parameters in it.  Subsequently,
 * errmsg() and perhaps other routines will be called to further populate
 * the stack entry.  Finally, errfinish() will be called to actually process
 * the error report.
 *
 * Returns TRUE in normal case.  Returns FALSE to short-circuit the error
 * report (if it's a warning or lower and not to be reported anywhere).
 */
bool errstart(int elevel, const char* filename, int lineno, const char* funcname, const char* domain)
{
    ErrorData* edata = NULL;
    bool output_to_server = false;
    bool output_to_client = false;
    int i = 0;
    bool verbose = false;

#ifdef ENABLE_UT
    if (t_thrd.log_cxt.disable_log_output)
        return false;
#endif

    /*
     * Check some cases in which we want to promote an error into a more
     * severe error.  None of this logic applies for non-error messages.
     */
    if (elevel >= ERROR && u_sess != NULL) {
        /*
         * If we are inside a critical section, all errors become PANIC
         * errors.	See miscadmin.h.
         */
        if (t_thrd.int_cxt.CritSectionCount > 0)
            elevel = PANIC;
        /* error accurs, cannot just release slot to pool */
        u_sess->libpq_cxt.HasErrorAccurs = true;
        /*
         * Check reasons for treating ERROR as FATAL:
         *
         * 1. we have no handler to pass the error to (implies we are in the
         * postmaster or in backend startup).
         *
         * 2. u_sess->attr.attr_common.ExitOnAnyError mode switch is set (initdb uses this).
         *
         * 3. the error occurred after proc_exit has begun to run.	(It's
         * proc_exit's responsibility to see that this doesn't turn into
         * infinite recursion!)
         */
        if (elevel == ERROR) {
            if (t_thrd.log_cxt.PG_exception_stack == NULL ||
                t_thrd.proc_cxt.proc_exit_inprogress ||
                t_thrd.xact_cxt.executeSubxactUndo)
            {
                elevel = FATAL;
            }

            if (u_sess->attr.attr_common.ExitOnAnyError && !AmPostmasterProcess()) {
                /*
                 * The following processes rely on u_sess->attr.attr_common.ExitOnAnyError to terminate successfully,
                 * during which panic is not expected.
                 */
                if (AmCheckpointerProcess() || AmBackgroundWriterProcess() || AmWalReceiverWriterProcess() ||
                    AmDataReceiverWriterProcess() || AmWLMArbiterProcess())
                    elevel = FATAL;
                else
                    elevel = PANIC;
            }

            if (u_sess->utils_cxt.test_err_type >= 3) {
                int save_type = u_sess->utils_cxt.test_err_type;

                u_sess->utils_cxt.test_err_type = 0;
                force_backtrace_messages = false;
                ereport(ERROR, (errmsg_internal("ERR CONTAINS ERR, %d", save_type)));
            }
        }

        /*
         * If the error level is ERROR or more, errfinish is not going to
         * return to caller; therefore, if there is any stacked error already
         * in progress it will be lost.  This is more or less okay, except we
         * do not want to have a FATAL or PANIC error downgraded because the
         * reporting process was interrupted by a lower-grade error.  So check
         * the stack and make sure we panic if panic is warranted.
         */
        for (i = 0; i <= t_thrd.log_cxt.errordata_stack_depth; i++)
            elevel = Max(elevel, t_thrd.log_cxt.errordata[i].elevel);
        if (elevel == FATAL && (t_thrd.role == JOB_WORKER || t_thrd.role == DMS_WORKER
#ifdef USE_SPQ
        || t_thrd.spq_ctx.spq_in_processing
#endif        
        )) {
            elevel = ERROR;
        }
    }

    if (unlikely(u_sess == NULL)) {
        if (g_instance.role == VUNKNOWN || t_thrd.role == MASTER_THREAD) {
            // there out of memory in startup, we exit directly.
            /* Ooops, hard crash time; very little we can do safely here */
            write_stderr("error occurred at %s:%d before error message processing is available\n"
                        " u_sess is NULL! gaussdb is exit now.\n",
                filename ? filename : "(unknown file)", lineno);
            _exit(STATUS_ERROR);
        } else {
            write_stderr("error occurred at %s:%d before session context available, u_sess is NULL.\n",
                         filename ? filename : "(unknown file)", lineno);
            ThreadExitCXX(STATUS_ERROR);
        }
    }
    /*
     * Now decide whether we need to process this report at all; if it's
     * warning or less and not enabled for logging, just return FALSE without
     * starting up any error logging machinery.
     */

    /* Determine whether message is enabled for server log output */
    if (IsPostmasterEnvironment)
        output_to_server = is_log_level_output(elevel, log_min_messages);
    else
        /* In bootstrap/standalone case, do not sort LOG out-of-order */
        output_to_server = (elevel >= log_min_messages);

    /* Determine whether message is enabled for client output */
    if (t_thrd.postgres_cxt.whereToSendOutput == DestRemote && elevel != COMMERROR) {
        /*
         * client_min_messages is honored only after we complete the
         * authentication handshake.  This is required both for security
         * reasons and because many clients can't handle NOTICE messages
         * during authentication.
         */
        if (u_sess && u_sess->ClientAuthInProgress)
            output_to_client = (elevel >= ERROR);
        else
            output_to_client = (elevel >= client_min_messages || elevel == INFO);
    }

    /* send to client for NOTICE messages in Stream thread */
    if (StreamThreadAmI() && elevel == NOTICE)
        output_to_client = true;

#ifdef ENABLE_QUNIT
    if (u_sess->utils_cxt.qunit_case_number != 0 && elevel >= WARNING)
        output_to_client = true;
#endif

    if (VERBOSEMESSAGE == elevel) {
        output_to_client = true;
        verbose = true;

        /* for CN  elevel is restored to INFO for the coming opetions. */
        if (IS_PGXC_COORDINATOR)
            elevel = INFO;
    }

    if ((AmWLMWorkerProcess() || AmWLMMonitorProcess() || AmWLMArbiterProcess() || AmCPMonitorProcess()) &&
        elevel >= ERROR) {
        output_to_client = false;
    }

    /* Skip processing effort if non-error message will not be output */
    if (elevel < ERROR && !output_to_server && !output_to_client)
        return false;

    /*
     * We need to do some actual work.  Make sure that memory context
     * initialization has finished, else we can't do anything useful.
     */
    if (ErrorContext == NULL) {
        /* Ooops, hard crash time; very little we can do safely here */
        write_stderr("error occurred at %s:%d before error message processing is available\n"
                     " ERRORContext is NULL now! Thread is exiting.\n",
            filename ? filename : "(unknown file)",
            lineno);

        /*
         * Libcomm permanent thread must be not exit,
         * don't allow to call ereport in libcomm thread, abort for generating core file.
         * In other cases, restart process now.
         */
        if (t_thrd.comm_cxt.LibcommThreadType != LIBCOMM_NONE)
            abort();

        if (!IsPostmasterEnvironment || t_thrd.proc_cxt.MyProcPid == PostmasterPid) {
            write_stderr("Gaussdb exit code is 2.\n");
            pg_usleep(1000);
            _exit(2);
        } else {
            /* release the Top memory context */
            force_backtrace_messages = false;
            MemoryContextDestroyAtThreadExit(t_thrd.top_mem_cxt);
            ThreadExitCXX(2);
        }
    }

    /*
     * Okay, crank up a stack entry to store the info in.
     */

    if (t_thrd.log_cxt.recursion_depth++ > 0 && elevel >= ERROR) {
        /*
         * Ooops, error during error processing.  Clear ErrorContext as
         * discussed at top of file.  We will not return to the original
         * error's reporter or handler, so we don't need it.
         */
        MemoryContextReset(ErrorContext);

        /*
         * Infinite error recursion might be due to something broken in a
         * context traceback routine.  Abandon them too.  We also abandon
         * attempting to print the error statement (which, if long, could
         * itself be the source of the recursive failure).
         */
        if (in_error_recursion_trouble()) {
            t_thrd.log_cxt.error_context_stack = NULL;
            t_thrd.postgres_cxt.debug_query_string = NULL;
        }
    }
    if (++t_thrd.log_cxt.errordata_stack_depth >= ERRORDATA_STACK_SIZE) {
        /*
         * Wups, stack not big enough.	We treat this as a PANIC condition
         * because it suggests an infinite loop of errors during error
         * recovery.
         */
        force_backtrace_messages = false;
        t_thrd.log_cxt.errordata_stack_depth = -1; /* make room on stack */

        /*Stack full, abort() directly instead of using erreport which goes to a deadloop*/
        t_thrd.int_cxt.ImmediateInterruptOK = false;
        abort();
    }

    /* Initialize data for this error frame */
    edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    errno_t rc = memset_s(edata, sizeof(ErrorData), 0, sizeof(ErrorData));
    securec_check(rc, "", "");
    edata->elevel = elevel;
    if (verbose)
        edata->verbose = true;
    edata->output_to_server = output_to_server;
    edata->output_to_client = output_to_client;
    if (filename != NULL) {
        const char* slash = NULL;

        /* keep only base name, useful especially for vpath builds */
        slash = strrchr(filename, '/');
        if (slash != NULL) {
            filename = slash + 1;
        }
    }
    edata->lineno = lineno;
    edata->filename = (char*)filename;
    edata->funcname = (char*)funcname;
    /* the default text domain is the backend's */
    edata->domain = domain ? domain : PG_TEXTDOMAIN("gaussdb");
    /* Select default errcode based on elevel */
    if (elevel >= ERROR)
        edata->sqlerrcode = ERRCODE_WRONG_OBJECT_TYPE;
    else if (elevel == WARNING)
        edata->sqlerrcode = ERRCODE_WARNING;
    else
        edata->sqlerrcode = ERRCODE_SUCCESSFUL_COMPLETION;
    /* errno is saved here so that error parameter eval can't change it */
    edata->saved_errno = errno;

    /* default module name will be used */
    edata->mod_id = MOD_MAX;
    edata->backtrace_log = NULL;
    edata->sqlstate = NULL;
    edata->class_origin = NULL;
    edata->subclass_origin = NULL;
    edata->cons_catalog = NULL;
    edata->cons_schema = NULL;
    edata->cons_name = NULL;
    edata->catalog_name = NULL;
    edata->schema_name = NULL;
    edata->table_name = NULL;
    edata->column_name = NULL;
    edata->cursor_name = NULL;
    edata->mysql_errno = NULL;

    t_thrd.log_cxt.recursion_depth--;
    return true;
}

/*
 * errfinish --- end an error-reporting cycle
 *
 * Produce the appropriate error report(s) and pop the error stack.
 *
 * If elevel is ERROR or worse, control does not return to the caller.
 * See elog.h for the error level definitions.
 */
void errfinish(int dummy, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    ErrorData* producer_save_edata = NULL;
    int elevel = edata->elevel;
    bool is_warnings_throw = edata->is_warnings_throw;
    MemoryContext oldcontext;

    ErrorContextCallback* econtext = NULL;

    t_thrd.log_cxt.recursion_depth++;
    force_backtrace_messages = false;

    CHECK_STACK_DEPTH();

    /*
     * If procuer thread save a edata when report ERROR,
     * now top consumer need use the saved edata.
     */
    if (StreamTopConsumerAmI() && u_sess->stream_cxt.global_obj != NULL && elevel >= ERROR) {
        producer_save_edata = u_sess->stream_cxt.global_obj->getProducerEdata();

        /*
         * In executing stream operator, when top consumer's elevel is greater than
         * producer's elevel, we can't update top consumer's elevel, because that operator
         * may decrease top consumer's elevel in some scene.
         */
        if (producer_save_edata != NULL && producer_save_edata->elevel >= elevel) {
            UpdateErrorData(edata, producer_save_edata);
            elevel = edata->elevel;
        }
    }

    /*
     * Do processing in ErrorContext, which we hope has enough reserved space
     * to report an error.
     */
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    /*
     * Call any context callback functions.  Errors occurring in callback
     * functions will be treated as recursive errors --- this ensures we will
     * avoid infinite recursion (see errstart).
     */
    for (econtext = t_thrd.log_cxt.error_context_stack; econtext != NULL; econtext = econtext->previous)
        (*econtext->callback)(econtext->arg);
    /* Database Security: Support database audit */
    /* Audit beyond privileges */
    if (edata->sqlerrcode == ERRCODE_INSUFFICIENT_PRIVILEGE) {
        pgaudit_user_no_privileges(NULL, edata->message);
    }

    /*
     * Because ErrorContext will be reset during FlushErrorState,
     * so we can reset NULL here
     */
    edata->backtrace_log = NULL;

    /* get backtrace info */
    if (edata->elevel >= u_sess->attr.attr_common.backtrace_min_messages && !t_thrd.log_cxt.output_backtrace_log) {
        StringInfoData buf;
        initStringInfo(&buf);

        HOLD_INTERRUPTS();
        int ret = output_backtrace_to_log(&buf);
        RESUME_INTERRUPTS();

        if (0 == ret) {
            edata->backtrace_log = pstrdup(buf.data);
        }
        pfree(buf.data);
    }

#ifdef MEMORY_CONTEXT_CHECKING
    /* Check all memory contexts when there is an error or a fatal */
    if (elevel >= ERROR || is_warnings_throw) {
        MemoryContextCheck(t_thrd.top_mem_cxt, false);
    }
#endif

    /* 
     * Make sure reset create schema flag if error happen,
     * even in pg_try_catch case and procedure exception case.
     * Top transaction memcxt will release the memory, just set NULL.
     */
    if (elevel >= ERROR || is_warnings_throw) {
        u_sess->catalog_cxt.setCurCreateSchema = false;
        u_sess->catalog_cxt.curCreateSchema = NULL;
        u_sess->exec_cxt.isLockRows = false;
        u_sess->exec_cxt.isFlashBack = false;
    }

    /*
     * If ERROR (not more nor less) we pass it off to the current handler.
     * Printing it and popping the stack is the responsibility of the handler.
     */
    if (elevel == ERROR || is_warnings_throw) {
        /*
         * We do some minimal cleanup before longjmp'ing so that handlers can
         * execute in a reasonably sane state.
         */

        /* This is just in case the error came while waiting for input */
        t_thrd.int_cxt.ImmediateInterruptOK = false;

        /*
         * Reset t_thrd.int_cxt.InterruptHoldoffCount in case we ereport'd from inside an
         * interrupt holdoff section.  (We assume here that no handler will
         * itself be inside a holdoff section.	If necessary, such a handler
         * could save and restore t_thrd.int_cxt.InterruptHoldoffCount for itself, but this
         * should make life easier for most.)
         */
        t_thrd.int_cxt.InterruptHoldoffCount = 0;
        t_thrd.int_cxt.QueryCancelHoldoffCount = 0;

        t_thrd.int_cxt.InterruptCountResetFlag = true;

        t_thrd.int_cxt.CritSectionCount = 0; /* should be unnecessary, but... */

        /*
         * Note that we leave CurrentMemoryContext set to ErrorContext. The
         * handler should reset it to something else soon.
         */

        t_thrd.log_cxt.recursion_depth--;

        if (EnableLocalSysCache()) {
            MemoryContextSwitchTo(oldcontext);
        }

        PG_RE_THROW();
    }

    /*
     * If we are doing FATAL or PANIC, abort any old-style COPY OUT in
     * progress, so that we can report the message before dying.  (Without
     * this, pq_putmessage will refuse to send the message at all, which is
     * what we want for NOTICE messages, but not for fatal exits.) This hack
     * is necessary because of poor design of old-style copy protocol.
     */
    if (elevel >= FATAL && t_thrd.postgres_cxt.whereToSendOutput == DestRemote)
        pq_endcopyout(true);

    bool isVerbose = false;
    if (edata->elevel == VERBOSEMESSAGE) {
        edata->elevel = INFO;
        handle_in_client(true);
        isVerbose = true;
    }

    if (StreamThreadAmI() && u_sess->stream_cxt.producer_obj != NULL && elevel == FATAL) {
        /*
         * Just like reportError() in longjump point of StreamMain(),
         * report FATAL error to consumer here.
         */
        ((StreamProducer*)u_sess->stream_cxt.producer_obj)->reportError();
    } else if (StreamThreadAmI() && u_sess->stream_cxt.producer_obj != NULL && elevel < ERROR) {
        /* Send to server log, if enabled */
        if (edata->output_to_server && is_errmodule_enable(edata->elevel, edata->mod_id))
            send_message_to_server_log(edata);

        /* Send to client, if enabled */
        if (edata->output_to_client) {
            /* report NOTICE to consumer here. */
            u_sess->stream_cxt.producer_obj->reportNotice();
        }
    } else {
        /* Emit the message to the right places */
        EmitErrorReport();
    }

#ifdef ENABLE_MULTIPLE_NODES
    if (elevel >= ERROR) {
        clean_ec_conn();
        delete_ec_ctrl();
    }
#endif

    if (isVerbose)
        handle_in_client(false);

    /* Now free up subsidiary data attached to stack entry, and release it */
    if (edata->message)
        pfree(edata->message);
    if (edata->detail)
        pfree(edata->detail);
    if (edata->detail_log)
        pfree(edata->detail_log);
    if (edata->hint)
        pfree(edata->hint);
    if (edata->context)
        pfree(edata->context);
    if (edata->internalquery)
        pfree(edata->internalquery);
    if (edata->backtrace_log)
        pfree(edata->backtrace_log);
    if (edata->cause)
        pfree(edata->cause);
    if (edata->action)
        pfree(edata->action);
    if (edata->sqlstate)
        pfree(edata->sqlstate);
    if (edata->class_origin)
        pfree(edata->class_origin);
    if (edata->subclass_origin)
        pfree(edata->subclass_origin);
    if (edata->cons_catalog)
        pfree(edata->cons_catalog);
    if (edata->cons_schema)
        pfree(edata->cons_schema);
    if (edata->cons_name)
        pfree(edata->cons_name);
    if (edata->catalog_name)
        pfree(edata->catalog_name);
    if (edata->schema_name)
        pfree(edata->schema_name);
    if (edata->table_name)
        pfree(edata->table_name);
    if (edata->column_name)
        pfree(edata->column_name);
    if (edata->cursor_name)
        pfree(edata->cursor_name);
    if (edata->mysql_errno)
        pfree(edata->mysql_errno);
    t_thrd.log_cxt.errordata_stack_depth--;

    /* Exit error-handling context */
    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;

    /*
     * Perform error recovery action as specified by elevel.
     */
    if (elevel == FATAL) {
        /*
         * For a FATAL error, we let proc_exit clean up and exit.
         */
        t_thrd.int_cxt.ImmediateInterruptOK = false;

        /*
         * If we just reported a startup failure, the client will disconnect
         * on receiving it, so don't send any more to the client.
         */
        if (t_thrd.log_cxt.PG_exception_stack == NULL && t_thrd.postgres_cxt.whereToSendOutput == DestRemote)
            t_thrd.postgres_cxt.whereToSendOutput = DestNone;

        /*
         * fflush here is just to improve the odds that we get to see the
         * error message, in case things are so hosed that proc_exit crashes.
         * Any other code you might be tempted to add here should probably be
         * in an on_proc_exit or on_shmem_exit callback instead.
         */
        fflush(stdout);
        fflush(stderr);

        /* release operator-level hash table in memory */
        releaseExplainTable();

        if (StreamTopConsumerAmI() && u_sess->debug_query_id != 0) {
            gs_close_all_stream_by_debug_id(u_sess->debug_query_id);
        }

        /*
         * Do normal process-exit cleanup, then return exit code 1 to indicate
         * FATAL termination.  The postmaster may or may not consider this
         * worthy of panic, depending on which subprocess returns it.
         */
        proc_exit(1);
    }

    if (elevel >= PANIC) {
        /*
         * Serious crash time. Postmaster will observe SIGABRT process exit
         * status and kill the other backends too.
         *
         * XXX: what if we are *in* the postmaster?  abort() won't kill our
         * children...
         */
        t_thrd.int_cxt.ImmediateInterruptOK = false;
        fflush(stdout);
        fflush(stderr);
        abort();
    }

    /*
     * We reach here if elevel <= WARNING. OK to return to caller.
     *
     * But check for cancel/die interrupt first --- this is so that the user
     * can stop a query emitting tons of notice or warning messages, even if
     * it's in a loop that otherwise fails to check for interrupts.
     * Just check for interrupts when ignore_interrupt is not set to true.
     *
     * Well, I think CHECK_FOR_INTERRUPTS() here is somewhat _terrible_. Programmers have put elog nearly everywhere in
     * the code, including critical section which should be executed atomically. CHECK_FOR_INTERRUPTS() here have the
     * probability to break such critical section. This will result in unexpected behaviors!
     */
}

/*
 * @Description: set module id for logging
 * @IN id: module id
 * @Return:
 * @See also:
 */
int errmodule(ModuleId id)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];

    /* we don't bother incrementing t_thrd.log_cxt.recursion_depth */
    CHECK_STACK_DEPTH();

    /* set module id */
    Assert(VALID_SINGLE_MODULE(id));
    edata->mod_id = id;

    /* return value does not matter */
    return 0;
}

/*
 * errcode --- add SQLSTATE error code to the current error
 *
 * The code is expected to be represented as per MAKE_SQLSTATE().
 */
int errcode(int sqlerrcode)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];

    /* we don't bother incrementing t_thrd.log_cxt.recursion_depth */
    CHECK_STACK_DEPTH();

    edata->sqlerrcode = sqlerrcode;

    return 0; /* return value does not matter */
}

/*
 * save error message for history session info
 */


/*
 * errcode_for_file_access --- add SQLSTATE error code to the current error
 *
 * The SQLSTATE code is chosen based on the saved errno value.	We assume
 * that the failing operation was some type of disk file access.
 *
 * NOTE: the primary error message string should generally include %m
 * when this is used.
 */
#ifdef EROFS
#endif
#if defined(ENOTEMPTY) && (ENOTEMPTY != EEXIST) /* same code on AIX */
#endif

/*
 * errcode_for_socket_access --- add SQLSTATE error code to the current error
 *
 * The SQLSTATE code is chosen based on the saved errno value.	We assume
 * that the failing operation was some type of socket access.
 *
 * NOTE: the primary error message string should generally include %m
 * when this is used.
 */
#ifdef ECONNRESET
#endif

/*
 * This macro handles expansion of a format string and associated parameters;
 * it's common code for errmsg(), errdetail(), etc.  Must be called inside
 * a routine that is declared like "const char *fmt, ..." and has an edata
 * pointer set up.	The message is assigned to edata->targetfield, or
 * appended to it if appendval is true.  The message is subject to translation
 * if translateit is true.
 *
 * Note: we pstrdup the buffer rather than just transferring its storage
 * to the edata field because the buffer might be considerably larger than
 * really necessary.
 */
#define EVALUATE_MESSAGE(targetfield, appendval, translateit)           \
    {                                                                   \
        char* fmtbuf = NULL;                                            \
        StringInfoData buf;                                             \
        /* Internationalize the error format string */                  \
        if ((translateit) && !in_error_recursion_trouble())             \
            fmt = dgettext(edata->domain, fmt);                         \
        /* Expand %m in format string */                                \
        fmtbuf = expand_fmt_string(fmt, edata);                         \
        initStringInfo(&buf);                                           \
        if ((appendval) && edata->targetfield) {                        \
            appendStringInfoString(&buf, edata->targetfield);           \
            appendStringInfoChar(&buf, '\n');                           \
        }                                                               \
        /* Generate actual output --- have to use appendStringInfoVA */ \
        for (; ; ) {                                                      \
            va_list args;                                               \
            bool success = false;                                       \
            va_start(args, fmt);                                        \
            success = appendStringInfoVA(&buf, fmtbuf, args);           \
            va_end(args);                                               \
            if (success)                                                \
                break;                                                  \
            enlargeStringInfo(&buf, buf.maxlen);                        \
        }                                                               \
        /* Done with expanded fmt */                                    \
        pfree(fmtbuf);                                                  \
        /* Save the completed message into the stack item */            \
        if (edata->targetfield)                                         \
            pfree(edata->targetfield);                                  \
        edata->targetfield = pstrdup(buf.data);                         \
        pfree(buf.data);                                                \
    }

/*
 * Same as above, except for pluralized error messages.  The calling routine
 * must be declared like "const char *fmt_singular, const char *fmt_plural,
 * unsigned long n, ...".  Translation is assumed always wanted.
 */
#define EVALUATE_MESSAGE_PLURAL(targetfield, appendval)                  \
    {                                                                    \
        const char* fmt = NULL;                                          \
        char* fmtbuf = NULL;                                             \
        StringInfoData buf;                                              \
        /* Internationalize the error format string */                   \
        if (u_sess->attr.attr_common.enable_nls                          \
            && (!in_error_recursion_trouble()))                          \
            fmt = dngettext(edata->domain, fmt_singular, fmt_plural, n); \
        else                                                             \
            fmt = (n == 1 ? fmt_singular : fmt_plural);                  \
        /* Expand %m in format string */                                 \
        fmtbuf = expand_fmt_string(fmt, edata);                          \
        initStringInfo(&buf);                                            \
        if ((appendval) && edata->targetfield) {                         \
            appendStringInfoString(&buf, edata->targetfield);            \
            appendStringInfoChar(&buf, '\n');                            \
        }                                                                \
        /* Generate actual output --- have to use appendStringInfoVA */  \
        for (; ; ) {                                                       \
            va_list args;                                                \
            bool success = false;                                        \
            va_start(args, n);                                           \
            success = appendStringInfoVA(&buf, fmtbuf, args);            \
            va_end(args);                                                \
            if (success)                                                 \
                break;                                                   \
            enlargeStringInfo(&buf, buf.maxlen);                         \
        }                                                                \
        /* Done with expanded fmt */                                     \
        pfree(fmtbuf);                                                   \
        /* Save the completed message into the stack item */             \
        if (edata->targetfield)                                          \
            pfree(edata->targetfield);                                   \
        edata->targetfield = pstrdup(buf.data);                          \
        pfree(buf.data);                                                 \
    }

/*
 * errmsg --- add a primary error message text to the current error
 *
 * In addition to the usual %-escapes recognized by printf, "%m" in
 * fmt is replaced by the error message for the caller's value of errno.
 *
 * Note: no newline is needed at the end of the fmt string, since
 * ereport will provide one for the output methods that need it.
 */
int errmsg(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(message, false, u_sess->attr.attr_common.enable_nls);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}

/*
 * errmsg_internal --- add a primary error message text to the current error
 *
 * This is exactly like errmsg() except that strings passed to errmsg_internal
 * are not translated, and are customarily left out of the
 * internationalization message dictionary.  This should be used for "can't
 * happen" cases that are probably not worth spending translation effort on.
 * We also use this for certain cases where we *must* not try to translate
 * the message because the translation would fail and result in infinite
 * error recursion.
 */
int errmsg_internal(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(message, false, false);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}

/*
 * errmsg_plural --- add a primary error message text to the current error,
 * with support for pluralization of the message text
 */


/*
 * errdetail --- add a detail error message text to the current error
 */
int errdetail(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(detail, false, u_sess->attr.attr_common.enable_nls);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}

/*
 * errdetail_internal --- add a detail error message text to the current error
 *
 * This is exactly like errdetail() except that strings passed to
 * errdetail_internal are not translated, and are customarily left out of the
 * internationalization message dictionary.  This should be used for detail
 * messages that seem not worth translating for one reason or another
 * (typically, that they don't seem to be useful to average users).
 */


/*
 * errdetail_log --- add a detail_log error message text to the current error
 */


/*
 * errdetail_plural --- add a detail error message text to the current error,
 * with support for pluralization of the message text
 */


int errcause(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(cause, false, u_sess->attr.attr_common.enable_nls);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}

int erraction(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(action, false, u_sess->attr.attr_common.enable_nls);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}
/*
 * errhint --- add a hint error message text to the current error
 */
int errhint(const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(hint, false, u_sess->attr.attr_common.enable_nls);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
    return 0; /* return value does not matter */
}

/*
 * errquery --- add a query error message text to the current error
 */


/*
 * errcontext --- add a context error message text to the current error
 *
 * Unlike other cases, multiple calls are allowed to build up a stack of
 * context information.  We assume earlier calls represent more-closely-nested
 * states.
 */


/*
 * errhidestmt --- optionally suppress STATEMENT: field of log entry
 *
 * This should be called if the message text already includes the statement.
 */

/*
 * errhideprefix --- optionally suppress line prefix: field of log entry
 *
 * This should be called if the message text already includes the line prefix.
 */


/*
 * errposition --- add cursor position to the current error
 */
int errposition(int cursorpos)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];

    /* we don't bother incrementing t_thrd.log_cxt.recursion_depth */
    CHECK_STACK_DEPTH();

    edata->cursorpos = cursorpos;

    return 0; /* return value does not matter */
}

/*
 * internalerrposition --- add internal cursor position to the current error
 */


/*
 * internalerrquery --- add internal query text to the current error
 *
 * Can also pass NULL to drop the internal query text entry.  This case
 * is intended for use in error callback subroutines that are editorializing
 * on the layout of the error report.
 */








/*
 * signal_returnd_sqlstate --- add returnd_sqlstate to the current error
 */


/*
 * siganl_class_origin --- add class_origin to the current error
 */


/*
 * siganl_subclass_origin --- add subclass_origin to the current error
 */


/*
 * signal_constraint_catalog --- add constraint_catalog to the current error
 */


/*
 * signal_constraint_schema --- add constraint_schema to the current error
 */


/*
 * signal_constraint_name --- add constraint_name to the current error
 */


/*
 * signal_catalog_name --- add catalog_name to the current error
 */


/*
 * signal_schema_name --- add schema_name to the current error
 */


/*
 * signal_table_name --- add table_name to the current error
 */


/*
 * signal_column_name --- add column_name to the current error
 */


/*
 * signal_cursor_name --- add cursor_name to the current error
 */


/*
 * signal_mysql_errno --- add mysql_errno to the current error
 */

/*
 * ErrOutToClient --- sets whether to send error output to client or not.
 */


/*
 * geterrcode --- return the currently set SQLSTATE error code
 *
 * This is only intended for use in error callback subroutines, since there
 * is no other place outside elog.c where the concept is meaningful.
 */


/*
 * Geterrmsg --- return the currently set error message
 *
 * This is only intended for use in error callback subroutines, since there
 * is no other place outside elog.c where the concept is meaningful.
 */


/*
 * geterrposition --- return the currently set error position (0 if none)
 *
 * This is only intended for use in error callback subroutines, since there
 * is no other place outside elog.c where the concept is meaningful.
 */


/*
 * getinternalerrposition --- same for internal error position
 *
 * This is only intended for use in error callback subroutines, since there
 * is no other place outside elog.c where the concept is meaningful.
 */


/*
 * handle_in_client --- mark if the message should be sent and handled in client
 */
int handle_in_client(bool handle)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];

    /* we don't bother incrementing t_thrd.log_cxt.recursion_depth */
    CHECK_STACK_DEPTH();

    edata->handle_in_client = handle;

    return 0; /* return value does not matter */
}

/*
 * ignore_interrupt --- mark if should ignore interrupt when writing server log
 */


/*
 * onlyfrontmsg
 * outputflag, false -- output err level
 *             true  -- don' output err level
 */


/*
 * elog_start --- startup for old-style API
 *
 * All that we do here is stash the hidden filename/lineno/funcname
 * arguments into a stack entry.
 *
 * We need this to be separate from elog_finish because there's no other
 * portable way to deal with inserting extra arguments into the elog call.
 * (If macros with variable numbers of arguments were portable, it'd be
 * easy, but they aren't.)
 */
void elog_start(const char* filename, int lineno, const char* funcname)
{
    ErrorData* edata = NULL;

#ifdef ENABLE_UT
    if (t_thrd.log_cxt.disable_log_output)
        return;
#endif

    /* Make sure that memory context initialization has finished */
    if (ErrorContext == NULL) {
        /* Ooops, hard crash time; very little we can do safely here */
        write_stderr("error occurred at %s:%d before error message processing is available\n",
            filename ? filename : "(unknown file)",
            lineno);
        pg_usleep(1000);
        _exit(2);
    }

    if (++t_thrd.log_cxt.errordata_stack_depth >= ERRORDATA_STACK_SIZE) {
        /*
         * Wups, stack not big enough.	We treat this as a PANIC condition
         * because it suggests an infinite loop of errors during error
         * recovery.  Note that the message is intentionally not localized,
         * else failure to convert it to client encoding could cause further
         * recursion.
         */
        t_thrd.log_cxt.errordata_stack_depth = -1; /* make room on stack */
        ereport(PANIC, (errmsg_internal("ERRORDATA_STACK_SIZE exceeded")));
    }

    edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    if (filename != NULL) {
        const char* slash = NULL;

        /* keep only base name, useful especially for vpath builds */
        slash = strrchr(filename, '/');
        if (slash != NULL) {
            filename = slash + 1;
        }
    }
    edata->filename = (char*)filename;
    edata->lineno = lineno;
    edata->funcname = (char*)funcname;
    /* errno is saved now so that error parameter eval can't change it */
    edata->saved_errno = errno;
    edata->backtrace_log = NULL;
}

/*
 * elog_finish --- finish up for old-style API
 */
void elog_finish(int elevel, const char* fmt, ...)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

#ifdef ENABLE_UT
    if (t_thrd.log_cxt.disable_log_output)
        return;
#endif

    CHECK_STACK_DEPTH();

    /*
     * Do errstart() to see if we actually want to report the message.
     */
    t_thrd.log_cxt.errordata_stack_depth--;
    errno = edata->saved_errno;
    if (!errstart(elevel, edata->filename, edata->lineno, edata->funcname, NULL))
        return; /* nothing to do */

    /*
     * Format error message just like errmsg_internal().
     */
    t_thrd.log_cxt.recursion_depth++;
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    EVALUATE_MESSAGE(message, false, false);

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;

    /*
     * And let errfinish() finish up.
     */
    errfinish(0);
}

/*
 * Functions to allow construction of error message strings separately from
 * the ereport() call itself.
 *
 * The expected calling convention is
 *
 *	pre_format_elog_string(errno, domain), var = format_elog_string(format,...)
 *
 * which can be hidden behind a macro such as GUC_check_errdetail().  We
 * assume that any functions called in the arguments of format_elog_string()
 * cannot result in re-entrant use of these functions --- otherwise the wrong
 * text domain might be used, or the wrong errno substituted for %m.  This is
 * okay for the current usage with GUC check hooks, but might need further
 * effort someday.
 *
 * The result of format_elog_string() is stored in ErrorContext, and will
 * therefore survive until FlushErrorState() is called.
 */




/*
 * Actual output of the top-of-stack error message
 *
 * In the ereport(ERROR) case this is called from PostgresMain (or not at all,
 * if the error is caught by somebody).  For all other severity levels this
 * is called by errfinish.
 */
void EmitErrorReport(void)
{
    ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];
    MemoryContext oldcontext;

    t_thrd.log_cxt.recursion_depth++;
    CHECK_STACK_DEPTH();
    oldcontext = MemoryContextSwitchTo(ErrorContext);

    /* Send to server log, if enabled */
    if (edata->output_to_server && is_errmodule_enable(edata->elevel, edata->mod_id))
        send_message_to_server_log(edata);

    /* Send to client, if enabled */
    if (edata->output_to_client) {
        bool need_skip_by_retry = IsStmtRetryAvaliable(edata->elevel, edata->sqlerrcode);
        bool can_skip = (edata->elevel < FATAL);
        if (can_skip && need_skip_by_retry) {
            /* skip sending messsage to front, do noting for now */
        } else {
            if (u_sess->proc_cxt.MyProcPort && u_sess->proc_cxt.MyProcPort->protocol_config) {
                u_sess->proc_cxt.MyProcPort->protocol_config->fn_send_message(edata);
            }
        }
    }

    MemoryContextSwitchTo(oldcontext);
    t_thrd.log_cxt.recursion_depth--;
}

/*
 * CopyErrorData --- obtain a copy of the topmost error stack entry
 *
 * This is only for use in error handler code.	The data is copied into the
 * current memory context, so callers should always switch away from
 * ErrorContext first; otherwise it will be lost when FlushErrorState is done.
 */


/*
 * UpdateErrorData --- update current edata from newData.
 */
void UpdateErrorData(ErrorData* edata, ErrorData* newData)
{
    FREE_POINTER(edata->message);
    FREE_POINTER(edata->detail);
    FREE_POINTER(edata->detail_log);
    FREE_POINTER(edata->hint);
    FREE_POINTER(edata->context);
    FREE_POINTER(edata->internalquery);
    FREE_POINTER(edata->backtrace_log);
    FREE_POINTER(edata->cause);
    FREE_POINTER(edata->action);
    FREE_POINTER(edata->sqlstate);
    FREE_POINTER(edata->class_origin);
    FREE_POINTER(edata->subclass_origin);
    FREE_POINTER(edata->cons_catalog);
    FREE_POINTER(edata->cons_schema);
    FREE_POINTER(edata->cons_name);
    FREE_POINTER(edata->catalog_name);
    FREE_POINTER(edata->schema_name);
    FREE_POINTER(edata->table_name);
    FREE_POINTER(edata->column_name);
    FREE_POINTER(edata->cursor_name);
    FREE_POINTER(edata->mysql_errno);
    MemoryContext oldcontext = MemoryContextSwitchTo(ErrorContext);

    edata->elevel = newData->elevel;
    edata->filename = pstrdup(newData->filename);
    edata->lineno = newData->lineno;
    edata->funcname = pstrdup(newData->funcname);
    edata->sqlerrcode = newData->sqlerrcode;
    edata->message = pstrdup(newData->message);
    edata->detail = pstrdup(newData->detail);
    edata->detail_log = pstrdup(newData->detail_log);
    edata->hint = pstrdup(newData->hint);
    edata->context = pstrdup(newData->context);
    edata->cursorpos = newData->cursorpos;
    edata->internalpos = newData->internalpos;
    edata->internalquery = pstrdup(newData->internalquery);
    edata->saved_errno = newData->saved_errno;
    edata->backtrace_log = pstrdup(newData->backtrace_log);
    edata->internalerrcode = newData->internalerrcode;
    edata->cause = newData->cause;
    edata->action = newData->action;
    edata->sqlstate = pstrdup(newData->sqlstate);
    edata->class_origin = pstrdup(newData->class_origin);
    edata->subclass_origin = pstrdup(newData->subclass_origin);
    edata->cons_catalog = pstrdup(newData->cons_catalog);
    edata->cons_schema = pstrdup(newData->cons_schema);
    edata->cons_name = pstrdup(newData->cons_name);
    edata->catalog_name = pstrdup(newData->catalog_name);
    edata->schema_name = pstrdup(newData->schema_name);
    edata->table_name = pstrdup(newData->table_name);
    edata->column_name = pstrdup(newData->column_name);
    edata->cursor_name = pstrdup(newData->cursor_name);
    edata->mysql_errno = pstrdup(newData->mysql_errno);
    MemoryContextSwitchTo(oldcontext);
}

/*
 * FreeErrorData --- free the structure returned by CopyErrorData.
 *
 * Error handlers should use this in preference to assuming they know all
 * the separately-allocated fields.
 */


/*
 * FlushErrorState --- flush the error state after error recovery
 *
 * This should be called by an error handler after it's done processing
 * the error; or as soon as it's done CopyErrorData, if it intends to
 * do stuff that is likely to provoke another error.  You are not "out" of
 * the error subsystem until you have done this.
 */




/*
 * ReThrowError --- re-throw a previously copied error
 *
 * A handler can do CopyErrorData/FlushErrorState to get out of the error
 * subsystem, then do some processing, and finally ReThrowError to re-throw
 * the original error.	This is slower than just PG_RE_THROW() but should
 * be used if the "some processing" is likely to incur another error.
 */


/*
 * pg_re_throw --- out-of-line implementation of PG_RE_THROW() macro
 */
void pg_re_throw(void)
{
    /* If possible, throw the error to the next outer setjmp handler */
    if (t_thrd.log_cxt.PG_exception_stack != NULL)
        siglongjmp(*t_thrd.log_cxt.PG_exception_stack, 1);
    else {
        /*
         * If we get here, elog(ERROR) was thrown inside a PG_TRY block, which
         * we have now exited only to discover that there is no outer setjmp
         * handler to pass the error to.  Had the error been thrown outside
         * the block to begin with, we'd have promoted the error to FATAL, so
         * the correct behavior is to make it FATAL now; that is, emit it and
         * then call proc_exit.
         */
        ErrorData* edata = &t_thrd.log_cxt.errordata[t_thrd.log_cxt.errordata_stack_depth];

        Assert(t_thrd.log_cxt.errordata_stack_depth >= 0);
        Assert(edata->elevel == ERROR);
        edata->elevel = FATAL;

        /*
         * At least in principle, the increase in severity could have changed
         * where-to-output decisions, so recalculate.  This should stay in
         * sync with errstart(), which see for comments.
         */
        if (IsPostmasterEnvironment)
            edata->output_to_server = is_log_level_output(FATAL, log_min_messages);
        else
            edata->output_to_server = (FATAL >= log_min_messages);

        if (t_thrd.postgres_cxt.whereToSendOutput == DestRemote)
            edata->output_to_client = true;

        /*
         * We can use errfinish() for the rest, but we don't want it to call
         * any error context routines a second time.  Since we know we are
         * about to exit, it should be OK to just clear the context stack.
         */
        t_thrd.log_cxt.error_context_stack = NULL;

        errfinish(0);
    }

    /* Doesn't return ... */
    ExceptionalCondition("pg_re_throw tried to return", "FailedAssertion", __FILE__, __LINE__);
    abort(); // keep compiler quiet
}


/*
 * PgRethrowAsFatal - Promote the error level to fatal.
 */



/*
 * Initialization of error output file
 */


#ifdef HAVE_SYSLOG

/*
 * Set or update the parameters for syslog logging
 */


/*
 * Write a message line to syslog
 */

#endif /* HAVE_SYSLOG */

#ifdef WIN32
/*
 * Write a message line to the windows event log
 */
static void write_eventlog(int level, const char* line, int len)
{
    WCHAR* utf16 = NULL;
    int eventlevel = EVENTLOG_ERROR_TYPE;
    static HANDLE evtHandle = INVALID_HANDLE_VALUE;

    if (evtHandle == INVALID_HANDLE_VALUE) {
        evtHandle = RegisterEventSource(
            NULL, g_instance.attr.attr_common.event_source ? g_instance.attr.attr_common.event_source : "PostgreSQL");
        if (evtHandle == NULL) {
            evtHandle = INVALID_HANDLE_VALUE;
            return;
        }
    }

    switch (level) {
        case DEBUG5:
        case DEBUG4:
        case DEBUG3:
        case DEBUG2:
        case DEBUG1:
        case LOG:
        case COMMERROR:
        case INFO:
        case NOTICE:
            eventlevel = EVENTLOG_INFORMATION_TYPE;
            break;
        case WARNING:
            eventlevel = EVENTLOG_WARNING_TYPE;
            break;
        case ERROR:
        case FATAL:
        case PANIC:
        default:
            eventlevel = EVENTLOG_ERROR_TYPE;
            break;
    }

    /*
     * Convert message to UTF16 text and write it with ReportEventW, but
     * fall-back into ReportEventA if conversion failed.
     *
     * Also verify that we are not on our way into error recursion trouble due
     * to error messages thrown deep inside pgwin32_toUTF16().
     */
    if (GetDatabaseEncoding() != GetPlatformEncoding() && !in_error_recursion_trouble()) {
        utf16 = pgwin32_toUTF16(line, len, NULL);
        if (NULL != utf16) {
            ReportEventW(evtHandle,
                eventlevel,
                0,
                0, /* All events are Id 0 */
                NULL,
                1,
                0,
                (LPCWSTR*)&utf16,
                NULL);

            pfree(utf16);
            return;
        }
    }
    ReportEventA(evtHandle,
        eventlevel,
        0,
        0, /* All events are Id 0 */
        NULL,
        1,
        0,
        &line,
        NULL);
}
#endif /* WIN32 */

#ifdef WIN32
#else
#endif

/*
 * setup t_thrd.log_cxt.formatted_log_time, for consistent times between CSV and regular logs
 */


/*
 * setup t_thrd.log_cxt.formatted_start_time
 */


/*
 * Format tag info for log lines; append to the provided buffer.
 */


/*
 * append a CSV'd version of a string to a StringInfo
 * We use the openGauss defaults for CSV, i.e. quote = escape = '"'
 * If it's NULL, append nothing.
 */


/*
 * Constructs the error message, depending on the Errordata it gets, in a CSV
 * format which is described in doc/src/sgml/config.sgml.
 */


/*
 * Unpack MAKE_SQLSTATE code. Note that this returns a pointer to a
 * static THR_LOCAL buffer.
 */


/* if sqlcode is init by the database, it must be positive; if is init by the user, then it must be negative */




int output_backtrace_to_log(StringInfoData* pOutBuf)
{
    const int max_buffer_size = 128;
    void* buffer[max_buffer_size];

    char title[max_buffer_size];

    AutoMutexLock btLock(&bt_lock);

    btLock.lock();
    
    t_thrd.log_cxt.output_backtrace_log = true;

    if (t_thrd.log_cxt.thd_bt_symbol) {
        free(t_thrd.log_cxt.thd_bt_symbol);
        t_thrd.log_cxt.thd_bt_symbol = NULL;
    }

    int len_symbols = backtrace(buffer, max_buffer_size);
    t_thrd.log_cxt.thd_bt_symbol = backtrace_symbols(buffer, len_symbols);

    int ret = snprintf_s(title, sizeof(title), sizeof(title) - 1, "tid[%d]'s backtrace:\n", gettid());
    securec_check_ss_c(ret, "\0", "\0");
    appendStringInfoString(pOutBuf, title);

    if (NULL == t_thrd.log_cxt.thd_bt_symbol) {
        appendStringInfoString(pOutBuf, "Failed to get backtrace symbols.\n");
        t_thrd.log_cxt.output_backtrace_log = false;
        btLock.unLock();
        return -1;
    }

    for (int i = 0; i < len_symbols; i++) {
        appendStringInfoString(pOutBuf, t_thrd.log_cxt.thd_bt_symbol[i]);
        appendStringInfoString(pOutBuf, "\n");
    }
    appendStringInfoString(pOutBuf, "Use addr2line to get pretty function name and line\n");

    /*
     * If above code longjmp, we should free this pointer when call this function again.
     * for normal case, free it when exit from function.
     */
    free(t_thrd.log_cxt.thd_bt_symbol);
    t_thrd.log_cxt.thd_bt_symbol = NULL;
    t_thrd.log_cxt.output_backtrace_log = false;
    btLock.unLock();

    return 0;
}

/*
 * Write error report to server's log
 */
static void send_message_to_server_log(ErrorData *edata) {}

/*
 * @Description: Write error report to server's log in a simple way without errstack
 * @param: elevel, fmt
 * @return: void
 */

/*
 * @Description: Write error report to server's log for stream thread
 *
 * @param: void
 * @return: void
 */


/*
 * @Description: Write error report to client for stream thread
 *
 * @param: void
 * @return: void
 */


/*
 * Send data to the syslogger using the chunked protocol
 *
 * Note: when there are multiple backends writing into the syslogger pipe,
 * it's critical that each write go into the pipe indivisibly, and not
 * get interleaved with data from other processes.	Fortunately, the POSIX
 * spec requires that writes to pipes be atomic so long as they are not
 * more than PIPE_BUF bytes long.  So we divide long messages into chunks
 * that are no more than that length, and send one chunk per write() call.
 * The collector process knows how to reassemble the chunks.
 *
 * Because of the atomic write requirement, there are only two possible
 * results from write() here: -1 for failure, or the requested number of
 * bytes.  There is not really anything we can do about a failure; retry would
 * probably be an infinite loop, and we can't even report the error usefully.
 * (There is noplace else we could send it!)  So we might as well just ignore
 * the result from write().  However, on some platforms you get a compiler
 * warning from ignoring write()'s result, so do a little dance with casting
 * rc to void to shut up the compiler.
 */


/*
 * Append a text string to the error report being built for the client.
 *
 * This is ordinarily identical to pq_sendstring(), but if we are in
 * error recursion trouble we skip encoding conversion, because of the
 * possibility that the problem is a failure in the encoding conversion
 * subsystem itself.  Code elsewhere should ensure that the passed-in
 * strings will be plain 7-bit ASCII, and thus not in need of conversion,
 * in such cases.  (In particular, we disable localization of error messages
 * to help ensure that's true.)
 */


/*
 * Brief		: Write only message to client, is_putline is to declare the
 * 			  caller is dbe_output.print_line or dbe_output.print
 */


/* Get internal error code by the location(filename and lineno) of error message arised */


/**
 * @Description: cn add all error info from dn
 * @in/out pErrData - remote error data from dn
 * @return - 0 is ok
 */


/*
 * Write error report to client
 */
void send_message_to_frontend(ErrorData *edata) {}


/*
 * Support routines for formatting error messages.
 */

/*
 * expand_fmt_string --- process special format codes in a format string
 *
 * We must replace %m with the appropriate strerror string, since vsnprintf
 * won't know what to do with it.
 *
 * The result is a palloc'd string.
 */
static char* expand_fmt_string(const char* fmt, ErrorData* edata)
{
    StringInfoData buf;
    const char* cp = NULL;

    initStringInfo(&buf);

    for (cp = fmt; *cp; cp++) {
        if (cp[0] == '%' && cp[1] != '\0') {
            cp++;
            if (*cp == 'm') {
                /*
                 * Replace %m by system error string.  If there are any %'s in
                 * the string, we'd better double them so that vsnprintf won't
                 * misinterpret.
                 */
                const char* cp2 = NULL;

                cp2 = useful_strerror(edata->saved_errno);
                for (; *cp2; cp2++) {
                    if (*cp2 == '%')
                        appendStringInfoCharMacro(&buf, '%');
                    appendStringInfoCharMacro(&buf, *cp2);
                }
            } else {
                /* copy % and next char --- this avoids trouble with %%m */
                appendStringInfoCharMacro(&buf, '%');
                appendStringInfoCharMacro(&buf, *cp);
            }
        } else
            appendStringInfoCharMacro(&buf, *cp);
    }

    return buf.data;
}

/*
 * A slightly cleaned-up version of strerror()
 */
static const char* useful_strerror(int errnum)
{
    /* this buffer is only used if errno has a bogus value */
    char* errorstr_buf = t_thrd.buf_cxt.errorstr_buf;
    int size = sizeof(t_thrd.buf_cxt.errorstr_buf);
    const char* str = NULL;
    int infolen = 0;
    errno_t rc = EOK;

#ifdef WIN32
    /* Winsock error code range, per WinError.h */
    if (errnum >= 10000 && errnum <= 11999)
        return pgwin32_socket_strerror(errnum);
#endif
    str = gs_strerror(errnum);

    /*
     * Some strerror()s return an empty string for out-of-range errno. This is
     * ANSI C spec compliant, but not exactly useful.
     */
    if (str == NULL || *str == '\0') {
        infolen = strlen("operating system error ") + sizeof(int) + 1;
        rc = snprintf_s(errorstr_buf,
            size,
            infolen,
            /* ------
              translator: This string will be truncated at 47
              characters expanded. */
            _("operating system error %d"),
            errnum);
        securec_check_ss_c(rc, "\0", "\0");
        str = errorstr_buf;
    }

    return str;
}

/*
 * error_severity --- get localized string representing elevel
 */


/*
 *	append_with_tabs
 *
 *	Append the string to the StringInfo buffer, inserting a tab after any
 *	newline.
 */


/*
 * Reaper -- get current time.
 */




/*
 * Write errors to stderr (or by equal means when stderr is
 * not available). Used before ereport/elog can be used
 * safely (memory context, GUC load etc)
 */

void
write_stderr(const char *fmt,...)
{
	va_list	ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fflush(stderr);
	va_end(ap);
}



/*
 * is_log_level_output -- is elevel logically >= log_min_level?
 *
 * We use this for tests that should consider LOG to sort out-of-order,
 * between ERROR and FATAL.  Generally this is the right thing for testing
 * whether a message should go to the postmaster log, whereas a simple >=
 * test is correct for testing whether the message should go to the client.
 */
static bool is_log_level_output(int elevel, int log_min_level)
{
    if (elevel == LOG || elevel == COMMERROR) {
        if (log_min_level == LOG || log_min_level <= ERROR)
            return true;
    }
    /* Neither is LOG */
    else if (elevel >= log_min_level)
        return true;

    return false;
}

/*
 * Adjust the level of a recovery-related message per u_sess->attr.attr_common.trace_recovery_messages.
 *
 * The argument is the default log level of the message, eg, DEBUG2.  (This
 * should only be applied to DEBUGn log messages, otherwise it's a no-op.)
 * If the level is >= u_sess->attr.attr_common.trace_recovery_messages, we return LOG, causing the
 * message to be logged unconditionally (for most settings of
 * log_min_messages).  Otherwise, we return the argument unchanged.
 * The message will then be shown based on the setting of log_min_messages.
 *
 * Intention is to keep this for at least the whole of the 9.0 production
 * release, so we can more easily diagnose production problems in the field.
 * It should go away eventually, though, because it's an ugly and
 * hard-to-explain kluge.
 */




/*
 * When the SQL statement is truncated, this function cannot perform normal password masking.
 * maskPassword will return null if the statement does not need to be masked or any error occurs.
 */







 














/*
 * Mask the password in statment CREATE ROLE, CREATE USER, ALTER ROLE, ALTER USER, CREATE GROUP
 * SET ROLE, CREATE DATABASE LINK, and some function
 */




/*
 * Report error according to the return value.
 * At the same time, we should free the space alloced by developers.
 */
void freeSecurityFuncSpace(const char* charList, ...)
{
    va_list argptr;

    /* if the first parameter is not empty */
    if (strcmp(charList, "\0") != 0) {
        /* free the first charList */
        pfree_ext(charList);

        /* if have move charList */
        va_start(argptr, charList);
        while (true) {
            char* szBuf = va_arg(argptr, char*);
            if (strcmp(szBuf, "\0") == 0) /* empty string */
                break;
            pfree_ext(szBuf);
        }
        va_end(argptr);
    }

    return;
}

/*
 * @Description: mask part of espaced characters which may cause log injection attack.
 * @in source_str : the messages which need mask before write to syslog.
 * @return : non-return
 */





ErrorDataArea *initErrorDataArea()
{
    ErrorDataArea *errorDataArea = (ErrorDataArea *)palloc0(sizeof(ErrorDataArea));
    errorDataArea->current_edata_count_by_level = (uint64 *)palloc0(4 * sizeof(uint64));
    errorDataArea->sqlErrorDataList = NIL;
    errorDataArea->current_edata_count = 0;
    for (int i = 0; i <= enum_dolphin_error_level::B_END; i++) {
        errorDataArea->current_edata_count_by_level[i] = 0;
    }
    return errorDataArea;
}
















#define NUM_SHOW_WARNINGS_COLUMNS 3

typedef struct {
    ErrorDataArea *errorDataArea;
    int currIdx;
    int limit;
    ListCell *lc = NULL;
} ErrorDataAreaStatus;










