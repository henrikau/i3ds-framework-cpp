///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CLIENT_FACTORY_HPP
#define __I3DS_CLIENT_FACTORY_HPP

#include <i3ds/communication.hpp>

#include <i3ds/sensor_client.hpp>
#include <i3ds/camera_client.hpp>
#include <i3ds/tof_camera_client.hpp>

namespace i3ds
{

class ClientFactory
{
public:

  typedef std::shared_ptr<ClientFactory> Ptr;

  static Ptr Create(Context::Ptr context);

  ClientFactory(Context::Ptr context);
  virtual ~ClientFactory();

  ToFCameraClient::Ptr CreateToFCamera(NodeID node);
  CameraClient::Ptr CreateCamera(NodeID node);

private:

  Context::Ptr context_;
  NodeID next_id_;
};

} // namespace i3ds

#endif
