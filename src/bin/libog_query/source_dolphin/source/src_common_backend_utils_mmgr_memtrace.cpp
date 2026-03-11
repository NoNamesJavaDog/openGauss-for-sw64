/*--------------------------------------------------------------------
 * Symbols referenced in this file:
 * - MemoryContextShouldTrack
 * - RemoveTrackMemoryInfo
 * - InsertTrackMemoryInfo
 * - RemoveTrackMemoryContext
 * - RemoveTrackMemoryContext
 * - RemoveTrackMemoryContext
 * - RemoveTrackMemoryInfo
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
 * memtrace.cpp
 *
 * Used to track the memory usage
 *
 * IDENTIFICATION
 *    src/common/backend/utils/mmgr/memtrace.cpp
 *
 * -------------------------------------------------------------------------
 */

#include "postgres.h"
#include "knl/knl_variable.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/acl.h"
#include "utils/aset.h"
#include "utils/memprot.h"
#include "utils/memtrace.h"
#include "workload/cpwlm.h"

#define MAX_CONTEXT_NUM 16





#define NUM_FREELISTS 32















bool MemoryContextShouldTrack(const char* name)
{
    bool found = false;

    if (g_instance.stat_cxt.track_memory_inited == false) {
        return false;
    }

    /* Check is memory context in track hash table */
    (void)hash_search(g_instance.stat_cxt.track_context_hash, name, HASH_FIND, &found);
    if (found) {
        return true;
    }

    return false;
}

void InsertTrackMemoryInfo(const void* pointer, const MemoryContext context,
                           const char* file, int line, Size size)
{
    MemoryAllocInfo* entry = NULL;
    uint64 key = (uint64)pointer;
    bool found = false;
    errno_t rc;

    if (g_instance.stat_cxt.track_memory_inited == false) {
        return;
    }

    entry = (MemoryAllocInfo*)hash_search(g_instance.stat_cxt.track_memory_info_hash, &key, HASH_ENTER, &found);
    if (!found && entry != NULL) {
        entry->file = file;
        entry->line = line;
        entry->size = size;
        entry->context = context;
        rc = strncpy_s(entry->context_name, NAMEDATALEN, context->name, NAMEDATALEN - 1);
        securec_check(rc, "\0", "\0");
    }
}

void RemoveTrackMemoryInfo(const void* pointer)
{
    uint64 key = (uint64)pointer;

    if (g_instance.stat_cxt.track_memory_inited == false) {
        return;
    }

    (void)hash_search(g_instance.stat_cxt.track_memory_info_hash, &key, HASH_REMOVE, NULL);
}

void RemoveTrackMemoryContext(const MemoryContext context)
{
    HASH_SEQ_STATUS hash_seq;
    MemoryAllocInfo* entry = NULL;

    if (g_instance.stat_cxt.track_memory_inited == false) {
        return;
    }

    /* remove all alloc info while memory context delete or reset */
    hash_seq_init(&hash_seq, g_instance.stat_cxt.track_memory_info_hash);
    while ((entry = (MemoryAllocInfo*)hash_seq_search(&hash_seq)) != NULL) {
        if (context == entry->context) {
            uint64 key = (uint64)entry->pointer;
            (void)hash_search(g_instance.stat_cxt.track_memory_info_hash, &key, HASH_REMOVE, NULL);
        }
    }
}









