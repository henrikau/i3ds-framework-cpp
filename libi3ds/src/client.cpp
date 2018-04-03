///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "client.hpp"

i3ds::Client::Client(Context::Ptr context, NodeID node)
  : node_(node),
    context_(context)
{
}

i3ds::Client::~Client()
{
}

void i3ds::Client::Reset()
{
  int port = 8000 + (node_ & 0xFF);

  socket_ = context_->Client();

  socket_->Connect("tcp://127.0.0.1:" + std::to_string(port));
}

bool
i3ds::Client::Execute(EndpointID endpoint, Message& request, Message& response, int timeout_ms)
{
  if (!socket_)
    {
      Reset();
    }
  
  request.set_address(Address(node_, endpoint));

  socket_->Send(request);

  if (!socket_->Receive(response, timeout_ms))
    {
      socket_.reset();
      return false;
    }

  Address a = response.address();

  if (a.node != node_)
    {
      throw std::runtime_error("Response has wrong node ID: " + std::to_string(a.node));
    }

  if (a.endpoint != endpoint)
    {
      throw std::runtime_error("Response has wrong endpoint ID: " + std::to_string(a.endpoint));
    }

  return true;
}

void 
i3ds::Client::Stop()
{
  socket_.reset();
}
