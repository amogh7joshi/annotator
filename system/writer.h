//
// Created by Amogh Joshi on 4/26/21.
//

#ifndef ANNOTATION_WRITER_H
#define ANNOTATION_WRITER_H

#include <vector>
#include <filesystem>

#include "writer.h"
#include "paths.h"

/**
 * Writes a set of annotations into
 * an annotation file, with each line
 * corresponding to a specific annotation
 * and containing the class and then
 * values based on the chosen mode.
 */
class FileWriter {
protected:
    /* The specific mode being used, which corresponds
     * to the arrangement of the different coordinate
     * values in the text file.
     *
     * The default arrangement is x_min, y_min, x_max,
     * y_max, corresponding to the default {0, 1, 2, 3},
     * but this can be overridden in the constructor with
     * a different arrangement of integers.
     * */
    std::vector<int> mode = {0, 1, 2, 3};

    /* The directory where the annotation files will be
     * written to. By default, the actual file names will
     * be a copy of the actual image file names. */
    const char* output_dir;

public:
    /**
     * Initializes the FileWriter class
     * with a specific mode.
     * @param mode: The mode of choice.
     * @param output_directory: The directory to
     * write the output annotation files to.
     */
    FileWriter(const std::vector<int>& mode_choice,
               const char* output_directory_choice);

    /**
     * Initializes the FileWriter class
     * with the default mode.
     * @param output_directory: The directory to
     * write the output annotation files to.
     */
    explicit FileWriter(const char* output_directory_choice);

    /**
     * Initializes the FileWriter class with the
     * default mode and directory. In this case, the
     * default directory will end up being the same
     * as each input image, with the directory `images`
     * replaced with `annotations` instead.
     */
    FileWriter();

    /**
     * Constructs the annotation file given an input
     * set of annotation bounding boxes. This is the main
     * exposed method that should be used.
     * @param image_file_name: The filename of the
     * image that the annotations are being made for.
     * @param content: The file content.
     */
    void build_annotation_file(const char* image_file_name,
                               const std::vector<std::tuple<const char*, std::vector<int>>>& content);

private:
    /**
     * Formats each line of the file into the
     * correct format, to be used in the actual
     * file writing method.
     * @param content: The line of content.
     */
    std::string format_line(std::tuple<const char*,
            std::vector<int>>& content);

    /**
     * Gets the name of the output filepath from
     * the corresponding input image file.
     * @param image_file: The input image file.
     */
    const char* get_output_path(const char* image_file);

    /**
     * Builds the output directories as necessary.
     */
    static void build_output_directory(const char* path);


};

#endif //ANNOTATION_WRITER_H
