/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - GetSessionContext
 *--------------------------------------------------------------------
 */

#include "postgres.h"
#include "plugin_nodes/parsenodes_common.h"
#include "plugin_parser/parser.h"
#include "plugin_parser/analyze.h"
#include "plugin_parser/parse_oper.h"
#include "plugin_storage/hash.h"
#include "plugin_postgres.h"
#include "plugin_utils/plpgsql.h"
#include "plugin_optimizer/planner.h"
#include "commands/extension.h"
#include "commands/dbcommands.h"
#include "commands/copy.h"
#include "commands/tablespace.h"
#include "commands/comment.h"
#include "commands/prepare.h"
#include "commands/alter.h"
#include "commands/directory.h"
#include "commands/user.h"
#include "commands/conversioncmds.h"
#include "commands/discard.h"
#include "commands/lockcmds.h"
#include "commands/typecmds.h"
#include "commands/shutdown.h"
#include "commands/proclang.h"
#include "commands/async.h"
#include "commands/sequence.h"
#include "commands/createas.h"
#include "commands/view.h"
#include "commands/portalcmds.h"
#include "commands/collationcmds.h"
#include "commands/schemacmds.h"
#include "commands/sec_rls_cmds.h"
#include "commands/seclabel.h"
#include "commands/subscriptioncmds.h"
#include "commands/publicationcmds.h"
#include "commands/cluster.h"
#include "commands/matview.h"
#include "utils/guc.h"
#include "plugin_mb/pg_wchar.h"
#include "utils/builtins.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_synonym.h"
#include "catalog/gs_matview.h"
#include "catalog/gs_db_privilege.h"
#include "catalog/pg_extension.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_attribute.h"
#include "executor/spi_priv.h"
#include "tcop/utility.h"
#include "gs_ledger/ledger_utils.h"
#include "storage/lmgr.h"
#include "utils/inval.h"
#include "utils/lsyscache.h"
#include "utils/acl.h"
#include "utils/knl_catcache.h"
#include "plugin_utils/date.h"
#include "utils/nabstime.h"
#include "utils/geo_decls.h"
#include "utils/varbit.h"
#include "utils/json.h"
#include "utils/jsonb.h"
#include "utils/xml.h"
#include "pgxc/groupmgr.h"
#include "libpq/pqformat.h"
#include "optimizer/nodegroups.h"
#include "plugin_optimizer/pgxcplan.h"
#include "pgstat.h"
#include "client_logic/client_logic.h"
#include "db4ai/create_model.h"



#include "pgxc/poolutils.h"
#include "access/reloptions.h"
#include "access/cstore_delta.h"
#include "access/twophase.h"
#include "rewrite/rewriteDefine.h"
#include "storage/procarray.h"
#include "storage/tcap.h"
#include "plugin_parser/parse_func.h"
#include "plugin_parser/parse_utilcmd.h"
#include "plugin_vector/vecfunc.h"
#include "replication/archive_walreceiver.h"
#include "plugin_commands/mysqlmode.h"
#include "plugin_protocol/startup.h"
#include "libpq/libpq.h"
#include "tcop/ddldeparse.h"
#include "plugin_protocol/printtup.h"
#include "plugin_protocol/dqformat.h"
#ifdef DOLPHIN
#include "plugin_utils/my_locale.h"
#include "plugin_executor/functions.h"
#include "plugin_utils/varbit.h"
#include "plugin_commands/defrem.h"
#include "utils/biginteger.h"
#endif
#ifndef WIN32_ONLY_COMPILER
#include "dynloader.h"
#else
#include "port/dynloader/win32.h"
#endif

typedef struct sql_mode_entry {
    const char* name; /* name of sql mode entry */
    int flag;         /* bit flag position */
} sql_mode_entry;



#define DOLPHIN_TYPES_NUM 12
#define TYPE_ATTRIBUTES_NUM 3
#define NAMEEQ_FN_OID 62
/* typname, enable precision, enable scale */

/*
 * For loading plpgsql function.
 */
typedef struct {
    char* func_name;
    PGFunction func_addr;
} RegExternFunc;

;

extern void InitLockNameHash();
extern struct HTAB* lockNameHash;
extern pthread_mutex_t gNameHashLock;
extern void initBSQLBuiltinFuncs();
extern struct HTAB* b_nameHash;
extern struct HTAB* b_oidHash;
extern RegExternFunc b_plpgsql_function_table[3];
extern int tmp_b_fmgr_nbuiltins;
extern FmgrBuiltin tmp_b_fmgr_builtins[];
extern void deparse_query(Query* query, StringInfo buf, List* parentnamespace, bool finalise_aggs, bool sortgroup_colno,
    void* parserArg, bool qrw_phase, bool is_fqs);
