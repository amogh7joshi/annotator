//
// Created by Amogh Joshi on 5/25/21.
//

#ifndef ANNOTATOR_CONFIG_H
#define ANNOTATOR_CONFIG_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>

struct UserConfig { // NOLINT
public:
    /* The path to the image directory. */
    std::string image_directory;

    /* The list of labels. */
    std::vector<std::string> labels;

    /* Whether to recurse or not. */
    bool recurse;

private:
    /* The path to the configuration file. */
    const char* config_path = "./config.txt";
    /* A fallback in case the compiled annotation
     * file ends up in a different directory (e.g.,
     * if the user is using something like CLion). */
    const char* config_path_fallback = "../config.txt";

public:
    /**
     * Loads in a user configuration from the
     * `config.txt` file (in the top-level).
     */
    void load_config();
};

#endif //ANNOTATOR_CONFIG_H
