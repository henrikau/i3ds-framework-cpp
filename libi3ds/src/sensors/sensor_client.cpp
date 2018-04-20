///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "i3ds/sensors/sensor_client.hpp"

i3ds::SensorClient::SensorClient(Context::Ptr context, NodeID sensor)
  : Client(context, sensor)
{
}

bool
i3ds::SensorClient::load_status()
{
  return Call<Sensor::StatusService>(status_, 1000);
}

bool
i3ds::SensorClient::set_state(StateCommand state)
{
  Sensor::StateService::Data command;

  command.request = state;

  return Call<Sensor::StateService>(command, 1000);
}

bool
i3ds::SensorClient::set_rate(SampleRate rate)
{
  Sensor::SampleService::Data command;

  command.request.rate = rate;
  command.request.count = 0;

  return Call<Sensor::SampleService>(command, 1000);
}
