// Copyright (c) 2015-2018 LG Electronics, Inc.
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

#include <string.h>
#include "json_utils.h"

bool get_json_string(struct json_object *object, const char *key, char **value)
{
    struct json_object *strObj = NULL;
    strObj = json_object_object_get(object, key);
    if(!strObj || json_object_get_type(strObj) != json_type_string)
        return false;

    *value = json_object_get_string(strObj);
    if(*value == NULL || !strlen(*value))
        return false;
    return true;
}

bool get_json_int(struct json_object *object, const char *key, int *value)
{
    struct json_object *intObj = NULL;
    intObj = json_object_object_get(object, key);
    if(!intObj || json_object_get_type(intObj) != json_type_int)
        return false;

    *value = json_object_get_int(intObj);
    return true;
}

bool get_json_boolean(struct json_object *object, const char *key, bool *value)
{
    struct json_object *boolObj = NULL;
    boolObj = json_object_object_get(object, key);
    if(!boolObj || json_object_get_type(boolObj) != json_type_boolean)
        return false;

    *value = json_object_get_boolean(boolObj);
    return true;
}

bool get_json_object_as_string(struct json_object *object, const char *key, char **value)
{
    struct json_object *strObj = NULL;
    strObj = json_object_object_get(object, key);
    if(!strObj || json_object_get_type(strObj) != json_type_object)
        return false;

    *value = json_object_get_string(strObj);
    if(*value == NULL || !strlen(*value))
        return false;
    return true;
}
