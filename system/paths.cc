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

#include "paths.h"

#include <iostream>
#include <fstream>

#include <filesystem>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;
namespace fs = std::__fs::filesystem;

bool path_exists(const char* path)
{
    // If a null string has been passed, then
    // the path doesn't exist to begin with.
    if (path == nullptr)
        return false;

    // Initialize the necessary variables.
    DIR *pathDir;
    bool pathExists = false;

    // Open the path and check whether it is null or not.
    if ((pathDir = opendir(path)) != nullptr) {
        pathExists = true;
        (void)closedir(pathDir);
    }

    // Return the path state.
    return pathExists;
}

std::vector<std::string> get_image_paths(const char* path, bool recurse)
{
    // Save the provided path and initialize a vector
    // of image paths where they will be stored.
    static string initial_path = path;
    vector<string> image_paths;

    // Check whether the provided path exists.
    assert(path_exists(path));

    // Create structures to read through the directory.
    DIR *dir;
    struct dirent *ent;

    // Iterate through the files in the directory.
    if ((dir = opendir(path)) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            // Get the path name.
            const char* short_fp = ent->d_name;
            const string fp = string(path) + "/" + string(short_fp);

            // Skip any paths that are `.` or `..`.
            if (string(short_fp) == "." || string(short_fp) == "..") {
                continue;
            }

            // Determine whether the path object represents
            // a directory or a relative directory path.
            struct stat buf{};
            stat(fp.c_str(), &buf);
            if (S_ISDIR(buf.st_mode)) { // NOLINT
                if (recurse) {
                    for (const auto& r_path: get_image_paths(fp.c_str(), recurse)) {
                        image_paths.emplace_back(r_path);
                    }
                } else {
                    continue;
                }
            }

            // Check whether the path is of an image.
            for (const auto& ext : vector<string> {".jpg", ".png", ".jpeg"}) {
                if (equal(ext.rbegin(), ext.rend(), fp.rbegin())) {
                    image_paths.emplace_back(fp);
                }
            }
        }
    }

    // Return the list of image paths.
    return image_paths;
}
