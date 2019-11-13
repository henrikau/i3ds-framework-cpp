///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>

#include <i3ds/emulated_pose_estimator.hpp>

i3ds::EmulatedPoseEstimator::EmulatedPoseEstimator(Context::Ptr context, NodeID node)
  : PoseEstimator(node),
    sampler_(std::bind(&i3ds::EmulatedPoseEstimator::send_sample, this, std::placeholders::_1)),
    publisher_(context, node), n_images_(5), current_image_(0),
    imaging_mode_(false), selected_camera_(0)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated pose estimator with NodeID: " << node;
  PoseEstimatorMeasurementCodec::Initialize(pose_estimate_);
  FrameCodec::Initialize(frame_);
  frame_.descriptor.region.size_x = 640;
  frame_.descriptor.region.size_y = 480;
  frame_.descriptor.region.offset_x = 0;
  frame_.descriptor.region.offset_y = 0;

  frame_.descriptor.frame_mode = mode_mono;
  frame_.descriptor.data_depth = 8;
  frame_.descriptor.pixel_size = 1;

  image_size_ = image_size(frame_.descriptor);
  for (int image = 0; image < n_images_; ++image)
    {
      dummy_images_.push_back((byte*)malloc(image_size_));
      for (size_t i = 0; i < image_size_; ++i)
        {
          dummy_images_[image][i] = rand() % 255;
        }
    }
  set_device_name("Emulated pose estimator");
}

i3ds::EmulatedPoseEstimator::~EmulatedPoseEstimator()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated pose estimator with NodeID: " << node();
}

void
i3ds::EmulatedPoseEstimator::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedPoseEstimator::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedPoseEstimator::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedPoseEstimator::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedPoseEstimator::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

bool
i3ds::EmulatedPoseEstimator::imaging_mode() const
{
    return imaging_mode_;
}

uint8_t
i3ds::EmulatedPoseEstimator::selected_camera() const
{
    return selected_camera_;
}

void 
i3ds::EmulatedPoseEstimator::handle_imaging_mode(ImagingModeService::Data& command)
{
  imaging_mode_ = command.request.imaging_mode;
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " set imaging_mode: " << imaging_mode_;
}

void
i3ds::EmulatedPoseEstimator::handle_camera_select(CameraSelectService::Data& command)
{
  selected_camera_ = command.request.camera_select;
  BOOST_LOG_TRIVIAL(info) << "Emulated pose estimator with NodeID: " << node() << " set selected camera: " << imaging_mode_;
}

bool
i3ds::EmulatedPoseEstimator::send_sample(unsigned long timestamp_us)
{
  if(imaging_mode_)
    {
      BOOST_LOG_TRIVIAL(trace) << "Emulated pose estimator with NodeID: " << node() << " sends image at " << timestamp_us;

      frame_.descriptor.attributes.timestamp = timestamp_us;
      frame_.descriptor.attributes.validity = sample_valid;

      frame_.descriptor.image_count = 1;

      frame_.clear_images();
      frame_.append_image(dummy_images_[current_image_], image_size_);
      if (current_image_ == n_images_ -1)
        {
          current_image_ = 0;
        }
      else
        {
          current_image_++;
        }

      publisher_.Send<FrameTopic>(frame_);
    }
  else
    {

      BOOST_LOG_TRIVIAL(trace) << "Emulated pose estimator with NodeID: " << node() << " sends pose sample at " << timestamp_us;

      pose_estimate_.attributes.timestamp = timestamp_us;
      pose_estimate_.attributes.validity = sample_valid;

      pose_estimate_.estimated_pose.position.data.nCount = 3;
      pose_estimate_.estimated_pose.position.data.arr[0] = 1.0;
      pose_estimate_.estimated_pose.position.data.arr[1] = 2.0;
      pose_estimate_.estimated_pose.position.data.arr[2] = 3.0;

      pose_estimate_.estimated_pose.orientation.im.nCount = 3;
      pose_estimate_.estimated_pose.orientation.im.arr[0] = 1.0;
      pose_estimate_.estimated_pose.orientation.im.arr[1] = 2.0;
      pose_estimate_.estimated_pose.orientation.im.arr[2] = 3.0;
      pose_estimate_.estimated_pose.orientation.re = 4.0;

      publisher_.Send<MeasurementTopic>(pose_estimate_);
    }

  return true;
}