extern bool isAllTempObjects(Node* parse_tree, const char* query_string, bool sent_to_remote);
extern void ts_check_feature_disable();
extern void ExecAlterDatabaseSetStmt(Node* parse_tree, const char* query_string, bool sent_to_remote);
extern void DoVacuumMppTable(VacuumStmt* stmt, const char* query_string, bool is_top_level, bool sent_to_remote);
extern bool IsVariableinBlackList(const char* name);
extern void ExecAlterRoleSetStmt(Node* parse_tree, const char* query_string, bool sent_to_remote);
static bool CheckSqlMode(char** newval, void** extra, GucSource source);
static bool CheckNullsMinimalPolicy(bool* newval, void** extra, GucSource source);
static void AssignSqlMode(const char* newval, void* extra);
static bool check_b_db_timestamp(double* newval, void** extra, GucSource source);
static void assign_b_db_timestamp(double newval, void* extra);
#ifdef DOLPHIN
static const char* show_last_insert_id(void);
static bool check_sql_mode(char** newval, void** extra, GucSource source);
static bool check_lower_case_table_names(int* newval, void** extra, GucSource source);
static bool check_default_week_format(int* newval, void** extra, GucSource source);
static void assign_default_week_format(int newval, void* extra);
static bool check_lc_time_names(char** newval, void** extra, GucSource source);
static bool check_auto_increment_increment(int* newval, void** extra, GucSource source);
static bool check_character_set_client(char** newval, void** extra, GucSource source);
static bool check_character_set_results(char** newval, void** extra, GucSource source);
static bool check_character_set_server(char** newval, void** extra, GucSource source);
static bool check_collation_server(char** newval, void** extra, GucSource source);
static bool check_init_connect(char** newval, void** extra, GucSource source);
static bool check_interactive_timeout(int* newval, void** extra, GucSource source);
static bool check_license(char** newval, void** extra, GucSource source);
static bool check_max_allowed_packet(int* newval, void** extra, GucSource source);
static bool check_net_buffer_length(int* newval, void** extra, GucSource source);
static bool check_net_write_timeout(int* newval, void** extra, GucSource source);
static bool check_query_cache_size(long int* newval, void** extra, GucSource source);
static bool check_query_cache_type(int* newval, void** extra, GucSource source);
static bool check_system_time_zone(char** newval, void** extra, GucSource source);
static bool check_time_zone(char** newval, void** extra, GucSource source);
static bool check_wait_timeout(int* newval, void** extra, GucSource source);
static int SpiIsExecMultiSelect(PLpgSQL_execstate* estate, PLpgSQL_expr* expr,
    PLpgSQL_stmt_execsql* pl_stmt, ParamListInfo paramLI, long tcount, bool* multi_res);
static void SpiMultiSelectException();
extern DestReceiver* dophin_default_printtup_create_DR(CommandDest dest);






#endif


extern void set_hypopg_prehook(ProcessUtility_hook_type func);
extern void set_pgaudit_prehook(ProcessUtility_hook_type func);
extern bool check_plugin_function(Oid funcId);
extern PGFunction SearchFuncByOid(Oid funcId);




extern "C" DLL_PUBLIC void _PG_init(void);
extern "C" DLL_PUBLIC void _PG_fini(void);

;
extern "C" DLL_PUBLIC Datum dolphin_types();
extern "C" Datum dolphin_binaryin(PG_FUNCTION_ARGS);
static void InitDolphinTypeId(BSqlPluginContext* cxt);
static void InitDolphinOperator(BSqlPluginContext* cxt);
static Datum DolphinGetTypeZeroValue(Form_pg_attribute att_tup);
static bool ReplaceNullOrNot();
static bool NoAutoValueOnZero();
static bool NullsMinimalPolicy();
static bool enableProcedureExecutement();

static void* DeparseCollectedCommand(int type, CollectedCommand *cmd, CollectedATSubcmd *sub,
    ddl_deparse_context *context);

;






HeapTuple searchCat(Relation relation, Oid indexoid,
    int cacheId, int nkeys, ScanKeyData* cur_skey, SysScanDesc* scandesc);
bool ccHashEqFuncs(Oid keytype, CCHashFN *hashfunc,
    RegProcedure *eqfunc, CCFastEqualFN *fasteqfunc, int cacheId);
