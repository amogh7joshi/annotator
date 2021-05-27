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

#include "handler.h"

#include <random>
#include <filesystem>

#define WINDOW_NAME "Annotation"

using namespace std;
using namespace cv;
namespace fs = std::__fs::filesystem;

// The list of colors needs to be initialized here.
std::vector<cv::Scalar> AnnotationHandler::colors = { // NOLINT
        Scalar(255, 225, 186), Scalar(255, 179, 186), Scalar(255, 255, 186),
        Scalar(186, 255, 201), Scalar(186, 225, 255)
};

AnnotationHandler::AnnotationHandler(const char *mode_choice,
                                     const std::vector<std::string>& class_list)
{
    // Validate and initialize the chosen mode.
    static std::set<const char*> mode_choices {"debug", "two-click", "drag"};
    if (mode_choices.find(mode_choice) != mode_choices.end()) {
        mode = mode_choice;
    } else {
        string msg = "Invalid mode \'" + string(mode_choice) + "\' received.";
        perror(msg.c_str()); exit(1);
    }

    // Create the relevant OpenCV methods, e.g. the named window
    // and then this class instance as the event handler.
    namedWindow(WINDOW_NAME);
    setMouseCallback(WINDOW_NAME, AnnotationHandler::dispatch_handler, (void*)(this));

    // Initialize the list of labels and the trackbar which
    // will be used to control the actual label choice.
    if (class_list.size() > 5) {
        const char* msg = "The list of class labels should not be longer than 5.";
        perror(msg); exit(1);
    }
    this->label_list = class_list;

    // Set the current label to be the first one in the list.
    this->current_label = this->label_list[0].c_str();
}

int AnnotationHandler::annotate(const char *image_path) {
    // Check whether the path exists or not.
    if (!fs::exists(image_path)) {
        string msg = "The provided image path \'" +
                     string(image_path) + "\' does not exist";
        perror(msg.c_str()); exit(1);
    } else {
        // Read the image and update the class state.
        Mat img = imread(image_path);
        this->update_states(img);
    }

    // Update the button animations for the first button.
    if (this->clicked_index == -1) {
        this->update_button_animations(0);
    } else {
        this->update_button_animations(this->clicked_index);
    }

    // Set a boolean to exit.
    bool exit = false;
    bool complete = false;

    // Iterate over the image and conduct an annotation session.
    while (true) {
        // Display the image.
        imshow(WINDOW_NAME, this->image);

        // Capture the "WaitKey" value.
        int k = waitKey(1);

        // Iterate over the different cases for `k`.
        switch(k) {
            case (int) ('q'): // Exit the loop.
                exit = true;
                break;
            case (int) ('c'): {// Restart the session for the image.
                // We can't just use the regular `update_button_animations`
                // with the regular class values here, so we first update
                // the class states, then access the current button and
                // use that to re-initialize all of them.
                this->update_states(this->image_cache);
                int current_button_index = this->clicked_index;
                this->clicked_index = -1;
                this->update_button_animations(current_button_index);
                break;
            }
            case (int) ('\r'): // The annotation is complete.
            case (int) ('\n'):
            case (int) (' '):
            case (int) ('n'):
                complete = true;
                break;
            default: // Continue throughout the session.
                break;
        }

        // If we need to exit, then exit.
        if (exit) {
            return -1;
        } else if (complete) {
            return 0;
        } else {
            continue;
        }
    }
}

void AnnotationHandler::update_states(cv::Mat& new_image) {
    // Reset the position and drawing trackers.
    this->ix = -1; this->iy = -1;
    this->is_drawing = false;

    // Set the new image.
    this->image = new_image;

    // Save a copy of the image for resetting.
    this->image_cache = this->image.clone();

    // Clear the list of buttons.
    this->buttons.clear();

    // Clear the list of bounding boxes.
    this->bounding_boxes.clear();

    // Get the image shape, its values, and then create a set
    // of buttons which correspond to the different class choices.
    this->add_buttons_to_image();
}

void AnnotationHandler::update_bounding_boxes() {
    // Create the bounding box.
    std::vector<int> box = std::vector<int>
            {this->ix, this->iy, this->fx, this->fy};

    // Create a tuple with the label and bounding box
    // and add it to the list of bounding boxes.
    this->bounding_boxes.emplace_back(
            std::make_tuple(this->current_label, box));

    // Reset the x/y coordinates and drawing mode.
    this->ix = -1; this->iy = -1;
    this->fx = -1; this->fy = -1;
    this->is_drawing = false;
}

