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

#include "textwriter.h"

#include <iostream>
#include <fstream>

#include <algorithm>
#include <numeric>
#include <regex>

using namespace std;
namespace fs = std::__fs::filesystem;

TextFileWriter::TextFileWriter(const std::vector<int>& mode_choice,
                               const char *output_directory)
                               : FileWriter(mode_choice) {
    // Check whether the output directory is valid.
    if (!path_exists(output_directory)) {
        const char* msg = "The output directory provided does not exist.";
        error_exit(msg);
    }

    // Set the output directory to the class.
    this->output_dir = output_directory;

    // Build the output directories.
    TextFileWriter::build_output_directory(this->output_dir);

    // Set the extension mode.
    this->ext_mode = ".txt";
}

TextFileWriter::TextFileWriter(const char *output_directory)
              : FileWriter(vector<int> {0, 1, 2, 3})  {
    // Check whether the output directory is valid.
    if (!path_exists(output_directory)) {
        const char* msg = "The output directory provided does not exist.";
        error_exit(msg);
    }

    // Set the output directory to the class.
    this->output_dir = output_directory;

    // Build the output directories.
    TextFileWriter::build_output_directory(this->output_dir);

    // Set the extension mode.
    this->ext_mode = ".txt";
}

TextFileWriter::TextFileWriter(const std::vector<int>& mode_choice) : FileWriter(mode_choice)  {
    // Set the directory path to a nullptr, which will
    // then be overwritten for each of the different
    // files which are created in the future.
    this->output_dir = nullptr;

    // Set the extension mode.
    this->ext_mode = ".txt";
}

TextFileWriter::TextFileWriter() : FileWriter(vector<int> {0, 1, 2, 3}) {
    // Set the directory path to a nullptr, which will
    // then be overwritten for each of the different
    // files which are created in the future.
    this->output_dir = nullptr;

    // Set the extension mode.
    this->ext_mode = ".txt";
}

std::string TextFileWriter::format_line(std::tuple<const char*, std::vector<int>>& content) {
    // Unpack the tuple into the points and label.
    const char* label = std::get<0>(content);
    vector<int> points = std::get<1>(content);

    // Create a string and add the label to it.
    string complete_line = string(label) + " ";

    // Add each of the points in the correct order
    // (as determined by `mode`) into the line.
    for (int value: this->mode) {
        // Convert the point to a double to meet criteria.
        complete_line += to_string((double)points[value]) + " ";
    }

    // Add a newline to the file.
    complete_line += "\n";

    // Return the complete line.
    return complete_line;
}

void TextFileWriter::build_annotation_file(const char* image_file_name,
                                           const vector<tuple<const char*, vector<int>>>& content) {
    // Get the corresponding output filename from the image.
    const string output_file_path = this->get_output_path(image_file_name);

    // Create a vector to hold the file lines.
    vector<string> file_lines;

    // Iterate over the bounding boxes in the vector, extract
    // the content, and add the converted lines into the vector.
    file_lines.reserve(content.size());
    for (auto content_piece: content) {
        file_lines.emplace_back(this->format_line(content_piece));
    }

    // Open the file and write the lines into it.
    ofstream out_file;
    out_file.open(output_file_path);
    for (const auto& line: file_lines) {
        out_file << line;
        out_file.flush();
    }
    out_file.close();
}




