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
CODEC(SensorResponse);

class Sensor : public Server
{
public:

  // Endpoint definitions for sensors.
  static const EndpointID COMMAND;
  static const EndpointID STATUS;
  static const EndpointID CONFIGURATION;
  static const EndpointID MEASUREMENT;

  // Service definitions for basic sensor.
  typedef Service<NullCodec, SensorStatusCodec> StatusService;
  typedef Service<SensorCommandCodec, SensorResponseCodec> CommandService;
  typedef Service<NullCodec, SensorConfigurationCodec> ConfigurationService;

  Sensor(Context::Ptr context, NodeID id);
  virtual ~Sensor();

  // Get sensor state.
  SensorState state() const {return state_;}

  // Get sensor rate in seconds between samples.
  SensorRate rate() const {return rate_;}

  // Get temperature in Kelvin (defaults to 0.0).
  virtual double temperature() const {return 0.0;}

protected:

  // Set default handler for status query.
  void default_status_handler();

  // Set default handler for configuration query.
  void default_configuration_handler();

  // Set default handler for command.
  void default_command_handler();

  // Execute sensor command.
  ResultCode execute_sensor_command(SensorCommand& command);

  // Get sensor configuration.
  void get_sensor_configuration(SensorConfiguration& config) const;

  // Get sensor status.
  void get_sensor_status(SensorStatus& status) const;

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

  // Default handler for sensor status query.
  void handle_status_query(StatusService::Data& status) const;

  // Default handler for sensor configuration query.
  void handle_configuration_query(ConfigurationService::Data& config) const;

  // Default handler for sensor command.
  void handle_command(CommandService::Data& command);

  // Execute state command according to sensor FSM pattern.
  ResultCode execute_state_command(StateCommand command);

  // Execute rate command checking if rate is supported.
  ResultCode execute_rate_command(SensorRate rate);

  SensorState state_;
  SensorRate rate_;
};

} // namespace i3ds

#endif
