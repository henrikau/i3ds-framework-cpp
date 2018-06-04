///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_IMU_HPP
#define __I3DS_EMULATED_IMU_HPP

#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/imu_sensor.hpp>

namespace i3ds
{

class EmulatedIMU : public IMU
{
public:

  typedef std::shared_ptr<EmulatedIMU> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<EmulatedIMU>(context, id);
  }

  EmulatedIMU(Context::Ptr context, NodeID id);
  virtual ~EmulatedIMU();

  // Supported period.
  virtual bool is_period_supported(SamplePeriod period);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  bool send_sample(unsigned long timestamp_us);

  Sampler sampler_;

  Publisher publisher_;
  IMUMeasurement20 frame_;
};

} // namespace i3ds

#endif
