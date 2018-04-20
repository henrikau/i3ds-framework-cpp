///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "i3ds/emulators/emulated_imu.hpp"

i3ds::EmulatedIMU::EmulatedIMU(Context::Ptr context, NodeID node)
  : IMU(node),
    sampler_(std::bind(&i3ds::EmulatedIMU::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  IMUMeasurementCodec::Initialize(frame_);
}

i3ds::EmulatedIMU::~EmulatedIMU()
{
}

void
i3ds::EmulatedIMU::do_activate()
{
}

void
i3ds::EmulatedIMU::do_start()
{
  sampler_.Start(rate());
}

void
i3ds::EmulatedIMU::do_stop()
{
  sampler_.Stop();
}

void
i3ds::EmulatedIMU::do_deactivate()
{
}

bool
i3ds::EmulatedIMU::is_rate_supported(SampleRate rate)
{
  return 0 < rate && rate <= 10000000;
}

bool
i3ds::EmulatedIMU::send_sample(unsigned long timestamp_us)
{
  std::cout << "Send: " << timestamp_us << std::endl;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.linear_accel.nCount = 3;
  frame_.linear_accel.arr[0] = 1.0;
  frame_.linear_accel.arr[1] = 2.0;
  frame_.linear_accel.arr[2] = 3.0;
  frame_.angular_rate.nCount = 3;
  frame_.angular_rate.arr[0] = 4.0;
  frame_.angular_rate.arr[1] = 5.0;
  frame_.angular_rate.arr[2] = 6.0;

  publisher_.Send<EmulatedIMUMeasurement>(frame_);

  return true;
}
