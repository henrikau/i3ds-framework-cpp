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
#include <i3ds/frame.hpp>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>

namespace i3ds
{

CODEC(PoseEstimatorImagingMode);
CODEC(PoseEstimatorCameraSelect);
CODEC(PoseEstimatorConfiguration);

CODEC(PoseEstimatorMeasurement);

class PoseEstimator : public Sensor
{
public:

  // Shared pointer for pose estimator interfaces.
  typedef std::shared_ptr<PoseEstimator> Ptr;

  typedef Command<16, PoseEstimatorImagingModeCodec> ImagingModeService;
  typedef Command<17, PoseEstimatorCameraSelectCodec> CameraSelectService;
  typedef Query<18, PoseEstimatorConfigurationCodec> ConfigurationService;

  typedef Topic<128, FrameCodec> FrameTopic;
  typedef Topic<129, PoseEstimatorMeasurementCodec> MeasurementTopic;

  // Constructor and destructor.
  PoseEstimator(NodeID node) : Sensor(node) {};
  virtual ~PoseEstimator() = default;

  virtual bool imaging_mode() const {return false;}

  virtual uint8_t selected_camera() const {return 0;}

  virtual void Attach(Server& server);

protected:

  // Handler for imaging mode command
  virtual void handle_imaging_mode(ImagingModeService::Data& command) = 0;

  // Handler for camera select command
  virtual void handle_camera_select(CameraSelectService::Data& command) = 0;

  // Handler for configuration query
  virtual void handle_configuration(ConfigurationService::Data& config);
};

} // namespace i3ds

#endif
