///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>

#include <i3ds/emulated_imu.hpp>

i3ds::EmulatedIMU::EmulatedIMU(Context::Ptr context, NodeID node)
  : IMU(node),
    sampler_(std::bind(&i3ds::EmulatedIMU::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated IMU with NodeID: " << node;
  IMUMeasurement20Codec::Initialize(frame_);
  set_device_name("Emulated IMU");
}

i3ds::EmulatedIMU::~EmulatedIMU()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated IMU with NodeID: " << node();
}

void
i3ds::EmulatedIMU::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated IMU with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedIMU::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated IMU with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedIMU::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated IMU with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedIMU::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated IMU with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedIMU::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated IMU with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

bool
i3ds::EmulatedIMU::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated IMU with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.linear_accel.nCount = 1;
  frame_.linear_accel.arr[0].nCount = 3;
  frame_.linear_accel.arr[0].arr[0] = 1.0;
  frame_.linear_accel.arr[0].arr[1] = 2.0;
  frame_.linear_accel.arr[0].arr[2] = 3.0;

  frame_.angular_rate.nCount = 1;
  frame_.angular_rate.arr[0].nCount = 3;
  frame_.angular_rate.arr[0].arr[0] = 4.0;
  frame_.angular_rate.arr[0].arr[1] = 5.0;
  frame_.angular_rate.arr[0].arr[2] = 6.0;

  publisher_.Send<MeasurementTopic>(frame_);

  return true;
}
