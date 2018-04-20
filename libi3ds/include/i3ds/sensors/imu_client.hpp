///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_IMU_CLIENT_HPP
#define __I3DS_IMU_CLIENT_HPP

#include "i3ds/sensors/sensor_client.hpp"
#include "i3ds/sensors/imu.hpp"

namespace i3ds
{

class IMUClient : public SensorClient
{
public:

  IMUClient(Context::Ptr context, NodeID sensor);

};

} // namespace i3ds

#endif
