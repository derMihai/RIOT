/*
 * Copyright (C) 2014, 2015 Freie Universitaet Berlin (FUB) and INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @ingroup cpu_native
 * @brief   Crash handling functions implementation for 'native' port
 * @author  Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 * @author  Kévin Roussel <Kevin.Roussel@inria.fr>
 * @author  Oliver Hahm <oliver.hahm@inria.fr>
 */

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "native_internal.h"

void panic_arch(void)
{
    extern unsigned _native_retval;
    _native_retval = EINVAL;

#ifdef DEVELHELP
    /* since we're atop an Unix-like platform,
       just use the (developer-)friendly core-dump feature */
    kill(_native_pid, SIGTRAP);
#endif
}
