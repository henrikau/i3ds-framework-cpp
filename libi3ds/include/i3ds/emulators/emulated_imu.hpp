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

#include "i3ds/core/topic.hpp"
#include "i3ds/core/publisher.hpp"
#include "i3ds/sensors/imu.hpp"
#include "i3ds/emulators/periodic_sampler.hpp"

namespace i3ds
{

class EmulatedIMU : public IMU
{
public:

  typedef Topic<128, IMUMeasurementCodec> EmulatedIMUMeasurement;

  EmulatedIMU(Context::Ptr context, NodeID id);
  virtual ~EmulatedIMU();

  // Supported rate.
  virtual bool is_rate_supported(SampleRate rate);

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
  IMUMeasurement frame_;
};

} // namespace i3ds

#endif
