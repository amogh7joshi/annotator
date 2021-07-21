/* Copyright 2021 Amogh Joshi. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "error.h"

#include <iostream>
#include <string>

using namespace std;

void error_exit(const char* msg) {
    // Check whether a period has been added to the input
    // message, and remove it (essentially simple formatting).
    const char* end = strrchr(msg, '.');
    if (end && !strcmp(end, ".")) {
        string c_msg(msg);
        c_msg = c_msg.substr(0, c_msg.size() - 1);
        msg = c_msg.c_str();
    }

    // Print the message to standard error and exit.
    perror(msg); exit(1);
}

