#include "sensor.hpp"

i3ds::Sensor::Sensor(SensorID id)
{
  _id = id;
  _state = inactive;
  _rate = 0.0;
}

i3ds::Sensor::~Sensor()
{
}

CommandResult
i3ds::Sensor::handle_sensor_command(SensorCommand& command)
{
  switch(command.kind)
    {
    case SensorCommand::command_PRESENT:
      return handle_state_command(command.u.command);

    case SensorCommand::set_rate_PRESENT:
      return handle_rate_command(command.u.set_rate);

    default:
      break;
    }

  return error_unsupported_command;
}

CommandResult
i3ds::Sensor::handle_state_command(StateCommand command)
{
  switch(_state)
    {
    case inactive:
      if (command == activate)
	{
	  do_activate();
	  _state = standby;
	  return success;
	}
      break;

    case standby:
      if (command == deactivate)
	{
	  do_deactivate();
	  _state = inactive;
	  return success;
	}
      else if (command == start)
	{
	  do_start();
	  _state = operational;
	  return success;
	}
      break;

    case operational:
      if (command == stop)
	{
	  do_stop();
	  _state = standby;
	  return success;
	}
      break;

    default:
      break;
    }

  return error_illegal_state;
}

CommandResult
i3ds::Sensor::handle_rate_command(SensorRate rate)
{
  int errCode;

  if (_state != standby)
    {
      return error_illegal_state;
    }

  if (!SensorRate_IsConstraintValid(&rate, &errCode))
    {
      return error_unsupported_value;
    }

  if (!support_rate(rate))
    {
      return error_unsupported_value;
    }

  _rate = rate;

  return success;
}
