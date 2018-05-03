///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/client_factory.hpp>

i3ds::ClientFactory::Ptr
i3ds::ClientFactory::Create(Context::Ptr context)
{
  return std::make_shared<ClientFactory>(context);
}

i3ds::ClientFactory::ClientFactory(Context::Ptr context)
  : context_(context)
{
}

i3ds::ClientFactory::~ClientFactory()
{
}

i3ds::ToFCameraClient::Ptr
i3ds::ClientFactory::CreateToFCamera(NodeID node)
{
  return std::make_shared<ToFCameraClient>(context_, node);
}

i3ds::CameraClient::Ptr
i3ds::ClientFactory::CreateCamera(NodeID node)
{
  return std::make_shared<CameraClient>(context_, node);
}
