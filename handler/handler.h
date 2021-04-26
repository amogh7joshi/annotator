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

#ifndef ANNOTATION_HANDLER_H
#define ANNOTATION_HANDLER_H

#include <string>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>

#include "../system/paths.h"

/**
 * This class handles the events which take
 * place during the displaying and annotation
 * of each individual image.
 *
 * In turn, it serves as a callback method for
 * the @code cv::setMouseCallback @endcode
 * event handler, as well as the trackbar which
 * is created by the @code cv::createTrackbar @endcode
 * callback (which is used for choosing the class).
 */
class AnnotationHandler {
protected:
    /* The mode of the handler, e.g., whether
     * to use a two-click or drag/drop interface. */
    const char* mode;

    /* For both modes, the initial position on
     * each set of annotations will be tracked. */
    int ix = -1;
    int iy = -1;

    /* Following the creation of an annotation, the
     * final position will be tracked. */
    int fx = -1;
    int fy = -1;

    /* For both modes, track when the user is
     * in the middle of drawing a bounding box. */
    bool is_drawing = false;

    /* Following a button click, this tracks which
     * button index was clicked, so that it can be cleared. */
    int clicked_index = -1;

    /* At all times, a specific image should be tracked. */
    cv::Mat image;

    /* Also, save a cache of the image for resetting. */
    cv::Mat image_cache;

    /* The class will always contain a list of labels
     * which it will call from when choosing a one. */
    std::vector<std::string> label_list;

    /* Furthermore, the current label of the annotation
     * being drawn will also be tracked at the time. */
    const char* current_label = "";

    /* Finally, there will always be a set of buttons
     * corresponding to the different labels being tracked. */
    std::vector<cv::Rect> buttons;

    /* For each of the different labels, a different color will
     * be picked out of this vector of colors. */
    static std::vector<cv::Scalar> colors;

private:
    /* During the period that each image is being annotated,
     * each individual bounding box coordinates as well as its
     * relevant label will be stored in this vector. */
    std::vector<std::tuple<const char*, std::vector<int>>> bounding_boxes;

public:
    /**
     * Initializes the class with the chosen
     * event handling mode.
     * @param mode_choice: The mode of choice.
     */
    explicit AnnotationHandler(const char* mode_choice,
                               const std::vector<std::string>& class_list);

    /**
     * Create an annotation session involving the
     * provided image path.
     */
    int annotate(const char* image_path);

    /**
     * Returns the bounding box annotation positions
     * from the image annotation session.
     */
    std::vector<std::tuple<const char*, std::vector<int>>> get_bounding_boxes() {
        return bounding_boxes;
    }

private:
    /**
     * Update the current image and class settings in
     * order to transition between stages.
     */
    void update_states(cv::Mat& image);

    /**
     * Updates the vector of bounding boxes with a new
     * tuple consisting of a new label and bounding box.
     */
    void update_bounding_boxes();

    /**
     * Initializes the image with a set of buttons which
     * can be "clicked" to choose different labels.
     */
    void add_buttons_to_image();

    /**
     * Similar to the above method `add_buttons_to_image`,
     * however this is used as an update method as opposed
     * to an initialization method, so it retains all of
     * the different bounding boxes when called.
     */
    void clear_buttons();

    /**
     * A static wrapper method for the primary dispatch
     * handler which is contained in the class. This is the
     * actual method which is used to handle mouse events,
     * and is controlled internally as such.
     */
    static void dispatch_handler(int event, int x, int y, int flags, void* param);

    /**
     * This is a method accessed by `dispatch_handler`,
     * but rather than corresponding to a certain mode,
     * this corresponds to any clicks in the region of
     * the display window that correspond to a label button.
     */
    bool button_click_handler(int event, int x, int y);

    /**
     * Handles the `two-click` mode, creating a
     * rectangle corresponding to two clicks.
     */
    void two_click_handler(int event, int x, int y);

    /**
     * Handles the `debug` mode, printing out info.
     */
    static void print_info(int event, int x, int y);

    /**
     * Updates the different buttons following a click.
     * First, all of the other static buttons have the
     * clicking animation removed, and then the new button
     * has the clicking animation added.
     */
    void update_button_animations(int new_index);

    /**
     * Calculates the text coordinates on a button.
     */
    static std::vector<int> calculate_text_coordinates(const cv::Rect& button, const char* label);

};

#endif //ANNOTATION_HANDLER_H
