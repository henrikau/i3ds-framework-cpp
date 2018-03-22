///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include "sensor.hpp"

const EndpointID i3ds::Sensor::COMMAND = command_Endpoint;
const EndpointID i3ds::Sensor::STATUS = status_Endpoint;
const EndpointID i3ds::Sensor::CONFIGURATION = configuration_Endpoint;
const EndpointID i3ds::Sensor::MEASUREMENT = measurement_Endpoint;

i3ds::Sensor::Sensor(Context::Ptr context, SensorID id)
  : Server(context, id)
{
  state_ = inactive;
  rate_ = 0.0;
}

i3ds::Sensor::~Sensor()
{
}

void i3ds::Sensor::default_command_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Sensor::execute_sensor_command, this, _1);

  set_service<CommandService>(COMMAND, op);
}

void i3ds::Sensor::default_status_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Sensor::get_sensor_status, this, _1);

  set_service<StatusService>(STATUS, op);
}

void i3ds::Sensor::default_configuration_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Sensor::get_sensor_configuration, this, _1);

  set_service<ConfigurationService>(CONFIGURATION, op);
}

void
i3ds::Sensor::execute_sensor_command(CommandService::Data& command)
{
  switch(command.request.kind)
    {
    case SensorCommand::command_PRESENT:
      command.response.result = execute_state_command(command.request.u.command);
      break;

    case SensorCommand::set_rate_PRESENT:
      command.response.result = execute_rate_command(command.request.u.set_rate);
      break;

    default:
      command.response.result = error_unsupported_command;
      break;
    }
}

CommandResult
i3ds::Sensor::execute_state_command(StateCommand command)
{
  switch(state_)
    {
    case inactive:
      if (command == activate)
	{
	  do_activate();
	  state_ = standby;
	  return success;
	}
      break;

    case standby:
      if (command == deactivate)
	{
	  do_deactivate();
	  state_ = inactive;
	  return success;
	}
      else if (command == start)
	{
	  do_start();
	  state_ = operational;
	  return success;
	}
      break;

    case operational:
      if (command == stop)
	{
	  do_stop();
	  state_ = standby;
	  return success;
	}
      break;

    default:
      break;
    }

  return error_illegal_state;
}

CommandResult
i3ds::Sensor::execute_rate_command(SensorRate rate)
{
  if (state_ != standby)
    {
      return error_illegal_state;
    }

  if (!support_rate(rate))
    {
      return error_unsupported_value;
    }

  rate_ = rate;

  return success;
}

void
i3ds::Sensor::get_sensor_status(StatusService::Data& status) const
{
  status.response.sensor_id = sensor();
  status.response.sensor_state = state();
  status.response.sensor_temperature.kelvin = temperature();
}

void
i3ds::Sensor::get_sensor_configuration(ConfigurationService::Data& config) const
{
  config.response.sensor_state = state();
  config.response.config_rate = rate();
  config.response.config_count = 0; // FIXME
}
