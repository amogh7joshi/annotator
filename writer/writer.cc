//
// Created by Amogh Joshi on 5/27/21.
//

#include "writer.h"

#include <iostream>
#include <regex>

using namespace std;
namespace fs = std::__fs::filesystem;

std::string FileWriter::get_output_path(const char *image_file) {
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
    const fs::path text_base = fs::path(string(basename) + this->ext_mode);

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

void FileWriter::build_annotation_file(
        const char* image_file_name, const vector<tuple<const char*, vector<int>>>& content) {
    // The virtual method shouldn't be used.
    const char* msg = "The base annotation writer class should not be used.";
    perror(msg); exit(1);
}

