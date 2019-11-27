///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <i3ds/emulated_camera.hpp>
#include <i3ds/opencv_convertion.hpp>

namespace fs = boost::filesystem;

i3ds::EmulatedCamera::EmulatedCamera(Context::Ptr context, NodeID node, Parameters param)
  : GigECamera(context, node, param),
    sampler_(std::bind(&i3ds::EmulatedCamera::generate_sample, this))
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated camera with NodeID: " << node;

  shutter_ = 1000;
  gain_ = 0.0;

  auto_shutter_enabled_ = false;
  auto_shutter_limit_ = getMaxShutter();

  auto_gain_enabled_ = false;
  auto_gain_limit_ = getMaxGain();

  sensor_.width = param.width;
  sensor_.height = param.height;
  sensor_.x = 0;
  sensor_.y = 0;

  region_.width = sensor_.width;
  region_.height = sensor_.height;
  region_.x = 0;
  region_.y = 0;

  set_device_name("Emulated camera");

  load_images(param.sample_dir);
}

i3ds::EmulatedCamera::~EmulatedCamera()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated camera with NodeID: " << node();
}

void
i3ds::EmulatedCamera::Open()
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID: " << node() << " Open()";
}

void
i3ds::EmulatedCamera::Close()
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID: " << node() << " Close()";
}

void
i3ds::EmulatedCamera::Start()
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID: " << node() << " Start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedCamera::Stop()
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID: " << node() << " Stop()";
  sampler_.Stop();
}

bool
i3ds::EmulatedCamera::setInternalTrigger(int64_t period_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID: " << node() << " set internal trigger " << period_us;
  return 20000 <= period_us && period_us <= 10000000;
}

int64_t
i3ds::EmulatedCamera::getSensorWidth() const
{
  return sensor_.width;
}

int64_t
i3ds::EmulatedCamera::getSensorHeight() const
{
  return sensor_.height;
}

bool
i3ds::EmulatedCamera::isRegionSupported() const
{
  return param_.image_count == 1;
}

int64_t
i3ds::EmulatedCamera::getRegionWidth() const
{
  return region_.width;
}

int64_t
i3ds::EmulatedCamera::getRegionHeight() const
{
  return region_.height;
}

int64_t
i3ds::EmulatedCamera::getRegionOffsetX() const
{
  return region_.x;
}

int64_t
i3ds::EmulatedCamera::getRegionOffsetY() const
{
  return region_.y;
}

bool
i3ds::EmulatedCamera::check_region() const
{
  return region_.x >= 0
         && region_.y >= 0
         && region_.x + region_.width <= sensor_.width
         && region_.y + region_.height <= sensor_.height;
}

void
i3ds::EmulatedCamera::setRegionWidth(int64_t width)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID " << node() << ": set region width " << width;

  region_.width = width;

  if (!check_region())
    {
      BOOST_LOG_TRIVIAL(error) << "Invalid ROI after width " << sensor_ << " " << region_;
      throw DeviceError("Invalid ROI after width");
    }
}

void
i3ds::EmulatedCamera::setRegionHeight(int64_t height)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID " << node() << ": set region height " << height;

  region_.height = height;

  if (!check_region())
    {
      throw DeviceError("Invalid ROI after height");
    }
}

void
i3ds::EmulatedCamera::setRegionOffsetX(int64_t offset_x)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID " << node() << ": set region offset x " << offset_x;

  region_.x = offset_x;

  if (!check_region())
    {
      throw DeviceError("Invalid ROI");
    }
}

void
i3ds::EmulatedCamera::setRegionOffsetY(int64_t offset_y)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated camera with NodeID " << node() << ": set region offset y " << offset_y;

  region_.y = offset_y;

  if (!check_region())
    {
      throw DeviceError("Invalid ROI");
    }
}

int64_t
i3ds::EmulatedCamera::getShutter() const
{
  return shutter_;
}

int64_t
i3ds::EmulatedCamera::getMaxShutter() const
{
  return 10000000;
}

int64_t
i3ds::EmulatedCamera::getMinShutter() const
{
  return 10;
}

void
i3ds::EmulatedCamera::setShutter(int64_t shutter_us)
{
  if (shutter_us < getMinShutter())
    {
      throw DeviceError("Too short shutter");
    }

  if (shutter_us > getMaxShutter())
    {
      throw DeviceError("Too long shutter");
    }

  shutter_ = shutter_us;
}

bool
i3ds::EmulatedCamera::isAutoShutterSupported() const
{
  return true;
}

bool
i3ds::EmulatedCamera::getAutoShutterEnabled() const
{
  return auto_shutter_enabled_;
}

void
i3ds::EmulatedCamera::setAutoShutterEnabled(bool enable)
{
  auto_shutter_enabled_ = enable;
}

int64_t
i3ds::EmulatedCamera::getAutoShutterLimit() const
{
  return auto_shutter_limit_;
}

int64_t
i3ds::EmulatedCamera::getMaxAutoShutterLimit() const
{
  return getMaxShutter();
}

