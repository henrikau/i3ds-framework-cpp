///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_LIDAR_CLIENT_HPP
#define __I3DS_LIDAR_CLIENT_HPP

#include <i3ds/sensor_client.hpp>
#include <i3ds/lidar_sensor.hpp>

namespace i3ds
{

class LIDARClient : public SensorClient
{
public:

  typedef std::shared_ptr<LIDARClient> Ptr;

  LIDARClient(Context::Ptr context, NodeID sensor);

  void set_region(bool enable, PolarRegion region);

  bool region_enabled() const {return config_.response.region_enabled;}
  PolarRegion region() const {return config_.response.region;}

  virtual void load_config();

private:

  LIDAR::ConfigurationService::Data config_;
};

} // namespace i3ds

#endif
