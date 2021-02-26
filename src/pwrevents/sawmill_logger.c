// Copyright (c) 2011-2021 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

/**
 * @file sawmill_logger.c
 *
 * @brief This module's purpose is to log periodic statistics
 * to /var/log/messages for processing by sawmill
 *
 */

#include <glib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <luna-service2/lunaservice.h>

#include "init.h"
#include "logging.h"

#define PRINT_INTERVAL_MS 60000

static long unsigned int sTimeOnWake = 0;
static long unsigned int sTimeOnPrint = 0;
static long unsigned int sTimeScreenOn = 0;
static long unsigned int sTimeScreenOff = 0;
static int sMSUntilPrint = 0;
static bool sScreenIsOn = false;
//static timespec sTimeOnSuspended = 0;

static long unsigned int sTotalMSAwake = 0;
static long unsigned int sTotalMSAsleep = 0;
static long unsigned int sTotalMSScreenOn = 0;
static long unsigned int sTotalMSScreenOff = 0;
static bool sIsAwake = true;
static guint sTimerEventSource = 0;

#define NS_PER_MS 1000000
#define MS_PER_S 1000
#define UPPER_CLAMP(x, high) (((x) > (high)) ? (high) : (x))

long int
time_to_ms(struct timespec t)
{
    return (t.tv_sec * MS_PER_S) + (t.tv_nsec / NS_PER_MS);
}

/**
 *
 * @return  A relative timestamp that can be used to compute
 *          elapsed times.
 */
void
get_time_now(struct timespec *time_now)
{
    clock_gettime(CLOCK_REALTIME, time_now);
}

/**
 * @brief Return a monotonic time, in ms.
 *
 * @return  A relative timestamp that can be used to compute
 *          elapsed times.
 */
long int
time_now_ms()
{
    struct timespec time_now;
    get_time_now(&time_now);
    return time_to_ms(time_now);
}

void
sawmill_logger_record_sleep(struct timespec time_awake)
{
    sTotalMSAwake += time_to_ms(time_awake);
    sIsAwake = false;

    // calculate the amnt of time left to fire
    sMSUntilPrint = UPPER_CLAMP(PRINT_INTERVAL_MS - (time_now_ms() - sTimeOnPrint),
                          PRINT_INTERVAL_MS);
}

gboolean
sawmill_logger_update(gpointer data)
{
    if (sIsAwake)
    {
        sTimeOnPrint = time_now_ms();
        long unsigned int diff_awake = sTimeOnPrint - sTimeOnWake;

        SLEEPDLOG_DEBUG("Logger Update : time_awake_ms: %lu time_asleep_ms: %lu time_screen_on_ms: %lu time_screen_off_ms: %lu",
                        sTotalMSAwake + diff_awake,
                        sTotalMSAsleep,
                        sTotalMSScreenOn + (sScreenIsOn ? (time_now_ms() - sTimeScreenOn) : 0),
                        sTotalMSScreenOff + (sScreenIsOn ? 0 : (time_now_ms() - sTimeScreenOff))
                       );
    }

    //TODO: use g_timer_source_set_interval(GTimerSource *tsource, guint interval_ms, gboolean from_poll)
    g_source_remove(sTimerEventSource);
    sTimerEventSource = g_timeout_add_full(G_PRIORITY_DEFAULT, PRINT_INTERVAL_MS,
                                           sawmill_logger_update, GINT_TO_POINTER(TRUE), NULL);
    return FALSE;
}

// note we dont get ms resolution here
void
sawmill_logger_record_wake(struct timespec time_asleep)
{
    unsigned long int ms_asleep = time_to_ms(time_asleep);
    sTotalMSAsleep += ms_asleep;
    sTimeOnWake = time_now_ms();
    sIsAwake = true;

    //TODO: use g_timer_source_set_interval(GTimerSource *tsource, guint interval_ms, gboolean from_poll)
    g_source_remove(sTimerEventSource);
    sTimerEventSource = g_timeout_add_full(G_PRIORITY_DEFAULT,
                                           UPPER_CLAMP(sMSUntilPrint - ms_asleep, PRINT_INTERVAL_MS), sawmill_logger_update,
                                           GINT_TO_POINTER(FALSE), NULL);
}

void
sawmill_logger_record_screen_toggle(bool set_on)
{

    // ignoring duplicate calls
    if (set_on == sScreenIsOn)
    {
        return;
    }

    if (set_on)
    {
        sTimeScreenOn = time_now_ms();
        sTotalMSScreenOff += (sTimeScreenOn - sTimeScreenOff);
    }
    else
    {
        sTimeScreenOff = time_now_ms();
        sTotalMSScreenOn += (sTimeScreenOff - sTimeScreenOn);
    }

    sScreenIsOn = set_on;
}


static int
_sawlog_init(void)
{
    sTimeOnWake = time_now_ms();
    sTimeOnPrint = time_now_ms();
    sTimeScreenOn = time_now_ms();
    sTimeScreenOff = time_now_ms();
    sTimerEventSource = g_timeout_add_full(G_PRIORITY_DEFAULT, PRINT_INTERVAL_MS,
                                           sawmill_logger_update, GINT_TO_POINTER(TRUE), NULL);

    return 0;
}

INIT_FUNC(INIT_FUNC_MIDDLE, _sawlog_init);
