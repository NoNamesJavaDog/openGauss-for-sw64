/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - MemoryTrackingDetailInfo
 * - MemoryTrackingBufToFile
 * - MemoryTrackingFreeInfo
 * - MemoryTrackingCreate
 * - MemoryTrackingAllocInfo
 *--------------------------------------------------------------------
 */

/*
 * Copyright (c) 2020 Huawei Technologies Co.,Ltd.
 *
 * openGauss is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 *
 * memtrack.cpp
 *
 * Used to track the memory usage:
 *   the peak memory of operator level;
 *   all memory context allocated by the query;
 *   the debug information of all allocation call of one memory context.
 *
 * IDENTIFICATION
 *    src/common/backend/utils/mmgr/memtrack.cpp
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"
#include "knl/knl_variable.h"
#include "miscadmin.h"
#include "utils/aset.h"
#include "utils/atomic.h"
#include "utils/memprot.h"
#include "utils/memtrack.h"
#include "postmaster/syslogger.h"
#include "pgxc/pgxc.h"
#include "tcop/tcopprot.h"
#include "executor/executor.h"

#define QUERY_LOG "query"
#define DEBUG_QUERY_LOG "debug_query"
#define MAX_STRING_BUF (50 * 1024 * 1024)  // 50MB



#ifdef MEMORY_CONTEXT_CHECKING

void MemoryTrackingBufToFile(StringInfoData* memoryBuf, const char* type);

/*
 * function: MemoryTrackingDetailInfo
 * description: Track the detail allocation information
 * arguments:
 *     context: memory context
 *     reqSize: requested size of palloc()
 *     chunkSize: the actual size of one chunk
 *     file: the file name when palloc() is called
 *     line: the line number when palloc() is called
 *
 */
void MemoryTrackingDetailInfo(MemoryContext context, Size reqSize, Size chunkSize, const char* file, int line)
{
    /* don't update the memory tracking when it is invalid */
    if (context->session_id > 0 || !t_thrd.mem_cxt.mem_track_mem_cxt)
        return;

    MemoryTrack track;
#ifndef ENABLE_MEMORY_CHECK
    AllocSet set = (AllocSet)context;
#else
    AsanSet set = (AsanSet)context;
#endif

    AssertArg(MemoryContextIsValid(context));

    track = set->track;

    if (track && track->isTracking) {
        /* Switch to t_thrd.mem_cxt.mem_track_mem_cxt to allocate memory */
        MemoryContext old = MemoryContextSwitchTo(t_thrd.mem_cxt.mem_track_mem_cxt);

        appendStringInfo(&(*t_thrd.utils_cxt.detailTrackingBuf), "%s:%d, %ld, %ld\n", file, line, chunkSize, reqSize);

        /* generate the file when data size reaches 50MB */
        if ((*t_thrd.utils_cxt.detailTrackingBuf).len > MAX_STRING_BUF) {
            MemoryTrackingBufToFile(&(*t_thrd.utils_cxt.detailTrackingBuf), DEBUG_QUERY_LOG);

            pfree((*t_thrd.utils_cxt.detailTrackingBuf).data);

            initStringInfo(&(*t_thrd.utils_cxt.detailTrackingBuf));
        }

        (void)MemoryContextSwitchTo(old);
    }
}

/*
 * function: MemoryTrackingParseGUC
 * description: parse the guc argument of u_sess->attr.attr_memory.memory_detail_tracking
 * arguments:
 *    val: the input string
 */


#endif

/*
 * function: MemoryTrackingInit
 * description: Init the memory tracking information
 */


/*
 * function: MemoryTrackingCreate
 * description: Allocate the memory tracking element and add it into tree path
 * arguments:
 *    context: the current memory context
 *    parent: the parent memory context
 */
void MemoryTrackingCreate(MemoryContext context, MemoryContext parent)
{
    MemoryContext old = MemoryContextSwitchTo(t_thrd.mem_cxt.mem_track_mem_cxt);
    MemoryTrack track;
#ifndef ENABLE_MEMORY_CHECK
    AllocSet set = (AllocSet)context;
    AllocSet pset = (AllocSet)parent;
#else
    AsanSet set = (AsanSet)context;
    AsanSet pset = (AsanSet)parent;
#endif
    errno_t rc = EOK;

    /* allocate a memory tracking element to match this context */
    track = (MemoryTrack)palloc0(sizeof(MemoryTrackData) + strlen(context->name) + 1);
    track->type = T_MemoryTracking;
    track->name = (char*)track + sizeof(MemoryTrackData);
    rc = strcpy_s(track->name, strlen(context->name) + 1, context->name);
    securec_check_c(rc, "\0", "\0");
    track->sequentCount = t_thrd.utils_cxt.mctx_sequent_count++;

#ifdef MEMORY_CONTEXT_CHECKING
    /* check if 'u_sess->attr.attr_memory.memory_detail_tracking' is the right context */
    if (track->sequentCount == t_thrd.utils_cxt.memory_track_sequent_count &&
        ExecGetPlanNodeid() == t_thrd.utils_cxt.memory_track_plan_nodeid) {
        track->isTracking = true;

        /* initialize the buffer for storing the allocation information */
        initStringInfo(&(*t_thrd.utils_cxt.detailTrackingBuf));
    }

#endif

    /* add the memory tracking into parent's tree path */
    if (pset && pset->track) {
        track->parent = pset->track;
        track->nextchild = pset->track->firstchild;
        pset->track->firstchild = track;
    }

    set->track = track;

    (void)MemoryContextSwitchTo(old);
}

