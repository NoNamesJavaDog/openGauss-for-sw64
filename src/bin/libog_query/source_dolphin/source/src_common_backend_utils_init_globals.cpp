/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - PostmasterPid
 * - CURSOR_EXPRESSION_VERSION_NUMBER
 * - KEYWORD_IGNORE_COMPART_VERSION_NUM
 * - SRF_FUSION_VERSION_NUM
 * - SEGMENT_PAGE_VERSION_NUM
 * - SLOW_SQL_VERSION_NUM
 * - SUBPARTITION_VERSION_NUM
 * - PUBLICATION_DDL_VERSION_NUM
 * - ML_OPT_MODEL_VERSION_NUM
 * - PARALLEL_ENABLE_VERSION_NUM
 * - REPLACE_INTO_VERSION_NUM
 * - MULTI_MODIFY_VERSION_NUM
 * - SUPPORT_VIEW_AUTO_UPDATABLE
 * - SWCB_VERSION_NUM
 * - INPLACE_UPDATE_VERSION_NUM
 * - SCAN_BATCH_MODE_VERSION_NUM
 * - EXECUTE_DIRECT_ON_MULTI_VERSION_NUM
 * - FIX_PBE_CUSTOME_PLAN_BUG_VERSION_NUM
 * - FIX_SQL_ADD_RELATION_REF_COUNT
 * - PLAN_SELECT_VERSION_NUM
 * - FDW_SUPPORT_JOIN_AGG_VERSION_NUM
 * - INNER_UNIQUE_VERSION_NUM
 * - CHARACTER_SET_VERSION_NUM
 * - SUBLINKPULLUP_VERSION_NUM
 * - FETCH_ENHANCE_VERSION_NUM
 * - SKIP_LOCKED_VERSION_NUM
 * - WAIT_N_TUPLE_LOCK_VERSION_NUM
 * - MULTI_PARTITIONS_VERSION_NUM
 * - INDEX_HINT_VERSION_NUM
 * - MATVIEW_VERSION_NUM
 * - SELECT_INTO_VAR_VERSION_NUM
 * - SELECT_INTO_FILE_VERSION_NUM
 * - CREATE_TABLE_AS_VERSION_NUM
 * - COMMENT_ROWTYPE_TABLEOF_VERSION_NUM
 * - COMMENT_RECORD_PARAM_VERSION_NUM
 * - COMMENT_ROWTYPE_NEST_TABLEOF_VERSION_NUM
 * - PARAM_MARK_VERSION_NUM
 * - KEEP_FUNC_VERSION_NUMBER
 * - TIMESCALE_DB_VERSION_NUM
 * - ROTATE_UNROTATE_VERSION_NUM
 * - IGNORE_NULLS_VERSION_NUMBER
 * - CLIENT_ENCRYPTION_PROC_VERSION_NUM
 * - CAST_FUNC_VERSION_NUMBER
 * - HASH_SAOP_VERSION_NUMBER
 * - MINMAXEXPR_CMPTYPE_VERSION_NUM
 * - UNION_NULL_VERSION_NUM
 * - MAT_VIEW_RECURSIVE_VERSION_NUM
 * - STRAIGHT_JOIN_VERSION_NUMBER
 * - APPLY_JOIN_VERSION_NUMBER
 * - ASOFJOIN_VERSION_NUM
 * - COMMENT_SUPPORT_VERSION_NUM
 * - PARTITION_ENHANCE_VERSION_NUM
 * - CREATE_INDEX_IF_NOT_EXISTS_VERSION_NUM
 * - SUPPORT_GPI_VERSION_NUM
 * - DISABLE_CONSTRAINT_VERSION_NUM
 * - INSERT_INTO_SELECT_VERSION_NUM
 * - GENERATED_COL_VERSION_NUM
 * - ON_UPDATE_TIMESTAMP_VERSION_NUM
 * - PUBLICATION_DDL_AT_VERSION_NUM
 * - COMMENT_PCT_TYPE_VERSION_NUM
 * - PARTIALPUSH_VERSION_NUM
 * - INSERT_RIGHT_REF_VERSION_NUM
 * - SELECT_STMT_HAS_USERVAR
 * - ENHANCED_TUPLE_LOCK_VERSION_NUM
 * - UPSERT_ALIAS_VERSION_NUM
 * - UPSERT_WHERE_VERSION_NUM
 * - MATERIALIZED_CTE_NUM
 * - DEFAULT_MAT_CTE_NUM
 * - PREDPUSH_VERSION_NUM
 * - PARTITION_NAME_VERSION_NUM
 * - FUNC_PARAM_COL_VERSION_NUM
 * - COPY_TRANSFORM_VERSION_NUM
 * - PBESINGLEPARTITION_VERSION_NUM
 * - HINT_ENHANCEMENT_VERSION_NUM
 * - PREDPUSH_SAME_LEVEL_VERSION_NUM
 * - SQL_PATCH_VERSION_NUM
 * - SUPPORT_GS_DEPENDENCY_VERSION_NUM
 * - CREATE_FUNCTION_DEFINER_VERSION
 * - PUBLIC_SYNONYM_VERSION_NUMBER
 * - PUBLICATION_INITIAL_DATA_VERSION_NAME
 * - IsPostmasterEnvironment
 *--------------------------------------------------------------------
 */