void AnnotationHandler::add_buttons_to_image() {
    // First, check whether there is even an image.
    if (this->image_cache.empty()) {
        const char* msg = "No image found to annotate";
        perror(msg); exit(1);
    }

    // Get the image width.
    int width = this->image_cache.cols;

    // Determine the total width of each of the different buttons.
    int button_width = (int)width / (int)this->label_list.size();

    // Create the coordinates of the different buttons.
    vector<vector<Point>> point_coordinates(this->label_list.size());
    for (int i = 0; i < this->label_list.size(); ++i) {
        point_coordinates[i].push_back(Point(i * button_width, 0));
        point_coordinates[i].push_back(Point((i + 1) * button_width, 50));
    }

    // Create the canvas.
    Mat3b canvas(this->image_cache.rows + 50, this->image_cache.cols, Vec3b(0, 0, 0));

    // Create the different buttons.
    for (int i = 0; i < point_coordinates.size(); ++i) {
        // Extract the set of coordinates from the current list.
        vector<Point> coords = point_coordinates[i];
        // Construct the button and add it to the list.
        Rect button(coords[0], coords[1]);
        // Add the button to the actual image.
        canvas(button) = Vec3b(150, 150, 150);
        // Add the label for the button.
        vector<int> points = AnnotationHandler::calculate_text_coordinates(
                button, this->label_list[i].c_str());
        putText(canvas(button), this->label_list[i],
                Point(points[0], points[1]),
                FONT_HERSHEY_SIMPLEX, 1, AnnotationHandler::colors[i], 2);
        // Add the button to the list.
        this->buttons.emplace_back(button);
    }

    // Copy the image onto the canvas.
    this->image_cache.clone().copyTo(canvas(
            Rect(0, 50, this->image_cache.cols, this->image_cache.rows)));
    this->image = canvas;
}

void AnnotationHandler::clear_buttons() {
    // Iterate over the different buttons.
    for (int i = 0; i < this->buttons.size(); ++i) {
        // Extract the button from the current list.
        Rect button = this->buttons[i];
        // Add the button to the actual image.
        this->image(button) = Vec3b(150, 150, 150);
        // Add the label for the button.
        vector<int> points = AnnotationHandler::calculate_text_coordinates(
                button, this->label_list[i].c_str());
        putText(this->image(button), this->label_list[i],
                Point(points[0], points[1]),
                FONT_HERSHEY_SIMPLEX, 1, AnnotationHandler::colors[i], 2);
    }
}

void AnnotationHandler::dispatch_handler(int event, int x, int y, int flags, void* param) {
    // First, check whether a button has been clicked.
    if (((AnnotationHandler*)param)->button_click_handler(event, x, y))
        return;

    // Otherwise, dispatch to the correct mode handler.
    std::string mode_choice = std::string(((AnnotationHandler*)param)->mode);
    if (mode_choice == "debug") {
        print_info(event, x, y);
    } else if (mode_choice == "two-click") {
        ((AnnotationHandler*)param)->two_click_handler(event, x, y);
    }
}

bool AnnotationHandler::button_click_handler(int event, int x, int y) {
    // Iterate over the buttons and check if any have been pressed.
    if (event == EVENT_LBUTTONDOWN) {
        for (int i = 0; i < this->buttons.size(); ++i) {
            // Get the button.
            const Rect& button = this->buttons[i];
            // Check whether the button contains the point.
            if (button.contains(Point(x, y))) {
                // If it does, then first update the current label.
                this->current_label = this->label_list[i].c_str();
                // Then, update the button animations.
                this->update_button_animations(i);
                // Finally, exit the loop.
                return true;
            }
        }
    }

    // Otherwise, no button contains the point.
    return false;
}

void AnnotationHandler::two_click_handler(int event, int x, int y) {
    // Check whether the mouse button is pressed.
    if (event == EVENT_LBUTTONDOWN) {
        // Check whether the annotation is in progress
        // or if this is the start of a new one.
        if (!this->is_drawing) {
            // If it is the first click, then set
            // drawing mode to true and track the position.
            this->ix = x; this->iy = y;
            this->is_drawing = true;
            // Create a small circle at the point to let
            // the user know where the annotation began.
            circle(this->image, Point(this->ix, this->iy), 1,
                   AnnotationHandler::colors[this->clicked_index], 3);
        } else {
            // Otherwise, end the annotation and draw a
            // rectangle in the location where it should be,
            // and set the new final annotation position.
            rectangle(this->image, Point(this->ix, this->iy),Point(x, y),
                      AnnotationHandler::colors[this->clicked_index], 3);
            this->fx = x; this->fy = y;
            this->is_drawing = false;
            // Update the list of bounding boxes.
            this->update_bounding_boxes();
        }
    }
}

void AnnotationHandler::print_info(int event, int x, int y) {
    // Print out the coordinates of the mouse whenever
    // the left button is pressed down (for tracking).
    if (event == EVENT_LBUTTONDOWN) {
        cout << x << " " << y << endl;
    }
}

void AnnotationHandler::update_button_animations(int new_index) {
    // First, clear the animation from the currently clicked button.
    if (this->clicked_index != -1) {
        this->clear_buttons();
    }

    // Then, add an animation to the new button.
    Rect clicked_button = this->buttons[new_index];
    // Add the button to the actual image.
    this->image(clicked_button) = Vec3b(100, 100, 100);
    // Add the label for the button.
    vector<int> points = AnnotationHandler::calculate_text_coordinates(
            clicked_button, this->current_label);
    putText(this->image(clicked_button), this->current_label,
            Point(points[0], points[1]),FONT_HERSHEY_SIMPLEX,
            1, AnnotationHandler::colors[new_index], 2);

    // Finally, update the new current clicked index.
    this->clicked_index = new_index;
}

std::vector<int> AnnotationHandler::calculate_text_coordinates(const cv::Rect& button, const char* label){
    // Calculate the text size.
    int baseline = 0;
    Size text_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, 1, 2, &baseline);

    // Calculate the coordinates.
    int text_x = (button.width - text_size.width) / 2;
    int text_y = (button.height + text_size.height) / 2;

    // Return a vector of the coordinates.
    return vector<int> {text_x, text_y};
};


