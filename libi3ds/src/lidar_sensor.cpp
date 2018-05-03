///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/lidar_sensor.hpp>
#include <i3ds/exception.hpp>

void
i3ds::LIDAR::Attach(Server& server)
{
  using std::placeholders::_1;

  Sensor::Attach(server);

  server.Attach<RegionService>(node(), std::bind(&i3ds::LIDAR::handle_region, this, _1));
  server.Attach<ConfigurationService>(node(), std::bind(&i3ds::LIDAR::handle_configuration, this, _1));
}

void
i3ds::LIDAR::handle_region(RegionService::Data& command)
{
  throw CommandError(error_unsupported, "Region of interest not supported");
}

void
i3ds::LIDAR::handle_configuration(ConfigurationService::Data& config) const
{
  config.response.region_enabled = region_enabled();
  config.response.region = region();
}
