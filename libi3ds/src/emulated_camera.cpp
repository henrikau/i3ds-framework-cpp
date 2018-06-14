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

namespace fs = boost::filesystem;

i3ds::EmulatedCamera::EmulatedCamera(Context::Ptr context, NodeID node, CameraProperties prop)
  : Camera(node),
    prop_(prop),
    sampler_(std::bind(&i3ds::EmulatedCamera::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated camera with NodeID: " << node;

  shutter_ = 0;
  gain_ = 0.0;

  auto_exposure_enabled_ = false;
  max_shutter_ = 0;
  max_gain_ = 0.0;

  region_.size_x = prop.width;
  region_.size_y = prop.height;
  region_.offset_x = 0;
  region_.offset_y = 0;

  flash_enabled_ = false;
  flash_strength_ = 0.0;

  pattern_enabled_ = false;
  pattern_sequence_ = 0;

  load_images();
}

i3ds::EmulatedCamera::~EmulatedCamera()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated camera with NodeID: " << node();
}

void
i3ds::EmulatedCamera::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedCamera::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedCamera::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedCamera::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedCamera::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

void
i3ds::EmulatedCamera::handle_exposure(ExposureService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " handle_exposure()";
  auto_exposure_enabled_ = false;
  shutter_ = command.request.shutter;
  gain_ = command.request.gain;
}

void
i3ds::EmulatedCamera::handle_auto_exposure(AutoExposureService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " handle_auto_exposure()";
  auto_exposure_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      max_shutter_ = command.request.max_shutter;
      max_gain_ = command.request.max_gain;
    }
}

void
i3ds::EmulatedCamera::handle_region(RegionService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " handle_region()";
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

void
i3ds::EmulatedCamera::handle_flash(FlashService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " handle_flash()";
  flash_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      flash_strength_ = command.request.strength;
    }
}

void
i3ds::EmulatedCamera::handle_pattern(PatternService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated camera with NodeID: " << node() << " do_pattern()";
  pattern_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      pattern_sequence_ = command.request.sequence;
    }
}

void
i3ds::EmulatedCamera::load_images()
{
  if (prop_.sample_dir.size() > 0)
    {
      try
        {
          std::vector<std::string> file_names;
          fs::path full_path( fs::initial_path<fs::path>() );
          full_path = fs::system_complete(fs::path(prop_.sample_dir));

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
                  sample_images_.push_back(cv::imread(file_name, CV_LOAD_IMAGE_GRAYSCALE));
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
      for (int i = 0; i < 10; i++)
        {
          cv::Mat img(prop_.height, prop_.width, CV_16U, cv::Scalar(0));

          cv::randu(img, cv::Scalar(0), cv::Scalar((1 << 16) - 1));

          sample_images_.push_back(img);
        }
    }
}

cv::Mat
i3ds::EmulatedCamera::next_image()
{
  if (++current_image_index_ >= sample_images_.size())
    {
      current_image_index_ = 0;
    }

  return sample_images_[current_image_index_];
}

bool
i3ds::EmulatedCamera::send_sample(unsigned long timestamp_us)
{
  FrameTopic::Data frame;

  FrameTopic::Initialize(frame);

  FrameDescriptor* d = &frame.descriptor;

  d->attributes.timestamp.microseconds = timestamp_us;
  d->attributes.validity = sample_valid;

  cv::Mat current_image = next_image();

  d->region.size_x = current_image.cols;
  d->region.size_y = current_image.rows;

  if (current_image.depth() == CV_16U)
    {
      d->data_depth = 16;
      d->pixel_size = current_image.channels() * 2;
    }
  else
    {
      d->data_depth = 8;
      d->pixel_size = current_image.channels();
    }

  d->frame_mode = mode_mono;
  d->image_count = prop_.image_count;

  const int size = d->region.size_x  * d->region.size_y * d->pixel_size;

  for (unsigned int i = 0; i < d->image_count; i++)
    {
      Image image;

      image.size = size;
      image.data = current_image.data;

      frame.image.push_back(image);
    }

  BOOST_LOG_TRIVIAL(trace) << timestamp_us
                           << ": Send frame with "
                           << d->image_count << " images of "
                           << size / 1024.0 << " KiB";

  publisher_.Send<FrameTopic>(frame);

  return true;
}
