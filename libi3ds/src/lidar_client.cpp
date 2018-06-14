///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/lidar_client.hpp>

i3ds::LIDARClient::LIDARClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
  LIDAR::ConfigurationService::Initialize(config_);
}

void
i3ds::LIDARClient::load_config()
{
  i3ds::SensorClient::load_config();
  Call<LIDAR::ConfigurationService>(config_);
}

void
i3ds::LIDARClient::set_region(bool enable, PolarRegion region)
{
  LIDAR::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  Call<LIDAR::RegionService>(command);
}
