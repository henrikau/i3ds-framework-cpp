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

i3ds::Sensor::Sensor(SensorID id)
{
  id_ = id;
  state_ = inactive;
  rate_ = 0.0;
}

i3ds::Sensor::~Sensor()
{
}

CommandResult
i3ds::Sensor::execute_sensor_command(const SensorCommand& command)
{
  switch(command.kind)
    {
    case SensorCommand::command_PRESENT:
      return execute_state_command(command.u.command);

    case SensorCommand::set_rate_PRESENT:
      return execute_rate_command(command.u.set_rate);

    default:
      break;
    }

  return error_unsupported_command;
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
i3ds::Sensor::get_sensor_status(SensorStatus& status) const
{
  status.sensor_id = this->get_id();
  status.sensor_state = this->get_state();
  status.sensor_temperature.kelvin = this->get_temperature();
}

void
i3ds::Sensor::get_sensor_configuration(SensorConfiguration& config) const
{
  config.sensor_state = this->get_state();
  config.config_rate = this->get_rate();
  config.config_count = 0; // FIXME
}

void
i3ds::Sensor::handle(const i3ds::Message& request, i3ds::Message& response)
{
  response.data = NULL;
  response.size = 0;
  response.sensor_id = get_id();
  response.endpoint_id = request.endpoint_id;

  if (request.sensor_id != get_id())
    {
      response.sensor_id = 0;
    }
  else if (request.endpoint_id == Sensor::COMMAND)
    {
      handle_command(request, response);
    }
  else if (request.endpoint_id == Sensor::CONFIGURATION)
    {
      handle_configuration_query(request, response);
    }
  else if (request.endpoint_id == Sensor::STATUS)
    {
      handle_status_query(request, response);
    }
  else
    {
      response.endpoint_id = 0;
    }
}

void
i3ds::Sensor::handle_command(const i3ds::Message& request, i3ds::Message& response)
{
  // Create decoder on stack.
  Decoder<SensorCommandCodec> decoder;
  decoder.Decode(request);

  // Execute the sensor command and get result.
  command_response_.data.result = execute_sensor_command(decoder.data);

  // Encode response message.
  command_response_.Encode(response);
}

void
i3ds::Sensor::handle_status_query(const i3ds::Message& request, i3ds::Message& response)
{
  // Status query message shall be empty.
  assert(request.size == 0);

  // Get sensor status.
  get_sensor_status(status_.data);

  // Encode response message.
  status_.Encode(response);
}

void
i3ds::Sensor::handle_configuration_query(const i3ds::Message& request, i3ds::Message& response)
{
  // Status query message shall be empty.
  assert(request.size == 0);

  // Get sensor status.
  get_sensor_configuration(configuration_.data);

  // Encode response message.
  configuration_.Encode(response);
}
