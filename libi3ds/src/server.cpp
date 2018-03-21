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

#include "server.hpp"

i3ds::Server::Server(Context& context, SensorID sensor)
  : sensor_(sensor), context_(context)
{
}

i3ds::Server::~Server()
{
  handlers_.clear();
}

void
i3ds::Server::set_handler(EndpointID endpoint, Handler::Ptr handler)
{
  handlers_[endpoint] = std::move(handler);
}

void
i3ds::Server::delete_handler(EndpointID endpoint)
{
  handlers_.erase(endpoint);
}

void
i3ds::Server::Reset()
{
  int port = 8000 + (sensor_ & 0xFF);

  socket_ = context_.Server();

  socket_->Bind("tcp://*:" + std::to_string(port));
}

void
i3ds::Server::Spin()
{
  try
    {
      while (SpinOnce());
    }
  catch(std::exception e)
    {
      std::cerr << "Terminating server!" << std::endl;
      socket_.reset();
    }
}

bool
i3ds::Server::SpinOnce(int timeout_ms)
{
  Message request, response;

  if (!socket_)
    {
      Reset();
    }

  if (!socket_->Receive(request, timeout_ms))
    {
      return false;
    }

  const Address address = request.address();

  if (address.sensor == sensor_ && handlers_.count(address.endpoint) > 0)
    {
      handlers_[address.endpoint]->Handle(request, response);
      response.set_address(address);
    }
  else
    {
      response.set_address(Address(sensor_, 0));
    }

  socket_->Send(response);

  return true;
}
