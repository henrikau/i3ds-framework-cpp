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

#include "StarTracker.h"

#include "sensor.hpp"
#include "service.hpp"
#include "codec.hpp"

namespace i3ds
{

CODEC(StarTrackerMeasurement);

class StarTracker : public Sensor
{
public:

  // Constructor and destructor.
  StarTracker(NodeID node) : Sensor(node) {};
  virtual ~StarTracker() {};

};

} // namespace i3ds

#endif
