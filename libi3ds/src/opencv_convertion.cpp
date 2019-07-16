///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include <i3ds/opencv_convertion.hpp>
#include <i3ds/frame.hpp>
#include <i3ds/depthmap.hpp>

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat
i3ds::frame_to_cv_mat(const DepthMap& frame, int image_number)
{
  const int rows = frame.descriptor.height;
  const int cols = frame.descriptor.width;

  cv::Mat raw(rows, cols, CV_32FC1, const_cast<float*>(frame.depths.data()));

  double max, min;

  cv::minMaxLoc(raw, &min, &max);

  cv::Mat res;

  raw.convertTo(res, CV_16U, 65535 / max);

  return res;
}


cv::Mat
i3ds::frame_to_cv_mat(const Frame& frame, int image_number)
{
  const FrameDescriptor& d = frame.descriptor;

  int rows = d.region.size_y;
  int cols = d.region.size_x;
  int pixel_size = d.pixel_size;

  int cv_type = CV_16UC1;
  double scaling_factor = 1;

  if (d.frame_mode == mode_rgb)
    {
      if (pixel_size == 3) { cv_type = CV_8UC3; }
      if (pixel_size == 6) { cv_type = CV_16UC3; }
      scaling_factor = pow(2,(8 * (pixel_size/3) - d.data_depth));
    }
  else
    {
      if (pixel_size == 1) { cv_type = CV_8UC1; }
      if (pixel_size == 2) { cv_type = CV_16UC1; }
      scaling_factor = pow(2,(8 * pixel_size - d.data_depth));
    }

  cv::Mat mat(rows, cols, cv_type, (char*)frame.image_data(image_number));

  if (scaling_factor != 1)
    {
      mat *= scaling_factor;
    }
  return mat;
}

void
i3ds::set_descriptor_from_mat(Frame& frame, const cv::Mat& mat,
                              unsigned long timestamp_us, Frame_mode_t mode, int image_count)
{
  FrameDescriptor* d = &frame.descriptor;

  d->attributes.timestamp = timestamp_us;
  d->attributes.validity = sample_valid;

  d->region.size_x = mat.cols;
  d->region.size_y = mat.rows;

  if (mat.depth() == CV_16UC1)
    {
      d->data_depth = 16;
      d->pixel_size = mat.channels() * 2;
    }
  else
    {
      d->data_depth = 8;
      d->pixel_size = mat.channels();
    }

  d->frame_mode = mode;
  d->image_count = image_count;
}

void
i3ds::add_image_data(Frame& frame, const cv::Mat& mat)
{
  frame.append_image(mat.data, image_size(frame.descriptor));
}
