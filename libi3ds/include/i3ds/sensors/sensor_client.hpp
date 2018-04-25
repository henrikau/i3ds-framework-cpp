///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SENSOR_CLIENT_HPP
#define __I3DS_SENSOR_CLIENT_HPP

#include "i3ds/core/client.hpp"
#include "i3ds/sensors/sensor.hpp"

namespace i3ds
{

class SensorClient : public Client
{
public:

  SensorClient(Context::Ptr context, NodeID sensor);

  void set_state(StateCommand state);
  void set_rate(SampleRate rate);

  SensorState state() const {return status_.response.state;}
  Temperature temperature() const {return status_.response.temperature;}

  void load_status();
  virtual void load_config() {};
  virtual void load_all();

private:

  Sensor::StatusService::Data status_;
};

} // namespace i3ds

#endif
