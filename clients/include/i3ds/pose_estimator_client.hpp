///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POSE_ESTIMATOR_CLIENT_HPP
#define __I3DS_POSE_ESTIMATOR_CLIENT_HPP

#include <i3ds/sensor_client.hpp>
#include <i3ds/pose_estimator_sensor.hpp>

namespace i3ds
{

class PoseEstimatorClient : public SensorClient
{
public:

  typedef std::shared_ptr<PoseEstimatorClient> Ptr;
  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<PoseEstimatorClient>(context, id);
  }

  PoseEstimatorClient(Context::Ptr context, NodeID sensor);

  void set_imaging_mode(bool imaging_mode);
  void set_selected_camera(uint8_t camera);
  
  bool imaging_mode() const {return config_.response.imaging_mode;}
  uint8_t selected_camera() const {return config_.response.camera_select;}

  virtual void load_config();

private:
  PoseEstimator::ConfigurationService::Data config_;
};

} // namespace i3ds

#endif
