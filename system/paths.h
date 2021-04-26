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

#ifndef ANNOTATION_PATHS_H
#define ANNOTATION_PATHS_H

#include <string>
#include <cassert>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

/**
 * Determine if a provided path exists.
 * @param path: The provided path.
 * @return A boolean representing the state.
 */
bool path_exists(const char* path);

/**
 * Returns a list of image paths from a provided
 * directory, validation whether they are actually images
 * and then adding them to a complete list.
 * @param path: The path to get images from.
 * @param recurse: Whether to recursively search.
 * @return The list of image paths.
 */
std::vector<std::string> get_image_paths(const char* path, bool recurse);

#endif //ANNOTATION_PATHS_H
