///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_ANALOG_CLIENT_HPP
#define __I3DS_ANALOG_CLIENT_HPP

#include <i3ds/sensor_client.hpp>
#include <i3ds/analog_sensor.hpp>

namespace i3ds
{

class AnalogClient : public SensorClient
{
public:

  typedef std::shared_ptr<AnalogClient> Ptr;
  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<AnalogClient>(context, id);
  }

  AnalogClient(Context::Ptr context, NodeID sensor);

};

} // namespace i3ds

#endif
