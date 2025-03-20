/*
 * Copyright (C) 2025 Mihai Renea <mihai.renea@ml-pa.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief   thread flags group test application
 *
 * @author  Mihai Renea <mihai.renea@ml-pa.com>
 *
 * @}
 */

#include <stdio.h>

#include "atomic_utils.h"
#include "test_utils/expect.h"
#include "thread.h"
#include "thread_flags_group.h"

#ifndef WAITER_THREADS_CNT
#  define WAITER_THREADS_CNT 3
#endif

static char stacks[WAITER_THREADS_CNT][THREAD_STACKSIZE_MAIN];

#define GOOD_FLAG 0x2
#define BAD_FLAG  0x4

static uint8_t woken_up = 0;
static uint8_t last_prio = 0;
static tfg_t group = THREAD_FLAGS_GROUP_INIT;

static void *forever_waiter(void *arg)
{
    printf("non-waiter %u waiting...\n", (unsigned)(uintptr_t)arg);
    thread_flags_wait_any(GOOD_FLAG | BAD_FLAG);
    expect(false);
}

static void *waiter(void *arg)
{
    thread_flags_group_join(&group);

    printf("waiter %u waiting...\n", (unsigned)(uintptr_t)arg);
    thread_flags_wait_any(GOOD_FLAG);

    printf("waiter %u woken up!\n", (unsigned)(uintptr_t)arg);

    expect(atomic_load_u8(&last_prio) <= thread_get_active()->priority);

    atomic_store_u8(&last_prio, thread_get_active()->priority);
    atomic_fetch_add_u8(&woken_up, 1);

    return NULL;
}

int main(void)
{
   	puts("START");
    unsigned waiters_cnt = 0;
    for (unsigned i = 0; i < WAITER_THREADS_CNT; i++) {
        int prio = (int)THREAD_PRIORITY_MAIN - i - 1;
        if (prio < 0) {
            prio = 0;
        }

        thread_task_func_t handler = i % 3 ? (waiters_cnt++, waiter) : forever_waiter;
        int res = thread_create(stacks[i], THREAD_STACKSIZE_MAIN,
                                prio, THREAD_CREATE_STACKTEST, handler,
                                (void *)(uintptr_t)i, "waiter");
        expect(res >= 0);
    }

    /* this shouldn't wake up */
    thread_flags_group_set(&group, BAD_FLAG);
    expect(atomic_load_u8(&woken_up) == 0);

    puts("waking up!");

    thread_flags_group_set(&group, GOOD_FLAG);

    /* waiters have higher prio, so they must have finished */
    printf("%u, %u\n", atomic_load_u8(&woken_up), waiters_cnt);
    expect(atomic_load_u8(&woken_up) == waiters_cnt);

    puts("SUCCESS");

    return 0;
}
