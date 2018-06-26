///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_TACTILE_HPP
#define __I3DS_EMULATED_TACTILE_HPP

#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/analog_sensor.hpp>

namespace i3ds
{

class EmulatedTactile : public Analog
{
public:

  typedef Analog::Measurement1KTopic MeasurementTopic;

  typedef std::shared_ptr<EmulatedTactile> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<EmulatedTactile>(context, id);
  }

  EmulatedTactile(Context::Ptr context, NodeID id);
  virtual ~EmulatedTactile();

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  // Function called periodically to send recorded measurements
  bool send_sample(unsigned long timestamp_us);

  // Function called to generate one measurement
  bool read_adc(unsigned long timestamp_us);

  // Periodically calls send_sample
  Sampler sender_;

  // Periodically calls read_adc
  Sampler sampler_;

  Publisher publisher_;

  MeasurementTopic::Data frame_;

  // Keeps track of where in the frame to insert new measurements
  unsigned long measurement_index_;

  // Period between samples of the hardware ADC in microseconds
  unsigned long hardware_sample_period_;

  unsigned long measurements_per_sample_;
};

} // namespace i3ds

#endif
