//
// Created by Amogh Joshi on 4/26/21.
//

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
