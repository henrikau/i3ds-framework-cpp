///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_ANALOG_HPP
#define __I3DS_EMULATED_ANALOG_HPP

#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/analog_sensor.hpp>

#include <vector>
#include <random>

namespace i3ds
{

class EmulatedAnalog : public Analog
{
public:

  struct Parameters
  {
    int series_count;
    int bit_resolution;
    float scale;
    float offset;
    float smooth;
  };

  typedef std::shared_ptr<EmulatedAnalog> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id, Parameters param)
  {
    return std::make_shared<EmulatedAnalog>(context, id, param);
  }

  EmulatedAnalog(Context::Ptr context, NodeID id, Parameters param);
  virtual ~EmulatedAnalog();

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  // Configuration parameters for analog sensor.
  const Parameters param_;

  // Called periodically to accumulated and send samples.
  bool send_sample(unsigned long timestamp_us);

  // Generat a single ADC reading with specified resolution.
  std::vector<float> read_adc();

  // Periodic sample tread calling send_sample.
  Sampler sampler_;

  Publisher publisher_;

  MeasurementTopic::Data frame_;

  // Number of batches inserted.
  BatchCount batches_;

  // Last value used for smoothing.
  std::vector<uint32_t> last_;

  // Random number generation.
  std::default_random_engine generator_;
  std::uniform_int_distribution<uint32_t> distribution_;
};

} // namespace i3ds

#endif
