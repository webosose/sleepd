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

#include <string.h> // For definition of NULL
#include "json_utils.h"

static struct json_object *get_typed_object(struct json_object *object,
        const char *key, json_type desired_type)
{
    struct json_object *obj = NULL;
    bool result = (json_object_object_get_ex(object, key, &obj)) &&
                  (json_object_get_type(obj) == desired_type);
    return result ? obj : NULL;
}

bool get_json_string(struct json_object *object, const char *key,
                     const char **value)
{
    bool result = false;
    struct json_object *strObj = get_typed_object(object, key, json_type_string);

    if (strObj)
    {
        *value = json_object_get_string(strObj);
        result = *value && strlen(*value) != 0;
    }

    return result;
}

bool get_json_int(struct json_object *object, const char *key, int *value)
{
    bool result = false;
    struct json_object *intObj = get_typed_object(object, key, json_type_int);

    if (intObj)
    {
        *value = json_object_get_int(intObj);
        result = true;
    }

    return result;
}

bool get_json_boolean(struct json_object *object, const char *key, bool *value)
{
    bool result = false;
    struct json_object *boolObj = get_typed_object(object, key, json_type_boolean);

    if (boolObj)
    {
        *value = json_object_get_boolean(boolObj);
        result = true;
    }

    return result;
}

bool get_json_object_as_string(struct json_object *object, const char *key,
                               const char **value)
{
    struct json_object *strObj = get_typed_object(object, key, json_type_object);
    bool result = false;

    if (strObj)
    {
        *value = json_object_get_string(strObj);
        result = *value && strlen(*value) != 0;
    }

    return result;
}
