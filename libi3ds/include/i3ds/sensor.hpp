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

#include <memory>

#include <i3ds/Common.h>
#include <i3ds/Sensor.h>

#include <i3ds/communication.hpp>
#include <i3ds/service.hpp>
#include <i3ds/server.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/exception.hpp>

namespace i3ds
{

CODEC(StateCommand);
CODEC(SampleCommand);
CODEC(SensorStatus);
CODEC(SensorConfiguration);

class Sensor
{
public:

  // Shared pointer for sensor interfaces.
  typedef std::shared_ptr<Sensor> Ptr;

  // Service definitions for basic sensor.
  typedef Command<1, StateCommandCodec>        StateService;
  typedef Command<2, SampleCommandCodec>       SampleService;
  typedef Query  <3, SensorStatusCodec>        StatusService;
  typedef Query  <4, SensorConfigurationCodec> ConfigurationService;

  // Constructor.
  Sensor(NodeID node);

  // Virtual destructor.
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

  // Get the node ID.
  inline NodeID node() const {return node_;}

  // Get sensor state.
  inline SensorState state() const {return state_;}

  // Get sensor period in microseconds between samples.
  inline SamplePeriod period() const {return period_;}

  // Get number of samples per batch.
  inline BatchSize batch_size() const {return batch_size_;}

  // Get number of batches to acquire when operational (0 is infinite).
  inline BatchCount batch_count() const {return batch_count_;}

  // Get name of sensor implementation
  virtual T_String device_name() const {return device_name_;}

  // Get temperature in Kelvin (defaults to 0.0).
  virtual double temperature() const {return 0.0;}

  // Throws exception if sample configuration is not supported.
  void check_sampling_supported(SampleCommand sample);

  // Returns true if sample configuration is supported.
  virtual bool is_sampling_supported(SampleCommand sample) = 0;

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Sensor action when activated.
  virtual void do_activate() = 0;

  // Sensor action when started.
  virtual void do_start() = 0;

  // Sensor action when activated.
  virtual void do_stop() = 0;

  // Sensor action when activated.
  virtual void do_deactivate() = 0;

  // Sensor action on failure.
  virtual void do_failure() {};

  // Set sensor in failure state.
  void set_failure();

  // Set the name of the sensor implementation
  void set_device_name(std::string device_name);

private:

  // Handler for state command.
  void handle_state(StateService::Data& command);

  // Handler for sample configuration.
  void handle_sample(SampleService::Data& sample);

  // Handler for sensor status query.
  void handle_status(StatusService::Data& status);

  // Handler for sensor configuration query.
  void handle_configuration(ConfigurationService::Data& config);

  const NodeID node_;

  SensorState state_;
  SamplePeriod period_;
  BatchSize batch_size_;
  BatchCount batch_count_;
  T_String device_name_;
};

} // namespace i3ds

#endif
