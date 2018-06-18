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

#include <i3ds/emulated_tactile.hpp>

i3ds::EmulatedTactile::EmulatedTactile(Context::Ptr context, NodeID node)
  : Analog(node, 3),
    sender_(std::bind(&i3ds::EmulatedTactile::send_sample, this, std::placeholders::_1)),
    sampler_(std::bind(&i3ds::EmulatedTactile::read_adc, this, std::placeholders::_1)),
    publisher_(context, node),
    measurement_index_(0),
    hardware_sample_period_(1000)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated tactile sensor with NodeID: " << node;
  MeasurementTopic::Codec::Initialize(frame_);
  frame_.series = series_count_;
  set_device_name("Emulated tactile sensor");
}

i3ds::EmulatedTactile::~EmulatedTactile()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated tactile sensor with NodeID: " << node();
}

void
i3ds::EmulatedTactile::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedTactile::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_start()";
  measurements_per_sample_ = period() / hardware_sample_period_;
  sender_.Start(period());
  sampler_.Start(hardware_sample_period_);
}

void
i3ds::EmulatedTactile::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_stop()";
  sender_.Stop();
  sampler_.Stop();
}

void
i3ds::EmulatedTactile::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedTactile::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " is_period_supported()";
  return (sample.batch_size >= 1 && sample.batch_size <=10000*series_count_) 
         && (0 < sample.period && sample.period <= 10000000);
}

bool
i3ds::EmulatedTactile::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated tactile sensor with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.samples.nCount = measurement_index_ * series_count_;
  frame_.batch_size = measurement_index_;

  publisher_.Send<MeasurementTopic>(frame_);

  measurement_index_ = 0;

  return true;
}

bool
i3ds::EmulatedTactile::read_adc(unsigned long timestamp_us)
{
  if (measurement_index_ < measurements_per_sample_ - series_count_)
    {
      for (unsigned int i = 0; i < series_count_; ++i)
        {
          frame_.samples.arr[measurement_index_++] = 20 * static_cast<double>(i)/series_count_;
        }
    }

  return true;
}
