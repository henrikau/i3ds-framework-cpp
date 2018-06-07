///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_ANALOG_HPP
#define __I3DS_ANALOG_HPP

#include <i3ds/Analog.h>

#include <i3ds/topic.hpp>
#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>

namespace i3ds
{

CODEC(AnalogMeasurement300);
CODEC(AnalogMeasurement1K);
CODEC(AnalogMeasurement3K);
CODEC(AnalogMeasurement10K);

class Analog : public Sensor
{
public:

  typedef Topic<128, AnalogMeasurement300Codec> Measurement300Topic;
  typedef Topic<129, AnalogMeasurement1KCodec> Measurement1KTopic;
  typedef Topic<130, AnalogMeasurement3KCodec> Measurement3KTopic;
  typedef Topic<131, AnalogMeasurement10KCodec> Measurement10KTopic;

  // Shared pointer for Analog interfaces.
  typedef std::shared_ptr<Analog> Ptr;

  // Constructor and destructor.
  Analog(NodeID node, SeriesCount series_count) : Sensor(node), series_count_(series_count) {};
  virtual ~Analog() {};

protected:

  // The number of series per measurement (number of channels)
  SeriesCount series_count_;

};

} // namespace i3ds

#endif
