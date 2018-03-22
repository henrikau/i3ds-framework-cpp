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

i3ds::Subscriber::Subscriber(Context& context, SensorID sensor)
  : Receiver(context), sensor_(sensor)
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

  if (socket_)
    {
      socket_->Filter(Address(sensor_, endpoint));
    }
}

void
i3ds::Subscriber::delete_handler(EndpointID endpoint)
{
  handlers_.erase(endpoint);
}

void
i3ds::Subscriber::Reset()
{
  int port = 7000 + (sensor_ & 0xFF);

  socket_ = context_.Subscriber();

  socket_->Connect("tcp://*:" + std::to_string(port));

  for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
    {
      socket_->Filter(Address(sensor_, it->first));
    }
}

bool
i3ds::Subscriber::ReceiveOne(int timeout_ms)
{
  Message message;

  if (!socket_)
    {
      Reset();
    }

  if (!socket_->Receive(message, timeout_ms))
    {
      return false;
    }

  if (message.sensor() == sensor_ && handlers_.count(message.endpoint()) > 0)
    {
      handlers_[message.endpoint()]->Handle(message);
    }

  return true;
}
