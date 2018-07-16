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

#include <i3ds/emulated_analog.hpp>

i3ds::EmulatedAnalog::Ptr
i3ds::EmulatedAnalog::CreateTactile(Context::Ptr context, NodeID id)
{
  Parameters param;

  param.series = 3;
  param.bit_resolution = 12;
  param.smooth = 0.5;

  for (int i = 0; i < param.series; i++)
    {
      param.scale.push_back(20.0 / 4095);
      param.offset.push_back(0.0);
    }

  return std::make_shared<EmulatedAnalog>(context, id, param);
}

i3ds::EmulatedAnalog::Ptr
i3ds::EmulatedAnalog::CreateForceTorque(Context::Ptr context, NodeID id)
{
  Parameters param;

  param.series = 6;
  param.bit_resolution = 12;
  param.smooth = 0.5;

  // Force is the three first
  for (int i = 0; i < param.series / 2; i++)
    {
      param.offset.push_back(-150.0);
      param.scale.push_back(300.0 / 4095);
    }

  // Torque is the three last
  for (int i = 0; i < param.series / 2; i++)
    {
      param.offset.push_back(-10.0);
      param.scale.push_back(20.0 / 4095);
    }

  return std::make_shared<EmulatedAnalog>(context, id, param);
}

i3ds::EmulatedAnalog::EmulatedAnalog(Context::Ptr context, NodeID node, const Parameters& param)
  : Analog(node, param.series),
    param_(param),
    sampler_(std::bind(&i3ds::EmulatedAnalog::send_sample, this, std::placeholders::_1)),
    publisher_(context, node),
    batches_(0),
    last_(param.series),
    generator_(),
    distribution_(0, (1 << param.bit_resolution) - 1)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated tactile sensor with NodeID: " << node;

  MeasurementTopic::Codec::Initialize(frame_);

  set_device_name("Emulated tactile sensor");

  for (int i = 0; i < param_.series; i++)
    {
      last_[i] = 0;
    }
}

i3ds::EmulatedAnalog::~EmulatedAnalog()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated tactile sensor with NodeID: " << node();
}

void
i3ds::EmulatedAnalog::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedAnalog::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_start()";
  batches_ = 0;
  sampler_.Start(period() / batch_size());
}

void
i3ds::EmulatedAnalog::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedAnalog::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedAnalog::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated tactile sensor with NodeID: " << node() << " is_period_supported()";
  return (sample.batch_size >= 1 && sample.batch_size <=10000*series_count())
         && (0 < sample.period && sample.period <= 10000000);
}

bool
i3ds::EmulatedAnalog::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated analog sensor with NodeID: " << node() << " samples " << timestamp_us;

  std::vector<float> value = read_adc();

  batches_++;

  const int samples = batches_ * param_.series;

  for (int i = 0; i < param_.series; i++)
    {
      frame_.samples.arr[samples + i] = value[i];
    }

  if (batches_ >= batch_size())
    {
      BOOST_LOG_TRIVIAL(trace) << "Emulated tactile sensor with NodeID: " << node() << " sends sample at " << timestamp_us;

      frame_.attributes.timestamp = timestamp_us;
      frame_.attributes.validity = sample_valid;

      frame_.samples.nCount = samples;
      frame_.series = param_.series;
      frame_.batch_size = batches_;

      publisher_.Send<MeasurementTopic>(frame_);

      batches_ = 0;
    }

  return true;
}

std::vector<float>
i3ds::EmulatedAnalog::read_adc()
{
  std::vector<float> value(param_.series);

  for (int i = 0; i < param_.series; i++)
    {
      // Generate new discrete value with mix of random and last.
      last_[i] = param_.smooth * last_[i] + (1.0 - param_.smooth) * distribution_(generator_);

      // Generate value from scaling of discrete.
      value[i] = param_.scale[i] * last_[i] + param_.offset[i];
    }

  return value;
}