/*
 * function: MemoryTrackingAllocInfo
 * description: Update the memory tracking information when allocating memory
 * arguments:
 *    context: the current memory context
 *    size: the allocation size when malloc() is called
 *
 */
void MemoryTrackingAllocInfo(MemoryContext context, Size size)
{
    /* don't update the memory tracking when it is invalid */
    if (!t_thrd.mem_cxt.mem_track_mem_cxt)
        return;

    MemoryTrack track;
    Size held;
#ifndef ENABLE_MEMORY_CHECK
    AllocSet set = (AllocSet)context;
#else
    AsanSet set = (AsanSet)context;
#endif

    AssertArg(MemoryContextIsValid(context));

    track = set->track;

    /* update the peakSpace for this context */
    if (set->totalSpace > track->peakSpace)
        track->peakSpace = set->totalSpace;

    while (track) {
        track->allBytesAlloc += size;

        /* update the allBytesPeak for this and its children's context */
        held = (Size)(track->allBytesAlloc - track->allBytesFreed);
        if (track->allBytesPeak < held)
            track->allBytesPeak = held;

        track = track->parent;
    }
}

/*
 * function: MemoryTrackingFreeInfo
 * description: Update the memory tracking information when freeing memory
 * arguments:
 *    context: the current memory context
 *    size: the allocation size when free() is called
 */
void MemoryTrackingFreeInfo(MemoryContext context, Size size)
{
    /* don't update the memory tracking when it is invalid */
    if (!t_thrd.mem_cxt.mem_track_mem_cxt || t_thrd.proc_cxt.proc_exit_inprogress)
        return;

    AssertArg(MemoryContextIsValid(context));

#ifndef ENABLE_MEMORY_CHECK
    AllocSet set = (AllocSet)context;
#else
    AsanSet set = (AsanSet)context;
#endif
    MemoryTrack track = set->track;

    while (track) {
        track->allBytesFreed += size;

        track = track->parent;
    }
}

/*
 * function: MemoryTrackingBufToFile
 * description: to create the output file of Memory Tracking information
 * arguments:
 *    memoryBuf: the string buffer to store the output information
 *    type: the type name to indicate which logging file is created
 */
void MemoryTrackingBufToFile(StringInfoData* memoryBuf, const char* type)
{
    int rc = 0;
    char tempdirpath[1024] = {0};
    FILE* fp = NULL;
    bool is_absolute = false;

    is_absolute = is_absolute_path(u_sess->attr.attr_common.Log_directory);
    if (is_absolute)
        rc = snprintf_s(tempdirpath,
            sizeof(tempdirpath),
            sizeof(tempdirpath) - 1,
            "%s/memory_track_%s_%s_%lu.csv",
            u_sess->attr.attr_common.Log_directory,
            g_instance.attr.attr_common.PGXCNodeName,
            type,
            u_sess->debug_query_id);
    else
        rc = snprintf_s(tempdirpath,
            sizeof(tempdirpath),
            sizeof(tempdirpath) - 1,
            "%s/pg_log/memory_track_%s_%s_%lu.csv",
            g_instance.attr.attr_common.data_directory,
            g_instance.attr.attr_common.PGXCNodeName,
            type,
            u_sess->debug_query_id);

    securec_check_ss(rc, "\0", "\0");

    fp = fopen(tempdirpath, "a+");
    if (fp == NULL) {
        ereport(LOG, (errmsg("Could not write memory usage information. Failed to open file: %s", tempdirpath)));
        return;
    }

    rc = chmod(tempdirpath, 0600);
    if (rc != 0) {
        fclose(fp);
        ereport(LOG, (errmsg("Could not change permission of file: %s", tempdirpath)));
        return;
    }

    uint64 bytes = fwrite(memoryBuf->data, 1, memoryBuf->len, fp);
    if (bytes != (uint64)memoryBuf->len) {
        ereport(LOG, (errmsg("Could not write memory usage information. Attempted to write %d", memoryBuf->len)));
    }

    fclose(fp);
}

/*
 * function: MemoryTrackingSaveToBuf
 * description: to append the output information into file
 * arguments:
 *    track: the memory track structure
 *    level: the level in the tree path
 *    pcnt: the parent count
 *    key: the plan nodeid in stream thread
 *    str: the string buffer
 */


/*
 * function: MemoryTrackingLoggingToFile
 * description: to save the memory tracking inforamtion into log file
 * arguments:
 *    track: the memory track structure
 *    key: the plan nodeid in stream thread
 */


#ifdef MEMORY_CONTEXT_CHECKING
/*
 * function: MemoryTrackingDetailInfoToFile
 * description: to save the detail memory tracking inforamtion into log file
 */

#endif

/*
 * function: MemoryTrackingNodeFreeChildren
 * description: to recursively free the memory track structure
 * arguments:
 *     track: memory track structure
 */


/*
 * function: MemoryTrackingNodeFree
 * description: to free the memory track structure
 * arguments:
 *     track: memory track structure
 */


/*
 * function: MemoryTrackingOutputFile
 * description: to output the memory tracking information into file and
 *              release the memory tracking structure
 */
#ifdef MEMORY_CONTEXT_CHECKING
#endif
