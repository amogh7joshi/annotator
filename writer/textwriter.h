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

#ifndef ANNOTATION_WRITER_H
#define ANNOTATION_WRITER_H

#include <vector>
#include <filesystem>

#include "writer.h"
#include "textwriter.h"
#include "../system/paths.h"

/**
 * Writes a set of annotations into an text annotation
 * file, with each line corresponding to a specific
 * annotation and containing the class and then
 * values based on the chosen mode.
 */
class TextFileWriter : public FileWriter {
public:
    /**
     * Initializes the FileWriter class
     * with a specific mode.
     * @param mode: The mode of choice.
     * @param output_directory: The directory to
     * write the output annotation files to.
     */
    TextFileWriter(const std::vector<int>& mode_choice,
                   const char* output_directory_choice);

    /**
     * Initializes the FileWriter class
     * with the default mode.
     * @param output_directory: The directory to
     * write the output annotation files to.
     */
    explicit TextFileWriter(const char* output_directory_choice);

    /**
     * Initializes the FileWriter class with the
     * default mode and directory. In this case, the
     * default directory will end up being the same
     * as each input image, with the directory `images`
     * replaced with `annotations` instead.
     */
    TextFileWriter();

    /**
     * Initializes the FileWriter class with the
     * default directory and chosen mode.
     * @param mode_choice: The chosen writing mode.
     */
    TextFileWriter(const std::vector<int>& mode_choice);

    /**
     * Constructs the annotation file given an input
     * set of annotation bounding boxes. This is the main
     * exposed method that should be used.
     * @param image_file_name: The filename of the
     * image that the annotations are being made for.
     * @param content: The file content.
     */
    void build_annotation_file(const char* image_file_name,
                               const std::vector<std::tuple<const char*,
                                       std::vector<int>>>& content);

private:
    /**
     * Formats each line of the file into the
     * correct format, to be used in the actual
     * file writing method.
     * @param content: The line of content.
     */
    std::string format_line(std::tuple<const char*,
                            std::vector<int>>& content);

};

#endif //ANNOTATION_WRITER_H
