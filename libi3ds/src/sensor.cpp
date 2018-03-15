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
#include "service_handler.hpp"

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
  handlers_.clear();
}

void i3ds::Sensor::default_command_handler()
{
  using std::placeholders::_1;
  using std::placeholders::_2;

  auto op = std::bind(&i3ds::Sensor::execute_sensor_command, this, _1, _2);

  set_handler(COMMAND, ServiceHandler<SensorCommandCodec, SensorCommandResponseCodec>::Create(op));
}

void i3ds::Sensor::default_status_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Sensor::get_sensor_status, this, _1);

  set_handler(STATUS, ServiceHandler<NullCodec, SensorStatusCodec>::Create(op));
}

void i3ds::Sensor::default_configuration_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Sensor::get_sensor_status, this, _1);

  set_handler(STATUS, ServiceHandler<NullCodec, SensorStatusCodec>::Create(op));
}

void i3ds::Sensor::set_handler(EndpointID id, Handler::Ptr handler)
{
  handlers_[id] = std::move(handler);
}

i3ds::Handler&
i3ds::Sensor::get_handler(EndpointID id) const
{
  auto it = handlers_.find(id);

  if (it == handlers_.end())
    {
      throw std::runtime_error("No handler for ID " + std::to_string(id));
    }

  return *it->second;
}

void
i3ds::Sensor::execute_sensor_command(const SensorCommand& command, SensorCommandResponse& response)
{
  switch(command.kind)
    {
    case SensorCommand::command_PRESENT:
      response.result = execute_state_command(command.u.command);
      break;

    case SensorCommand::set_rate_PRESENT:
      response.result = execute_rate_command(command.u.set_rate);
      break;

    default:
      response.result = error_unsupported_command;
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

  Handler& handler = get_handler(request.endpoint_id);

  handler.handle(request, response);
}
