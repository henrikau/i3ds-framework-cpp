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

#include "Common.h"
#include "Sensor.h"

#include "communication.hpp"
#include "service.hpp"
#include "server.hpp"
#include "codec.hpp"
#include "exception.hpp"

namespace i3ds
{

CODEC(StateCommand);
CODEC(SampleCommand);
CODEC(SensorStatus);
CODEC(SensorConfiguration);

class Sensor : public Server
{
public:

  // Service definitions for basic sensor.
  typedef Command<1, StateCommandCodec>        StateService;
  typedef Command<2, SampleCommandCodec>       SampleService;
  typedef Query  <3, SensorStatusCodec>        StatusService;
  typedef Query  <4, SensorConfigurationCodec> ConfigurationService;

  Sensor(Context::Ptr context, NodeID id);
  virtual ~Sensor();

  // Throws exception if sensor is not in inactive state.
  void check_inactive() const;

  // Throws exception if sensor is not in active state.
  void check_active() const;

  // Throws exception if sensor is not in standby state.
  void check_standby() const;

  // Throws exception if sensor is not in operational state.
  void check_operational() const;

  // Throws exception if sensor is not in failure state.
  void check_failure() const;

  // Returns true if sensor is in inactive state.
  inline bool is_inactive() const {return state() == inactive;}

  // Returns true if sensor is in active state.
  inline bool is_active() const {return state() == standby || state() == operational;}

  // Returns true if sensor is in standby state.
  inline bool is_standby() const {return state() == standby;}

  // Returns true if sensor is in operational state.
  inline bool is_operational() const {return state() == operational;}

  // Returns true if sensor is in failure state.
  inline bool is_failure() const {return state() == failure;}

  // Get sensor state.
  inline SensorState state() const {return state_;}

  // Get sensor rate in seconds between samples.
  inline SampleRate rate() const {return rate_;}

  // Get temperature in Kelvin (defaults to 0.0).
  virtual double temperature() const {return 0.0;}

  // Sensor action when activated.
  virtual void do_activate() = 0;

  // Sensor action when started.
  virtual void do_start() = 0;

  // Sensor action when activated.
  virtual void do_stop() = 0;

  // Sensor action when activated.
  virtual void do_deactivate() = 0;

  // Check if rate is supported.
  virtual bool support_rate(SampleRate rate) = 0;

private:

  // Handler for state command.
  void handle_state(StateService::Data& command);

  // Handler for sample configuration.
  void handle_sample(SampleService::Data& sample);

  // Handler for sensor status query.
  void handle_status(StatusService::Data& status) const;

  // Handler for sensor configuration query.
  void handle_configuration(ConfigurationService::Data& config) const;

  SensorState state_;
  SampleRate rate_;
};

} // namespace i3ds

#endif
