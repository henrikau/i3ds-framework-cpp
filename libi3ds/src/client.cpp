///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "i3ds/client.hpp"

i3ds::Client::Client(Context::Ptr context, NodeID node)
  : node_(node),
    context_(context),
    timeout_(-1)
{
}

i3ds::Client::~Client()
{
}

void
i3ds::Client::Reset()
{
  socket_ = Socket::Client(context_);
  socket_->Attach(node_);
}

void
i3ds::Client::Send(EndpointID endpoint, Message& request)
{
  if (!socket_ || pending_)
    {
      Reset();
    }

  request.set_address(Address(node_, endpoint));

  socket_->Send(request);

  pending_ = true;
}

void
i3ds::Client::Receive(EndpointID endpoint, Message& response)
{
  if (!socket_ || !pending_)
    {
      throw CommunicationError("No pending request to receive!");
    }

  socket_->Receive(response, timeout_);

  pending_ = false;

  Address a = response.address();

  if (a.node != node_)
    {
      throw CommunicationError("Response has wrong node ID: " + std::to_string(a.node));
    }

  if (a.endpoint != endpoint)
    {
      throw CommunicationError("Response has wrong endpoint ID: " + std::to_string(a.endpoint));
    }
}

void
i3ds::Client::Stop()
{
  socket_.reset();
}
