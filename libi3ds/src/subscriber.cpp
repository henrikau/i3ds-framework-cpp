///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "subscriber.hpp"

i3ds::Subscriber::Subscriber(Context::Ptr context, NodeID node)
  : Receiver(context), node_(node)
{
}

i3ds::Subscriber::~Subscriber()
{
  handlers_.clear();
}

void
i3ds::Subscriber::set_handler(EndpointID endpoint, Handler::Ptr handler)
{
  handlers_[endpoint] = std::move(handler);
}

void
i3ds::Subscriber::delete_handler(EndpointID endpoint)
{
  handlers_.erase(endpoint);
}

i3ds::Socket::Ptr
i3ds::Subscriber::Create(Context& context)
{
  int port = 7000 + (node_ & 0xFF);

  Socket::Ptr socket = context.Subscriber();

  socket->Connect("tcp://*:" + std::to_string(port));

  for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
    {
      socket->Filter(Address(node_, it->first));
    }

  return socket;
}

void
i3ds::Subscriber::Handle(Message& message, Socket& socket) noexcept
{
  try
    {
      if (message.node() == node_ && handlers_.count(message.endpoint()) > 0)
	{
	  handlers_[message.endpoint()]->Handle(message);
	}
    }
  catch (std::exception e)
    {
      // TODO: Log error
    }
}
