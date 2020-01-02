/*
 * This file is part of bot2-procman.
 *
 * bot2-procman is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-procman is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-procman. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_PROCMAN_DEPUTY_PROCINFO_H_
#define BOT2_PROCMAN_DEPUTY_PROCINFO_H_

// functions for reading how much CPU and memory are being used by individual
// processes and the system as a whole

#include <stdint.h>

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int pid;

    // cpu usage time
    uint32_t user;
    uint32_t system;

    // memory usage
    int64_t  vsize;
    int64_t  rss;
    int64_t  shared;
    int64_t  text;
    int64_t  data;
} proc_cpu_mem_t;

typedef struct {
    uint32_t user;
    uint32_t user_low;
    uint32_t system;
    uint32_t idle;

    int64_t memtotal;
    int64_t memfree;
    int64_t swaptotal;
    int64_t swapfree;
} sys_cpu_mem_t;

int procinfo_read_proc_cpu_mem (int pid, proc_cpu_mem_t *s);

int procinfo_read_sys_cpu_mem (sys_cpu_mem_t *s);

/**
 * returns a GArray of ints.
 */
GArray* procinfo_get_descendants (int pid);

int procinfo_is_orphaned_child_of(int orphan, int parent);

#ifdef __cplusplus
}
#endif

#endif  /* BOT2_PROCMAN_DEPUTY_PROCINFO_H_ */