int64_t
i3ds::EmulatedCamera::getMinAutoShutterLimit() const
{
  return getMinShutter();
}

void
i3ds::EmulatedCamera::setAutoShutterLimit(int64_t shutter_limit)
{
  if (shutter_limit < getMinAutoShutterLimit())
    {
      throw DeviceError("Too short shutter limit");
    }

  if (shutter_limit > getMaxAutoShutterLimit())
    {
      throw DeviceError("Too long shutter limit");
    }

  auto_shutter_limit_ = shutter_limit;
}

double
i3ds::EmulatedCamera::getGain() const
{
  return gain_;
}

double
i3ds::EmulatedCamera::getMaxGain() const
{
  return 30.0;
}

double
i3ds::EmulatedCamera::getMinGain() const
{
  return 0.0;
}

void
i3ds::EmulatedCamera::setGain(double gain)
{
  if (gain < getMinGain())
    {
      throw DeviceError("Too low gain");
    }

  if (gain > getMaxGain())
    {
      throw DeviceError("Too high gain");
    }

  gain_ = gain;
}

bool
i3ds::EmulatedCamera::isAutoGainSupported() const
{
  return true;
}

bool
i3ds::EmulatedCamera::getAutoGainEnabled() const
{
  return auto_gain_enabled_;
}

void
i3ds::EmulatedCamera::setAutoGainEnabled(bool enable)
{
  auto_gain_enabled_ = enable;
}

double
i3ds::EmulatedCamera::getAutoGainLimit() const
{
  return auto_gain_limit_;
}

double
i3ds::EmulatedCamera::getMaxAutoGainLimit() const
{
  return getMaxGain();
}

double
i3ds::EmulatedCamera::getMinAutoGainLimit() const
{
  return getMinGain();
}

void
i3ds::EmulatedCamera::setAutoGainLimit(double gain_limit)
{
  if (gain_limit < getMinAutoGainLimit())
    {
      throw DeviceError("Too low gain limit");
    }

  if (gain_limit > getMaxAutoGainLimit())
    {
      throw DeviceError("Too high gain limit");
    }

  auto_gain_limit_ = gain_limit;
}

void
i3ds::EmulatedCamera::load_images(std::string sample_dir)
{
  if (sample_dir.size() > 0)
    {
      int imread_mode = param_.frame_mode == mode_rgb ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE;
      try
        {
          std::vector<std::string> file_names;
          fs::path full_path( fs::initial_path<fs::path>() );
          full_path = fs::system_complete(fs::path(sample_dir));

          if (fs::exists(full_path) && fs::is_directory(full_path))
            {
              fs::directory_iterator end_iter;

              for (fs::directory_iterator dir_itr(full_path); dir_itr != end_iter; ++dir_itr)
                {
                  if (fs::is_regular_file(dir_itr->status()))
                    {
                      file_names.push_back(dir_itr->path().string());
                    }
                }

              std::sort(file_names.begin(), file_names.end());

              for (std::string file_name : file_names)
                {
                  sample_images_.push_back(cv::imread(file_name, imread_mode));
                  BOOST_LOG_TRIVIAL(trace) << "Emulated camera loaded file: " << file_name;
                }
            }
        }
      catch (std::exception e)
        {
          // If anything fails, sample_images_ remains empty, and is not used
          BOOST_LOG_TRIVIAL(warning) << "Error loading sample images: " << e.what();
        }
    }
  else
    {
      BOOST_LOG_TRIVIAL(trace) << "Emulated camera generating noise images";
      int cv_type = CV_16UC1;

      if (param_.frame_mode == mode_rgb)
      {
          if (param_.pixel_size == 3) { cv_type = CV_8UC3; }
          if (param_.pixel_size == 6) { cv_type = CV_16UC3; }
      }
      else
      {
          if (param_.pixel_size == 1) { cv_type = CV_8UC1; }
          if (param_.pixel_size == 2) { cv_type = CV_16UC1; }
      }

      for (int i = 0; i < 10; i++)
        {
          cv::Mat img(getSensorHeight(), getSensorWidth(), cv_type, cv::Scalar(0));

          int max_pixel_val = (1 << param_.data_depth) - 1;
          if (param_.frame_mode == mode_rgb)
            {
              cv::randu(img, cv::Scalar(0,0,0), cv::Scalar(max_pixel_val, max_pixel_val, max_pixel_val));
            }
          else
            {
              cv::randu(img, cv::Scalar(0), cv::Scalar(max_pixel_val));
            }

          sample_images_.push_back(img);
        }
    }
}

bool
i3ds::EmulatedCamera::generate_sample()
{
  // Get sample image from index.
  if (++current_image_index_ >= sample_images_.size())
    {
      current_image_index_ = 0;
    }

  cv::Mat& full_image = sample_images_[current_image_index_];

  // Take ROI of image.
  cv::Mat image = full_image(region_);

  BOOST_LOG_TRIVIAL(trace) << "Image: "
                           << image.cols << " cols, "
                           << image.rows << " rows, "
                           << image.channels() << " chan, "
                           << image.depth() << " depth";

  send_sample(image.data, region_.width, region_.height);

  return true;
}
