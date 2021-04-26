//
// Created by Amogh Joshi on 4/26/21.
//

#include "writer.h"

#include <iostream>
#include <fstream>

#include <algorithm>
#include <numeric>
#include <regex>

using namespace std;
namespace fs = std::__fs::filesystem;

FileWriter::FileWriter(const std::vector<int>& mode_choice, const char *output_directory) {
    // Check whether the provided mode is valid.
    if (mode_choice.size() == 4) {
        // Copy the vector.
        int curr = 0; vector<int> indexes(4);
        generate(indexes.begin(), indexes.end(),
                 [&](){return mode_choice[curr++];});
        vector<int> expected {0, 1, 2, 3};
        if (indexes != expected) {
            const char* msg = "Received an invalid mode choice, expected "
                              "a list containing 0, 1, 2, & 3.";
            perror(msg);
        }
    }

    // Set the mode to the class.
    this->mode = mode_choice;

    // Check whether the output directory is valid.
    if (!path_exists(output_directory)) {
        const char* msg = "The output directory provided does not exist.";
        perror(msg);
    }

    // Set the output directory to the class.
    this->output_dir = output_directory;

    // Build the output directories.
    FileWriter::build_output_directory(this->output_dir);
}

FileWriter::FileWriter(const char *output_directory) {
    // Check whether the output directory is valid.
    if (!path_exists(output_directory)) {
        const char* msg = "The output directory provided does not exist.";
        perror(msg);
    }

    // Set the output directory to the class.
    this->output_dir = output_directory;

    // Build the output directories.
    FileWriter::build_output_directory(this->output_dir);
}

FileWriter::FileWriter() {
    // Set the directory path to a nullptr, which will
    // then be overwritten for each of the different
    // files which are created in the future.
    this->output_dir = nullptr;
}

std::string FileWriter::format_line(std::tuple<const char*, std::vector<int>>& content) {
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

const char* FileWriter::get_output_path(const char *image_file) {
    // Ensure that the image file exists.
    if (!fs::exists(image_file)) {
        string msg = string("The provided file \'" + string(image_file)
                            + (const char*)"\' does not exist");
        perror(msg.c_str()); exit(1);
    }

    // Get the ID of the image file (e.g., the basename of
    // the file, and without the extension).
    const fs::path basename = fs::path(image_file).stem();

    // Create a new text file with the image ID.
    const fs::path text_base = fs::path(string(basename) + ".txt");

    // Create a holder for the output directory.
    static fs::path output_directory;

    // Create the output directory as necessary.
    if (this->output_dir == nullptr) {
        // Replace the `images` directory with `annotations`.
        string str_path = fs::path(image_file).parent_path().string();
        output_directory = regex_replace(
                str_path, regex("images"), "annotations");
        FileWriter::build_output_directory(output_directory.c_str());
    } else{
        // Otherwise, it has already been built in the instantiation
        // method, so simply just create the path.
        output_directory = fs::path(this->output_dir);
    }

    // Create the complete output path.
    fs::path full_output_path (output_directory / text_base);

    // Return the complete output path.
    return fs::absolute(full_output_path).c_str();
}

void FileWriter::build_output_directory(const char* path) {
    // Check whether the output directory exists.
    if (!fs::exists(fs::path(path))) {
        // If it doesn't, then build it. If it doesn't,
        // then build get the parent directory and build that.
        if (!fs::exists(fs::path(path).parent_path())) {
            // Repeat the same thing for a third time (this method
            // will recurse through the directories thrice).
            if (!fs::exists(fs::path(path).parent_path().parent_path())) {
                if (!fs::exists(fs::path(path).parent_path().parent_path().parent_path())) {
                    const char* msg = "More than three levels of the provided directory "
                                      "structure do not exist, please provide another";
                    perror(msg); exit(1);
                } else {
                    // Make the rest of the paths.
                    fs::create_directory(fs::path(path).parent_path().parent_path());
                    fs::create_directory(fs::path(path).parent_path());
                    fs::create_directory(fs::path(path));
                }
            } else {
                // Make the rest of the paths.
                fs::create_directory(fs::path(path).parent_path());
                fs::create_directory(fs::path(path));
            }
        } else {
            // Make the rest of the paths.
            fs::create_directory(fs::path(path));
        }
    }
}

void FileWriter::build_annotation_file(const char* image_file_name,
                                       const vector<tuple<const char*, vector<int>>>& content) {
    // Get the corresponding output filename from the image.
    string output_file_path = string(this->get_output_path(image_file_name));

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
    }
    out_file.close();
}




