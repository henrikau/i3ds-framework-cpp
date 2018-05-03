///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/radar_client.hpp>

i3ds::RadarClient::RadarClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
  Radar::ConfigurationService::Initialize(config_);
}

void
i3ds::RadarClient::load_config()
{
  Call<Radar::ConfigurationService>(config_);
}

void
i3ds::RadarClient::set_region(bool enable, PlanarRegion region)
{
  Radar::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  Call<Radar::RegionService>(command);
}
