///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POSE_ESTIMATOR_CLIENT_HPP
#define __I3DS_POSE_ESTIMATOR_CLIENT_HPP

#include <i3ds/sensor_client.hpp>

namespace i3ds
{

class PoseEstimatorClient : public SensorClient
{
public:

  typedef std::shared_ptr<PoseEstimatorClient> Ptr;
  static Ptr Create(Context::Ptr context, NodeID id)
  {
    return std::make_shared<PoseEstimatorClient>(context, id);
  }

  PoseEstimatorClient(Context::Ptr context, NodeID sensor);
};

} // namespace i3ds

#endif
