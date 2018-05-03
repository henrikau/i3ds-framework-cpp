///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/star_tracker_client.hpp>

i3ds::StarTrackerClient::StarTrackerClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
}
