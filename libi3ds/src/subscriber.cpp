///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/subscriber.hpp>

i3ds::Subscriber::Subscriber(Context::Ptr context)
  : Receiver(context)
{
}

i3ds::Subscriber::~Subscriber()
{
  handlers_.clear();
}

void
i3ds::Subscriber::attach_handler(Address address, Handler::Ptr handler)
{
  handlers_[address] = std::move(handler);
}

void
i3ds::Subscriber::detach_handler(Address address)
{
  handlers_.erase(address);
}

i3ds::Socket::Ptr
i3ds::Subscriber::Create()
{
  Socket::Ptr socket = Socket::Subscriber(context_);

  for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
    {
      socket->Attach(it->first.node);
      socket->Filter(it->first);
    }

  return socket;
}

void
i3ds::Subscriber::Handle(Message& message, Socket& socket)
{
  try
    {
      if (handlers_.count(message.address()) > 0)
        {
          handlers_[message.address()]->Handle(message);
        }
    }
  catch (std::exception e)
    {
      // TODO: Log error
    }
}
