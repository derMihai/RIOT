/*
 * Copyright (C) 2019 Gunar Schorcht
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * FreeRTOS to RIOT-OS adaption module for source code compatibility
 */

#ifndef DOXYGEN

#define ENABLE_DEBUG 0
#include "debug.h"

#include "esp_attr.h"
#include "irq.h"
#include "rom/ets_sys.h"

#include "freertos/FreeRTOS.h"

unsigned _xt_tick_divisor = 0;  /* cached number of cycles per tick */

extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);

/* source: /path/to/esp8266-rtos-sdk/components/freertos/port/esp8266/port.c */
void IRAM_ATTR vPortETSIntrLock(void)
{
    ETS_INTR_LOCK();
}

/* source: /path/to/esp8266-rtos-sdk/components/freertos/port/esp8266/port.c */
void IRAM_ATTR vPortETSIntrUnlock(void)
{
    ETS_INTR_UNLOCK();
}

/* source: /path/to/esp8266-rtos-sdk/components/freertos/port/esp8266/port.c */
void ResetCcountVal(unsigned int cnt_val)
{
    __asm__ volatile("wsr a2, ccount");
}

#endif /* DOXYGEN */
