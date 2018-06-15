///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_OPENCV_CONVERT_HPP
#define __I3DS_OPENCV_CONVERT_HPP

#include <i3ds/frame.hpp>

#include <opencv2/core/core.hpp>

namespace i3ds
{

// Creates a cv::Mat representation of a Frame. No data is copied and 
// the returned object is only valid while the Frame is in scope.
cv::Mat frame_to_cv_mat(const Frame& frame, int image_number);


// Sets the contents of a frame-descriptor from a cv::Mat
void set_descriptor_from_mat(Frame& frame, const cv::Mat& mat,
                             unsigned long timestamp_us, Frame_mode_t mode,
                             int image_count);


// Adds image data from a cv::Mat to a Frame.
void add_image_data(Frame& frame, const cv::Mat& mat);

} // namespace i3ds

#endif
