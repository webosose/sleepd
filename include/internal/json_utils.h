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

#ifndef __JSON_UTILS_H__
#define __JSON_UTILS_H__

#include <json.h>
#include <stdbool.h>

bool get_json_string(struct json_object *object, const char *key, char **value);
bool get_json_int(struct json_object *object, const char *key, int *value);
bool get_json_boolean(struct json_object *object, const char *key, bool *value);
bool get_json_object_as_string(struct json_object *object, const char *key, char **value);

#endif
