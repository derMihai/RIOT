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
 * @brief   Thread join test application
 *
 * @author  Mihai Renea <mihai.renea@ml-pa.com>
 *
 * @}
 */

#include <errno.h>
#include <stdio.h>

#include "test_utils/expect.h"
#include "thread.h"
#include "ztimer.h"

#ifdef CPU_JOIN_WITH_VALUE
void *retval;
#define RETVAL_STORAGE() (retval = 0, &retval)
#define EXPECT_RETVAL(expected) expect((unsigned)(uintptr_t)(retval) == (expected))
#else
#define RETVAL_STORAGE() (NULL)
#define EXPECT_RETVAL(expected) ((void)(expected))
#endif

static void *joinee_f(void *arg)
{
    ztimer_sleep(ZTIMER_MSEC, 100);

    return arg;
}

static kernel_pid_t _spawn_joinee(bool joinable, unsigned expect_retval)
{
    static char joinee_stack[THREAD_STACKSIZE_MAIN];
    return thread_create(joinee_stack, sizeof(joinee_stack),
                         THREAD_PRIORITY_MAIN,
                         joinable ? THREAD_CREATE_JOINABLE : 0,
                         joinee_f,
                         (void *)(uintptr_t)expect_retval,
                         "joinee");
}

int main(void)
{
    puts("START");

    kernel_pid_t joinee_pid = _spawn_joinee(true, 1);
    expect(joinee_pid >= 0);

    int res = thread_join(joinee_pid, RETVAL_STORAGE());
    expect(res == 0);
    expect(thread_get(joinee_pid) == NULL);
    EXPECT_RETVAL(1);

    joinee_pid = _spawn_joinee(true, 2);
    expect(joinee_pid >= 0);

    ztimer_sleep(ZTIMER_MSEC, 150);

    thread_t *joinee = thread_get(joinee_pid);
    expect(joinee && joinee->status == STATUS_ZOMBIE);

    res = thread_join(joinee_pid, RETVAL_STORAGE());
    expect(res == 0);
    expect(thread_get(joinee_pid) == NULL);
    EXPECT_RETVAL(2);

    joinee_pid = _spawn_joinee(false, 3);

    res = thread_join(joinee_pid, RETVAL_STORAGE());
    expect(res == -EINVAL);
    EXPECT_RETVAL(0);

    puts("SUCCESS");

    return 0;
}
