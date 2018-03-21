///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SENSOR_HPP
#define __I3DS_SENSOR_HPP

#include "SensorSuite.h"

#include "communication.hpp"
#include "service.hpp"
#include "server.hpp"
#include "codec.hpp"

namespace i3ds
{

CODEC(SensorStatus);
CODEC(SensorCommand);
CODEC(SensorConfiguration);
CODEC(SensorCommandResponse);

class Sensor : public Server
{
public:

  static const EndpointID COMMAND;
  static const EndpointID STATUS;
  static const EndpointID CONFIGURATION;
  static const EndpointID MEASUREMENT;

  typedef Service<NullCodec, SensorStatusCodec> StatusService;
  typedef Service<SensorCommandCodec, SensorCommandResponseCodec> CommandService;
  typedef Service<NullCodec, SensorConfigurationCodec> ConfigurationService;

  Sensor(Context& context, SensorID id);
  virtual ~Sensor();

  // Get sensor state.
  SensorState state() const {return state_;}

  // Get sensor rate.
  SensorRate rate() const {return rate_;}

  // Get temperature in Kelvin (defaults to 0.0).
  virtual double temperature() const {return 0.0;}

protected:

  // Set default handler for endpoints.
  void default_command_handler();
  void default_status_handler();
  void default_configuration_handler();

  // Get sensor status.
  void get_sensor_status(StatusService::Data& status) const;

  // Get sensor status.
  void get_sensor_configuration(ConfigurationService::Data& config) const;

  // Execute SensorCommand.
  void execute_sensor_command(CommandService::Data& command);

  // Sensor action when activated.
  virtual void do_activate() = 0;

  // Sensor action when started.
  virtual void do_start() = 0;

  // Sensor action when activated.
  virtual void do_stop() = 0;

  // Sensor action when activated.
  virtual void do_deactivate() = 0;

  // Check if rate is supported.
  virtual bool support_rate(SensorRate rate) = 0;

private:

  CommandResult execute_state_command(StateCommand command);
  CommandResult execute_rate_command(SensorRate rate);

  SensorState state_;
  SensorRate rate_;
};

} // namespace i3ds

#endif
