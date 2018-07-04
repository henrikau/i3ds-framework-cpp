///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_TIME_HPP
#define __I3DS_TIME_HPP

#include <i3ds/Common.h>
#include <i3ds/Sensor.h>

#include <chrono>

namespace i3ds
{

typedef std::chrono::high_resolution_clock clock;

inline Timepoint to_timestamp(clock::time_point t)
{
  using namespace std::chrono;
  return (Timepoint) duration_cast<microseconds>(t.time_since_epoch()).count();
}

inline Timepoint get_timestamp()
{
  return to_timestamp(clock::now());
}

inline clock::duration to_duration(SamplePeriod period)
{
  return std::chrono::microseconds(period);
}

}

#endif
