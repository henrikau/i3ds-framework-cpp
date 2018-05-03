///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/camera_client.hpp>

i3ds::CameraClient::CameraClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
  Camera::ConfigurationService::Initialize(config_);
}

void
i3ds::CameraClient::load_config()
{
  Call<Camera::ConfigurationService>(config_);
}

void
i3ds::CameraClient::set_exposure(ShutterTime shutter, SensorGain gain)
{
  Camera::ExposureService::Data command;

  command.request.shutter = shutter;
  command.request.gain = gain;

  Call<Camera::ExposureService>(command);
}

void
i3ds::CameraClient::set_auto_exposure(bool enable, ShutterTime max_shutter, SensorGain max_gain)
{
  Camera::AutoExposureService::Data command;

  command.request.enable = enable;
  command.request.max_shutter = max_shutter;
  command.request.max_gain = max_gain;

  Call<Camera::AutoExposureService>(command);
}

void
i3ds::CameraClient::set_region(bool enable, PlanarRegion region)
{
  Camera::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  Call<Camera::RegionService>(command);
}

void
i3ds::CameraClient::set_flash(bool enable, FlashStrength strength)
{
  Camera::FlashService::Data command;

  command.request.enable = enable;
  command.request.strength = strength;

  Call<Camera::FlashService>(command);
}

void
i3ds::CameraClient::set_pattern(bool enable, PatternSequence sequence)
{
  Camera::PatternService::Data command;

  command.request.enable = enable;
  command.request.sequence = sequence;

  Call<Camera::PatternService>(command);
}
