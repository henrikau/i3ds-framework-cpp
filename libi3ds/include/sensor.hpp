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
#include "message.hpp"
#include "codec.hpp"

namespace i3ds
{

CODEC(SensorStatus);
CODEC(SensorCommand);
CODEC(SensorConfiguration);
CODEC(SensorCommandResponse);

class Sensor
{
public:

  static const EndpointID COMMAND;
  static const EndpointID STATUS;
  static const EndpointID CONFIGURATION;
  static const EndpointID MEASUREMENT;

  Sensor(SensorID id);
  virtual ~Sensor();

  // Get sensor ID.
  SensorID get_id() const {return id_;}

  // Get sensor state.
  SensorState get_state() const {return state_;}

  // Get sensor rate.
  SensorRate get_rate() const {return rate_;}

  // Get temperature in Kelvin (defaults to 0.0).
  virtual double get_temperature() const {return 0.0;}

  // Callback for handling sensor request message.
  virtual Message handle_request(const Message& message);

protected:

  // Get sensor status.
  void get_sensor_status(SensorStatus& status) const;

  // Get sensor status.
  void get_sensor_configuration(SensorConfiguration& config) const;

  // Execute SensorCommand.
  CommandResult execute_sensor_command(const SensorCommand& command);

  // Callback for handling command message.
  virtual void handle_command(const Message& request, Message& response);

  // Callback for handling status query message.
  virtual void handle_status_query(const Message& request, Message& response);

  // Callback for handling configuration query message.
  virtual void handle_configuration_query(const Message& request, Message& response);

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

  SensorID id_;
  SensorState state_;
  SensorRate rate_;

  Encoder<SensorCommandResponseCodec> command_response_;
  Encoder<SensorConfigurationCodec> configuration_;
  Encoder<SensorStatusCodec> status_;
};

} // namespace i3ds

#endif