/*
 * NOTE: this function will be called concurently, when you add code here, please make sure your code is thread-safe.
 * If not, please use gInitPluginObjectLock to control your code.
 */














#define IS_CLIENT_CONN_VALID_PROC_SPI(port) \
    (((port) == NULL)                       \
    ? false                                 \
    : (((port)->is_logic_conn) ? ((port)->gs_sock.type != GSOCK_INVALID) : ((port)->sock != NO_SOCKET)))











/*
 * check_behavior_compat_options: GUC check_hook for behavior compat options
 */


/*
 * assign_distribute_test_param: GUC assign_hook for distribute_test_param
 */






#ifdef DOLPHIN












































BSqlPluginContext* GetSessionContext()
{
    int dolphin_index = 0;
    if (!u_sess->attr.attr_common.extension_session_vars_array) {
        int initExtArraySize = 10;
        u_sess->attr.attr_common.extension_session_vars_array =
        (void**)MemoryContextAllocZero(u_sess->self_mem_cxt, (Size)(initExtArraySize * sizeof(void*)));
    }
        
    if (u_sess->attr.attr_common.extension_session_vars_array[dolphin_index] == NULL) {
        init_session_vars();
    }
    return (BSqlPluginContext *) u_sess->attr.attr_common.extension_session_vars_array[dolphin_index];
}





#define LENGTH_OF_KV 2


#endif


void init_session_vars(void)
{
    int dolphin_index = 0;
    RepallocSessionVarsArrayIfNecessary();

    BSqlPluginContext *cxt = (BSqlPluginContext *) MemoryContextAlloc(u_sess->self_mem_cxt, sizeof(bSqlPluginContext));
    u_sess->attr.attr_common.extension_session_vars_array[dolphin_index] = cxt;
    cxt->enableBCmptMode = false;
    cxt->lockNameList = NIL;
    cxt->scan_from_pl = false;
    cxt->is_b_declare = false;
    cxt->default_database_name = NULL;
    cxt->mysql_ca = NULL;
    cxt->mysql_server_cert = NULL;
    cxt->mysql_server_key = NULL;
    cxt->paramIdx = 0;
    cxt->isUpsert = false;
    cxt->single_line_trigger_begin = 0;
    cxt->do_sconst = NULL;
    cxt->single_line_proc_begin = 0;
    cxt->is_schema_name = false;
    cxt->b_stmtInputTypeHash = NULL;
    cxt->b_sendBlobHash = NULL;
    cxt->is_dolphin_call_stmt = false;
    cxt->is_binary_proto = false;
    cxt->is_ast_stmt = false;
    cxt->group_by_error = false;
    cxt->is_create_alter_stmt = false;
    cxt->isDoCopy = false;
    cxt->isInTransformSet = false;
    cxt->is_set_stmt = false;
    cxt->Conn_Mysql_Info = NULL;

    if (temp_Conn_Mysql_Info) {
        cxt->Conn_Mysql_Info = (conn_mysql_infoP_t)MemoryContextAllocZero(u_sess->self_mem_cxt,
                                                                          (Size)(sizeof(conn_mysql_info_t)));
        errno_t rc = memcpy_s(cxt->Conn_Mysql_Info, sizeof(conn_mysql_info_t),
                              temp_Conn_Mysql_Info, sizeof(conn_mysql_info_t));
        securec_check(rc, " ", " ");
        pfree(temp_Conn_Mysql_Info);
        temp_Conn_Mysql_Info = NULL;
    }
    u_sess->attr.attr_sql.sql_compatibility = B_FORMAT;
    u_sess->attr.attr_common.delimiter_name = ";";
    cxt->sqlModeFlags = cxt->sqlModeFlags | OPT_SQL_MODE_ANSI_QUOTES | OPT_SQL_MODE_STRICT |
    					OPT_SQL_MODE_PIPES_AS_CONCAT | OPT_SQL_MODE_NO_ZERO_DATE |
    					OPT_SQL_MODE_PAD_CHAR_TO_FULL_LENGTH | OPT_SQL_MODE_AUTO_RECOMPILE_FUNCTION |
    					OPT_SQL_MODE_ERROR_FOR_DIVISION_BY_ZERO;
    cxt->enableBCmptMode = true;
}







#ifndef ENABLE_LITE_MODE
#endif











/* copy from openGauss-server's execUtils.cpp GetTypeZeroValue */











/* preserved interface for future */


#define OBJTREE_NVAR_3 3