/*-------------------------------------------------------------------------
 *
 * globals.c
 *	  global variable declarations
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/init/globals.c
 *
 * NOTES
 *	  Globals used all over the place should be declared here and not
 *	  in other modules.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "knl/knl_variable.h"

#include "access/xact.h"
#include "catalog/objectaccess.h"
#include "libpq/pqcomm.h"
#include "miscadmin.h"
#include "storage/backendid.h"
#include "utils/formatting.h"
#include "utils/atomic.h"
#include "replication/slot.h"





 /* full path to my executable */

/*
 * Hook on object accesses.  This is intended as infrastructure for security
 * and logging plugins.
 */




/*
 * standalone backend).  IsUnderPostmaster is true in postmaster child
 * processes.  Note that "child process" includes all children, not only
 * regular backends.  These should be set correctly as early as possible
 * in the execution of a process, so that error handling will do the right
 * things if an error should occur during process initialization.
 *
 * These are initialized for the bootstrap/standalone case.
 */


 volatile ThreadId PostmasterPid = 0;

 bool IsPostmasterEnvironment = false;





/********************************************
 * 1.HARD-WIRED BINARY VERSION NUMBER
 *
 *    version num range detail for openGauss:
 *      version | start at | release at | reserve to
 *      --------+----------+------------+------------
 *       1.0.X  |  00000   |   92072    |   92086   
 *       1.1.X  |  92087   |   92298    |   92298   
 *       2.0.X  |  92298   |   92298    |   92303   
 *       2.1.X  |  92304   |   92421    |   92423   
 *       3.0.X  |  92424   |   92605    |   92655   
 *       3.1.X  |    -     |     -      |     -     
 *       5.0.X  |  92656   |   92848    |   92898   
 *       6.0.X  |  92899   |   92950    |   92999
 *       NEXT   |  93000   |     ?      |     ?
 *
 ********************************************/



/********************************************
 * 2.VERSION NUM FOR EACH FEATURE
 *   Please write indescending order.
 ********************************************/



const uint32 HASH_SAOP_VERSION_NUMBER = 93027;
const uint32 CAST_FUNC_VERSION_NUMBER = 93026;
const uint32 ASOFJOIN_VERSION_NUM = 93028;
const uint32 IGNORE_NULLS_VERSION_NUMBER = 93025;


const uint32 KEEP_FUNC_VERSION_NUMBER = 93014;
const uint32 PUBLIC_SYNONYM_VERSION_NUMBER = 93013;
const uint32 FETCH_ENHANCE_VERSION_NUM = 93012;
const uint32 APPLY_JOIN_VERSION_NUMBER = 93011;


const uint32 PUBLICATION_DDL_AT_VERSION_NUM = 92949;
const uint32 MINMAXEXPR_CMPTYPE_VERSION_NUM = 92948;


const uint32 PARTITION_NAME_VERSION_NUM = 92947;



const uint32 PARALLEL_ENABLE_VERSION_NUM = 92941;

const uint32 STRAIGHT_JOIN_VERSION_NUMBER = 92939;
const uint32 INSERT_INTO_SELECT_VERSION_NUM = 92938;
const uint32 ROTATE_UNROTATE_VERSION_NUM = 92937;

const uint32 DISABLE_CONSTRAINT_VERSION_NUM = 92931;



const uint32 PUBLICATION_DDL_VERSION_NUM = 92921;
const uint32 UPSERT_ALIAS_VERSION_NUM = 92920;
const uint32 SUPPORT_GS_DEPENDENCY_VERSION_NUM = 92916;






const uint32 PARAM_MARK_VERSION_NUM = 92907;
const uint32 TIMESCALE_DB_VERSION_NUM = 92904;



const uint32 SRF_FUSION_VERSION_NUM = 92847;
const uint32 INDEX_HINT_VERSION_NUM = 92845;
const uint32 INNER_UNIQUE_VERSION_NUM = 92845;
const uint32 CREATE_TABLE_AS_VERSION_NUM = 92845;

