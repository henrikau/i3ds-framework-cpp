///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_POSE_ESTIMATOR_HPP
#define __I3DS_EMULATED_POSE_ESTIMATOR_HPP

#include <i3ds/topic.hpp>
#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/pose_estimator_sensor.hpp>

namespace i3ds
{

class EmulatedPoseEstimator : public PoseEstimator
{
public:

  typedef std::shared_ptr<EmulatedPoseEstimator> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<EmulatedPoseEstimator>(context, id);
  }

  EmulatedPoseEstimator(Context::Ptr context, NodeID id);
  virtual ~EmulatedPoseEstimator();

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  bool send_sample(unsigned long timestamp_us);

  Sampler sampler_;

  Publisher publisher_;
  PoseEstimatorMeasurement frame_;
};

} // namespace i3ds

#endif
