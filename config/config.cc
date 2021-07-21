//
// Created by Amogh Joshi on 5/25/21.
//

#include "config.h"

using namespace std;
namespace fs = std::__fs::filesystem;

void UserConfig::load_config() {
    // Create the chosen configuration path.
    const char* chosen_config;

    // Check whether the path exists.
    if (!fs::exists(fs::path(this->config_path))) {
        if (!fs::exists(fs::path(this->config_path_fallback))) {
            const char *msg = "The configuration path at `config.txt` "
                              "could not be found, check your paths.";
            error_exit(msg);
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

        // Determine the writing mode order.
        if (curr == 3) {
            // Create a vector of strings.
            string delimeter = " "; size_t pos = 0;
            vector<int> extracted_labels;

            // Iterate over the line.
            while ((pos = line.find(' ')) != string::npos) {
                // Extract the token.
                extracted_labels.push_back(stoi(line.substr(0, pos)));
                line.erase(0, pos + delimeter.length());
            }

            // Add the final token.
            extracted_labels.push_back(stoi(line));

            // Set the values to the class.
            this->mode_order = extracted_labels;
        }

        // Increment the iterator.
        curr += 1;

    }
}