const uint32 SLOW_SQL_VERSION_NUM = 92844;
const uint32 CHARACTER_SET_VERSION_NUM = 92844;
const uint32 SELECT_INTO_FILE_VERSION_NUM = 92844;
const uint32 PARTITION_ENHANCE_VERSION_NUM = 92844;
const uint32 CREATE_INDEX_IF_NOT_EXISTS_VERSION_NUM = 92843;

const uint32 INSERT_RIGHT_REF_VERSION_NUM = 92842;
const uint32 UNION_NULL_VERSION_NUM = 92841;
const uint32 FDW_SUPPORT_JOIN_AGG_VERSION_NUM = 92839;
const uint32 SUPPORT_VIEW_AUTO_UPDATABLE = 92838;

const uint32 SELECT_INTO_VAR_VERSION_NUM = 92834;
const uint32 MAT_VIEW_RECURSIVE_VERSION_NUM = 92833;


const uint32 SKIP_LOCKED_VERSION_NUM = 92829;
const uint32 REPLACE_INTO_VERSION_NUM = 92828;

const uint32 PLAN_SELECT_VERSION_NUM = 92826;
const uint32 MULTI_PARTITIONS_VERSION_NUM = 92825;
const uint32 MULTI_MODIFY_VERSION_NUM = 92814;

const uint32 SELECT_STMT_HAS_USERVAR = 92924;

const uint32 SQL_PATCH_VERSION_NUM = 92675;

const uint32 ON_UPDATE_TIMESTAMP_VERSION_NUM = 92664;

const uint32 COMMENT_SUPPORT_VERSION_NUM = 92662;
const uint32 KEYWORD_IGNORE_COMPART_VERSION_NUM = 92659;

const uint32 CREATE_FUNCTION_DEFINER_VERSION = 92658;
const uint32 PUBLICATION_INITIAL_DATA_VERSION_NAME = 92657;
const uint32 COMMENT_ROWTYPE_NEST_TABLEOF_VERSION_NUM = 92657;







const uint32 ENHANCED_TUPLE_LOCK_VERSION_NUM = 92583;


const uint32 WAIT_N_TUPLE_LOCK_VERSION_NUM = 92573;

const uint32 SCAN_BATCH_MODE_VERSION_NUM = 92568;



const uint32 PBESINGLEPARTITION_VERSION_NUM = 92523;
const uint32 PREDPUSH_SAME_LEVEL_VERSION_NUM = 92522;
const uint32 UPSERT_WHERE_VERSION_NUM = 92514;
const uint32 COMMENT_ROWTYPE_TABLEOF_VERSION_NUM = 92513;

const uint32 FUNC_PARAM_COL_VERSION_NUM = 92500;

const uint32 COMMENT_RECORD_PARAM_VERSION_NUM = 92484;

const uint32 SUBPARTITION_VERSION_NUM = 92436;
const uint32 DEFAULT_MAT_CTE_NUM = 92429;
const uint32 SWCB_VERSION_NUM = 92427;
const uint32 MATERIALIZED_CTE_NUM = 92424;





const uint32 COMMENT_PCT_TYPE_VERSION_NUM = 92396;
const uint32 COPY_TRANSFORM_VERSION_NUM = 92394;

const uint32 CLIENT_ENCRYPTION_PROC_VERSION_NUM = 92383;

const uint32 SEGMENT_PAGE_VERSION_NUM = 92360;
const uint32 HINT_ENHANCEMENT_VERSION_NUM = 92359;

const uint32 GENERATED_COL_VERSION_NUM = 92355;
const uint32 INPLACE_UPDATE_VERSION_NUM = 92350;



const uint32 FIX_SQL_ADD_RELATION_REF_COUNT = 92291;
const uint32 ML_OPT_MODEL_VERSION_NUM = 92284;




const uint32 SUPPORT_GPI_VERSION_NUM = 92257;

const uint32 MATVIEW_VERSION_NUM = 92213;


const uint32 FIX_PBE_CUSTOME_PLAN_BUG_VERSION_NUM = 92148;
const uint32 EXECUTE_DIRECT_ON_MULTI_VERSION_NUM = 92140;


const uint32 PREDPUSH_VERSION_NUM = 92096;
const uint32 SUBLINKPULLUP_VERSION_NUM = 92094;
const uint32 PARTIALPUSH_VERSION_NUM = 92087;

const uint32 CURSOR_EXPRESSION_VERSION_NUMBER = 92935;



/* This variable indicates wheather the instance is in progress of upgrade as a whole */



#ifdef PGXC

#endif

/* allow to store tables in segment storage while initdb */


/*
 *     EarlyBindingTLSVariables
 *         Bind static variables to another static TLS variable's address.
 *
 *	   This is needed because of the inability of the compiler: compiler
 *	   complains if you intialize a static TLS variable as another TLS
 *	   variable's address. So we do it for compiler in the earilest stage
 *	   of thread starting once.
 */

