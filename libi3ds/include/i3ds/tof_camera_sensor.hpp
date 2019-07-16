///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_TOF_CAMERA_HPP
#define __I3DS_TOF_CAMERA_HPP

#include <i3ds/ToFCamera.h>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>
#include <i3ds/depthmap.hpp>

namespace i3ds
{

CODEC(ToFRegion);
CODEC(ToFRange);
CODEC(ToFConfiguration);

class ToFCamera : public Sensor
{
public:

  // Shared pointer for ToF camera interfaces.
  typedef std::shared_ptr<ToFCamera> Ptr;

  // ToFCamera services.
  typedef Command<16, ToFRegionCodec>        RegionService;
  typedef Command<17, ToFRangeCodec>         RangeService;
  typedef Query  <18, ToFConfigurationCodec> ConfigurationService;

  // ToFCamera topics.
  typedef Topic<128, DepthMapCodec> MeasurementTopic;

  // Constructor and destructor.
  ToFCamera(NodeID node) : Sensor(node) {};
  virtual ~ToFCamera() {};

  // Get the region of interest enabled for ToF-camera.
  virtual bool region_enabled() const {return false;}

  // Get the region of interest for the ToF-camera.
  virtual PlanarRegion region() const {return {0,0,0,0};}

  // Get the min range configuration of the ToF-camera.
  virtual double range_min_depth() const {return 0.0;}

  // Get the max range configuration of the ToF-camera.
  virtual double range_max_depth() const {return 1.0e6;}

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for ToF-camera region of interest command.
  virtual void handle_region(RegionService::Data& command);

  // Handler for ToF-camera range command.
  virtual void handle_range(RangeService::Data& command);

  // Handler for camera configuration query.
  virtual void handle_configuration(ConfigurationService::Data& config);
};

} // namespace i3ds

#endif
