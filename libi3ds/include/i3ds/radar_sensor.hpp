///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_RADAR_HPP
#define __I3DS_RADAR_HPP

#include "Radar.h"

#include "sensor.hpp"
#include "service.hpp"
#include "codec.hpp"

namespace i3ds
{

CODEC(RadarRegion);
CODEC(RadarConfiguration);

CODEC(RadarMeasurement100K);
CODEC(RadarMeasurement200K);
CODEC(RadarMeasurement400K);

class Radar : public Sensor
{
public:

  // Radar services.
  typedef Command<16, RadarRegionCodec>        RegionService;
  typedef Query  <17, RadarConfigurationCodec> ConfigurationService;

  // Constructor and destructor.
  Radar(NodeID node) : Sensor(node) {};
  virtual ~Radar() {};

  // Get the region of interest enabled for radar.
  virtual bool region_enabled() const {return false;}

  // Get the region of interest for the radar.
  virtual PlanarRegion region() const {return {0,0,0,0};}

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for radar region of interest command.
  virtual void handle_region(RegionService::Data& command);

  // Handler for radar configuration query.
  virtual void handle_configuration(ConfigurationService::Data& config) const;
};

} // namespace i3ds

#endif
