///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_LIDAR_HPP
#define __I3DS_LIDAR_HPP

#include <i3ds/LIDAR.h>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>
#include <i3ds/pointcloud.hpp>

namespace i3ds
{

CODEC(LIDARRegion);
CODEC(LIDARConfiguration);

class LIDAR : public Sensor
{
public:

  // Shared pointer for LIDAR interfaces.
  typedef std::shared_ptr<LIDAR> Ptr;

  // LIDAR services.
  typedef Command<16, LIDARRegionCodec>        RegionService;
  typedef Query  <17, LIDARConfigurationCodec> ConfigurationService;

  // LIDAR topics
  typedef Topic<128, PointCloudCodec> MeasurementTopic;

  // Constructor and destructor.
  LIDAR(NodeID node) : Sensor(node) {};
  virtual ~LIDAR() {};

  // Get the region of interest enabled for LIDAR.
  virtual bool region_enabled() const {return false;}

  // Get the region of interest for the LIDAR.
  virtual PolarRegion region() const {return {0,0,0,0};}

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for LIDAR region of interest command.
  virtual void handle_region(RegionService::Data& command);

  // Handler for LIDAR configuration query.
  virtual void handle_configuration(ConfigurationService::Data& config);
};

} // namespace i3ds

#endif
