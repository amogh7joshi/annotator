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

#ifndef ANNOTATION_ANNOTATOR_H
#define ANNOTATION_ANNOTATOR_H

#include <string>
#include <vector>

#include "../system/paths.h"
#include "../system/writer.h"
#include "../handler/handler.h"

/**
 * The primary class that conducts the annotation
 * sessions and writes the annotation files.
 *
 * In essence, this is a wrapper class for both the
 * `AnnotationHandler` and `FileWriter` classes,
 * and combines their functionalities together
 * to create one seamless handler for both stages.
 */
class Annotator {
private:
    /* The path to the directory full of images. */
    const char* image_directory{};

    /* The list of labels to annotate for. */
    std::vector<const char*> labels;

protected:
    /* Whether to recurse through the provided image
     * directory to find all images in sub-directories,
     * or whether to just search the top-level. */
    bool recursive_search = true;

    /* The list of image paths which will be annotated. */
    std::vector<std::string> image_paths;

    /* The AnnotationHandler for the class. */
    AnnotationHandler handler;

    /* The FileWriter for the class. */
    FileWriter writer;

public:
    /**
     * Instantiates the Annotator class with
     * an image directory and list of labels.
     * @param img_dir: The directory of images.
     * @param label_list: The list of labels.
     * @param recurse_search: Whether to search
     * recursively through the image directory.
     */
    Annotator(const char* img_dir, const std::vector<std::string>& label_list, bool recurse_search);

    /**
     * Instantiates the Annotator class with
     * an image directory and list of labels.
     * @param img_dir: The directory of images.
     * @param label_list: The list of labels.
     */
    Annotator(const char* img_dir, const std::vector<std::string>& label_list);

    /**
     * Conducts the actual annotation session, e.g.
     * displaying each image file, drawing bounding
     * boxes on them, and then saving the annotated
     * images to a text file in the save location.
     */
    void start_annotation_session();

};

#endif //ANNOTATION_ANNOTATOR_H
