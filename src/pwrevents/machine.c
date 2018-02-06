// Copyright (c) 2011-2018 LG Electronics, Inc.
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


#include <unistd.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <stdbool.h>
#include <syslog.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <json.h>
#include <luna-service2/lunaservice.h>

#include "main.h"
#include "suspend.h"
#include "defines.h"

#include "machine.h"
#include "sleepd_debug.h"
#include "logging.h"
#include "suspend.h"
#include "config.h"

/**
 * Holds the current state of whether or not we're being supplied with power from a charger of any sort.
 */
bool chargerIsConnected = false;

bool
MachineCanSleep(void)
{
    return (!chargerIsConnected || gSleepConfig.suspend_with_charger);
}

const char *
MachineCantSleepReason(void)
{
    static char reason[512];

    snprintf(reason, 512, "%s", chargerIsConnected ? "charger_present" : "");

    return reason;
}


void MachineSleep(void)
{
    bool success;

    nyx_system_suspend(GetNyxSystemDevice(), &success);
}

void
MachineForceShutdown(const char *reason)
{
    SLEEPDLOG_INFO(MSGID_FRC_SHUTDOWN, 1, PMLOGKS("Reason", reason),
                   "Pwrevents shutting down system");

    if (gSleepConfig.fasthalt)
    {
        nyx_system_shutdown(GetNyxSystemDevice(), NYX_SYSTEM_EMERG_SHUTDOWN, reason);
    }
    else
    {
        nyx_system_shutdown(GetNyxSystemDevice(), NYX_SYSTEM_NORMAL_SHUTDOWN, reason);
    }
}

void
MachineForceReboot(const char *reason)
{
    SLEEPDLOG_INFO(MSGID_FRC_REBOOT, 1, PMLOGKS("Reason", reason),
                   "Pwrevents rebooting system");

    if (gSleepConfig.fasthalt)
    {
        nyx_system_reboot(GetNyxSystemDevice(), NYX_SYSTEM_EMERG_SHUTDOWN, reason);
    }
    else
    {
        nyx_system_reboot(GetNyxSystemDevice(), NYX_SYSTEM_NORMAL_SHUTDOWN, reason);
    }
}

bool ChargerStatus(LSHandle *sh,
                   LSMessage *message, void *user_data)
{
    struct json_object *object;
    object = json_tokener_parse(LSMessageGetPayload(message));


    if (object)
    {
        if (json_object_object_get(object, "connected"))
        {
            chargerIsConnected = json_object_get_boolean(json_object_object_get(object,
                                 "connected"));
        }
    }

    if (object)
    {
        json_object_put(object);
    }

    return true;
}
