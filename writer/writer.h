//
// Created by Amogh Joshi on 5/27/21.
//

#include <string>
#include <vector>
#include <filesystem>

#ifndef ANNOTATOR_WRITER_H
#define ANNOTATOR_WRITER_H

/**
 * Base class for the different writers.
 */
class FileWriter {
private:
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

protected:
    /* The output file base path that corresponds to the
     * specific writer type. */
    const char* ext_mode;

    /**
     * Gets the name of the output filepath from
     * the corresponding input image file.
     * @param image_file: The input image file.
     */
    std::string get_output_path(const char* image_file);

    /**
     * Builds the output directories as necessary.
     */
    static void build_output_directory(const char* path);

    /**
     * Constructs the annotation file given an input
     * set of annotation bounding boxes. This is the main
     * exposed method that should be used.
     * @param image_file_name: The filename of the
     * image that the annotations are being made for.
     * @param content: The file content.
     */
    virtual void build_annotation_file(const char* image_file_name,
                                       const std::vector<std::tuple<const char*,
                                       std::vector<int>>>& content);

};

#endif //ANNOTATOR_WRITER_H
