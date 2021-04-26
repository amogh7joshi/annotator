//
// Created by Amogh Joshi on 4/25/21.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "annotation/annotation.h"

using namespace std;
namespace fs = std::__fs::filesystem;

struct UserConfig { // NOLINT
public:
    /* The path to the image directory. */
    string image_directory;

    /* The list of labels. */
    vector<string> labels;

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
     * `config.txt` file (in the top-level.
     */
    void load_config() {
        // Create the chosen configuration path.
        const char* chosen_config;

        // Check whether the path exists.
        if (!fs::exists(fs::path(this->config_path))) {
            if (!fs::exists(fs::path(this->config_path_fallback))) {
                const char *msg = "The configuration path at `config.txt` "
                                  "could not be found, check your paths.";
                perror(msg);
                exit(1);
            } else {
                chosen_config = this->config_path_fallback;
            }
        } else {
            chosen_config = this->config_path;
        }

        // Open the file.
        ifstream file;
        file.open(chosen_config);

        // Read the different lines of the file.
        std::string line; int curr = 0;
        while (std::getline(file, line)) {
            // Get the path to the image directory.
            if (curr == 0) {
                this->image_directory = line;
            }

            // Get the different labels.
            if (curr == 1) {
                // Create a vector of strings.
                string delimeter = " "; size_t pos = 0;
                vector<string> extracted_labels;

                // Iterate over the line.
                while ((pos = line.find(' ')) != string::npos) {
                    // Extract the token.
                    extracted_labels.push_back(line.substr(0, pos));
                    line.erase(0, pos + delimeter.length());
                }

                // Add the final token.
                extracted_labels.push_back(line);

                // Set the values to the class.
                this->labels = extracted_labels;
            }

            // Choose whether to recurse or not.
            if (curr == 2) {
                std::transform(line.begin(), line.end(), line.end(), ::tolower);
                istringstream is(line); bool b;
                is >> boolalpha >> b;
                this->recurse = b;
            }

            // Increment the iterator.
            curr += 1;

        }
    }
};



int main() {
    // Load the configuration.
    UserConfig config;
    config.load_config();

    // Construct the annotator with the user-provided choices.
    Annotator annotator = Annotator(
            config.image_directory.c_str(),
            config.labels, config.recurse);

    // Start the annotation session.
    annotator.start_annotation_session();
}
