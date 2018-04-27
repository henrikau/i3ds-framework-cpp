///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_TOF_CAMERA_CLIENT_HPP
#define __I3DS_TOF_CAMERA_CLIENT_HPP

#include "sensor_client.hpp"
#include "tof_camera_sensor.hpp"

namespace i3ds
{

class ToFCameraClient : public SensorClient
{
public:

  ToFCameraClient(Context::Ptr context, NodeID sensor);

  void set_region(bool enable, PlanarRegion region);

  bool region_enabled() const {return config_.response.region_enabled;}
  PlanarRegion region() const {return config_.response.region;}

  virtual void load_config();

private:

  ToFCamera::ConfigurationService::Data config_;
};

} // namespace i3ds

#endif
