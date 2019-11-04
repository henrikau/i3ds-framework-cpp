///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POSE_ESTIMATOR_HPP
#define __I3DS_POSE_ESTIMATOR_HPP

#include <i3ds/PoseEstimator.h>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>

namespace i3ds
{

CODEC(PoseEstimatorMeasurement);

class PoseEstimator : public Sensor
{
public:

  typedef Topic<128, PoseEstimatorMeasurementCodec> MeasurementTopic;

  // Shared pointer for pose estimator interfaces.
  typedef std::shared_ptr<PoseEstimator> Ptr;

  // Constructor and destructor.
  PoseEstimator(NodeID node) : Sensor(node) {};
  virtual ~PoseEstimator() = default;

};

} // namespace i3ds

#endif
