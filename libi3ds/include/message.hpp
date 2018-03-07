///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESSAGE_HPP
#define __MESSAGE_HPP

#include "SensorSuite.h"
#include <cstdlib>

namespace i3ds
{

struct Message
{
  SensorID sensor_id;
  EndpointID endpoint_id;
  byte* data;
  size_t size;
};

} // namespace i3ds

#endif
