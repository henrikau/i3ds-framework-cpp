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

#include <i3ds/client.hpp>
#include <i3ds/sensor.hpp>

namespace i3ds
{

class SensorClient : public Client
{
public:

  SensorClient(Context::Ptr context, NodeID sensor);

  void set_state(StateCommand state);
  void set_sampling(SamplePeriod period, BatchSize batch_size = 1, BatchCount batch_count = 0);

  SensorState state() const {return status_.response.state;}
  Temperature temperature() const {return status_.response.temperature;}
  SamplePeriod period() const {return config_.response.period;}
  BatchSize batch_size() const {return config_.response.batch_size;}
  BatchCount batch_count() const {return config_.response.batch_count;}

  void load_status();
  virtual void load_config();
  virtual void load_all();

private:

  Sensor::StatusService::Data status_;
  Sensor::ConfigurationService::Data config_;
};

} // namespace i3ds

#endif
