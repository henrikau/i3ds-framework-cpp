///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_STAR_TRACKER_HPP
#define __I3DS_STAR_TRACKER_HPP

#include <i3ds/StarTracker.h>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>

namespace i3ds
{

CODEC(StarTrackerMeasurement);

class StarTracker : public Sensor
{
public:

  typedef Topic<128, StarTrackerMeasurementCodec> MeasurementTopic;

  // Shared pointer for star tracker interfaces.
  typedef std::shared_ptr<StarTracker> Ptr;

  // Constructor and destructor.
  StarTracker(NodeID node) : Sensor(node) {};
  virtual ~StarTracker() {};

};

} // namespace i3ds

#endif
