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
  CommandResult r;

  switch(command.kind)
    {
    case SensorCommand::command_PRESENT:
      r = handle_state_command(command.u.command);
      break;

    case SensorCommand::set_rate_PRESENT:
      r = handle_rate_command(command.u.set_rate);
      break;

    default:
      r = error_other;
    }

  return r;
}

CommandResult
i3ds::Sensor::handle_state_command(StateCommand command)
{
  CommandResult r;

  switch(_state)
    {
    case inactive:
      if (command == activate)
	{
	  do_activate();
	  _state = standby;
	  r = success;
	}
      else
	{
	  r = error_illegal_state;
	}
      break;

    case standby:
      if (command == deactivate)
	{
	  do_deactivate();
	  _state = inactive;
	  r = success;
	}
      else if (command == start)
	{
	  do_start();
	  _state = operational;
	  r = success;
	}
      else
	{
	  r = error_illegal_state;
	}
      break;

    case operational:
      if (command == stop)
	{
	  do_stop();
	  _state = standby;
	  r = success;
	}
      else
	{
	  r = error_illegal_state;
	}
      break;

    default:
      r = error_illegal_state;
    }

  return r;
}

CommandResult
i3ds::Sensor::handle_rate_command(SensorRate rate)
{
  CommandResult r;
  int errCode;

  if (_state == standby)
    {
      if (SensorRate_IsConstraintValid(&rate, &errCode) && update_rate(rate))
	{
	  r = success;
	  _rate = rate;
	}
      else
	{
	  r = error_unsupported_value;
	}
    }
  else
    {
      r = error_illegal_state;
    }

  return r;
}
