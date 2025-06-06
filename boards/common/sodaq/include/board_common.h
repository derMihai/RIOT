/*
 * Copyright (C) 2016 Kees Bakker, SODAQ
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @defgroup    boards_common_sodaq SODAQ boards common
 * @ingroup     boards_common
 * @brief       Common support for all SODAQ boards
 * @{
 *
 * @file
 * @brief       Board common definitions for the SODAQ boards
 *
 * @author      Kees Bakker <kees@sodaq.com>
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    xtimer configuration
 * @{
 */
#define XTIMER_DEV          TIMER_DEV(1)
#define XTIMER_CHAN         (0)
/** @} */

#ifdef __cplusplus
}
#endif

/** @} */
