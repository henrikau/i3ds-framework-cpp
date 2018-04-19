///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "i3ds/sensors/camera_client.hpp"

i3ds::CameraClient::CameraClient(Context::Ptr context, NodeID sensor)
  : Client(context, sensor)
{
  Camera::ConfigurationService::Initialize(config_);
}

bool
i3ds::CameraClient::load_configuration()
{
  return Call<Camera::ConfigurationService>(config_, 1000);
}

bool
i3ds::CameraClient::set_state(StateCommand state)
{
  Sensor::StateService::Data command;

  command.request = state;

  return Call<Sensor::StateService>(command, 1000);
}

bool
i3ds::CameraClient::set_rate(SampleRate rate)
{
  Sensor::SampleService::Data command;

  command.request.rate = rate;
  command.request.count = 0;

  return Call<Sensor::SampleService>(command, 1000);
}

bool
i3ds::CameraClient::set_exposure(ExposureTime exposure, SensorGain gain)
{
  Camera::ExposureService::Data command;

  command.request.exposure = exposure;
  command.request.gain = gain;

  return Call<Camera::ExposureService>(command, 1000);
}

bool
i3ds::CameraClient::set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain)
{
  Camera::AutoExposureService::Data command;

  command.request.enable = enable;
  command.request.max_exposure = max_exposure;
  command.request.max_gain = max_gain;

  return Call<Camera::AutoExposureService>(command, 1000);
}

bool
i3ds::CameraClient::set_region(bool enable, PlanarRegion region)
{
  Camera::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  return Call<Camera::RegionService>(command, 1000);
}

bool
i3ds::CameraClient::set_flash(bool enable, FlashStrength strength)
{
  Camera::FlashService::Data command;

  command.request.enable = enable;
  command.request.strength = strength;

  return Call<Camera::FlashService>(command, 1000);
}

bool
i3ds::CameraClient::set_pattern(bool enable, PatternSequence sequence)
{
  Camera::PatternService::Data command;

  command.request.enable = enable;
  command.request.sequence = sequence;

  return Call<Camera::PatternService>(command, 1000);
}
