///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/pose_estimator_client.hpp>

i3ds::PoseEstimatorClient::PoseEstimatorClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
}
