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
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated pose estimator with NodeID: " << node;
  PoseEstimatorMeasurementCodec::Initialize(frame_);
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
i3ds::EmulatedPoseEstimator::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated pose estimator with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.estimated_pose.position.data.nCount = 3;
  frame_.estimated_pose.position.data.arr[0] = 1.0;
  frame_.estimated_pose.position.data.arr[1] = 2.0;
  frame_.estimated_pose.position.data.arr[2] = 3.0;

  frame_.estimated_pose.orientation.im.nCount = 3;
  frame_.estimated_pose.orientation.im.arr[0] = 1.0;
  frame_.estimated_pose.orientation.im.arr[1] = 2.0;
  frame_.estimated_pose.orientation.im.arr[2] = 3.0;
  frame_.estimated_pose.orientation.re = 4.0;

  publisher_.Send<MeasurementTopic>(frame_);

  return true;
}
