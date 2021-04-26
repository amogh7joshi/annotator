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

#include "annotation.h"

#include <filesystem>

using namespace std;
namespace fs = std::__fs::filesystem;

Annotator::Annotator(const char *img_dir, std::vector<std::string> label_list, bool recurse)
        : handler("two-click", label_list), writer() {
    // Check whether the provided image directory exists.
    if (!fs::exists(fs::path(img_dir))) {
        const char* msg = "The provided image directory does not exist";
        perror(msg); exit(1);
    }

    // Load the list of image paths from the directory.
    this->recursive_search = recurse;
    this->image_paths = get_image_paths(img_dir, recursive_search);
}

Annotator::Annotator(const char *img_dir, std::vector<std::string> label_list)
        : handler("two-click", label_list), writer() {
    // Check whether the provided image directory exists.
    if (!fs::exists(fs::path(img_dir))) {
        const char* msg = "The provided image directory does not exist.";
        perror(msg); exit(1);
    }

    // Load the list of image paths from the directory.
    this->image_paths = get_image_paths(img_dir, recursive_search);
}

void Annotator::start_annotation_session() {
    // Iterate over each of the images in the list of paths.
    for (auto path: image_paths) {
        // Conduct the bounding box annotation session.
        int res = this->handler.annotate(path.c_str());
        if (res == -1) {
            // An issue was encountered.
            const char* msg = "Encountered an error while annotating";
            perror(msg); exit(1);
        }
        // Extract the bounding boxes and pass them to the writer.
        this->writer.build_annotation_file(path.c_str(), this->handler.get_bounding_boxes());
    }
}
