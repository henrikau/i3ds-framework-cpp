///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_STAR_TRACKER_CLIENT_HPP
#define __I3DS_STAR_TRACKER_CLIENT_HPP

#include <i3ds/sensor_client.hpp>
#include <i3ds/imu_sensor.hpp>

namespace i3ds
{

class StarTrackerClient : public SensorClient
{
public:

  typedef std::shared_ptr<StarTrackerClient> Ptr;

  StarTrackerClient(Context::Ptr context, NodeID sensor);
};

} // namespace i3ds

#endif
