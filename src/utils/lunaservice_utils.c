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

#include "lunaservice_utils.h"

void
LSMessageReplyErrorUnknown(LSHandle *sh, LSMessage *message)
{
    LSError lserror;
    LSErrorInit(&lserror);

    bool retVal = LSMessageReply(sh, message, "{\"returnValue\":false, \"errorCode\":-1,"
                                 "\"errorText\":\"Unknown Error.\"}", &lserror);

    if (!retVal)
    {
        LSErrorPrint(&lserror, stderr);
        LSErrorFree(&lserror);
    }
}

void
LSMessageReplyErrorInvalidParams(LSHandle *sh, LSMessage *message)
{
    LSError lserror;
    LSErrorInit(&lserror);

    bool retVal = LSMessageReply(sh, message, "{\"returnValue\":false, \"errorCode\":-1,"
                                 "\"errorText\":\"Invalid parameters.\"}", NULL);

    if (!retVal)
    {
        LSErrorPrint(&lserror, stderr);
        LSErrorFree(&lserror);
    }
}

void
LSMessageReplyErrorBadJSON(LSHandle *sh, LSMessage *message)
{
    LSError lserror;
    LSErrorInit(&lserror);

    bool retVal = LSMessageReply(sh, message, "{\"returnValue\":false, \"errorCode\":-1,"
                                 "\"errorText\":\"Malformed json.\"}", NULL);

    if (!retVal)
    {
        LSErrorPrint(&lserror, stderr);
        LSErrorFree(&lserror);
    }
}

void
LSMessageReplySuccess(LSHandle *sh, LSMessage *message)
{
    LSError lserror;
    LSErrorInit(&lserror);

    bool retVal = LSMessageReply(sh, message, "{\"returnValue\":true}",
                                 NULL);

    if (!retVal)
    {
        LSErrorPrint(&lserror, stderr);
        LSErrorFree(&lserror);
    }
}

void
LSMessageReplyCustomError(LSHandle *sh, LSMessage *message, const char *errormsg)
{
    LSError lserror;
    LSErrorInit(&lserror);
    char *errorString;

    errorString = g_strdup_printf("{\"returnValue\":false,\"errorCode\":-1,\"errorText\":\"%s\"}", errormsg);

    bool retVal = LSMessageReply(sh, message, errorString, NULL);
    if (!retVal)
    {
        LSErrorPrint(&lserror, stderr);
        LSErrorFree(&lserror);
    }

    g_free(errorString);
}


