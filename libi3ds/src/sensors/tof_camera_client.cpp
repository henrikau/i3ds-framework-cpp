///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "i3ds/sensors/tof_camera_client.hpp"

i3ds::ToFCameraClient::ToFCameraClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
  ToFCamera::ConfigurationService::Initialize(config_);
}

void
i3ds::ToFCameraClient::load_config()
{
  Call<ToFCamera::ConfigurationService>(config_);
}

void
i3ds::ToFCameraClient::set_region(bool enable, PlanarRegion region)
{
  ToFCamera::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  Call<ToFCamera::RegionService>(command);
}
