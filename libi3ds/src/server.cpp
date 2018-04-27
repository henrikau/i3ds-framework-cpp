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

#include "i3ds/server.hpp"

i3ds::Server::Server(Context::Ptr context)
  : Receiver(context)
{
}

i3ds::Server::~Server()
{
  handlers_.clear();
}

void
i3ds::Server::attach_handler(Address address, Handler::Ptr handler)
{
  handlers_[address] = std::move(handler);
}

void
i3ds::Server::detach_handler(Address address)
{
  handlers_.erase(address);
}

i3ds::Socket::Ptr
i3ds::Server::Create()
{
  Socket::Ptr socket = Socket::Server(context_);

  for (auto it = handlers_.cbegin(); it != handlers_.cend(); ++it)
    {
      socket->Attach(it->first.node);
    }

  return socket;
}

void
i3ds::Server::Handle(Message& message, Socket& socket)
{
  Message response;
  CommandResponseCodec::Data error;

  if (handlers_.count(message.address()) == 0)
    {
      response.set_address(Address(message.node(), 0));
      set_response(error, error_endpoint_id, "Unknown address: " + message.address().to_string());
      Encode<CommandResponseCodec>(response, error);
    }
  else
    {
      response.set_address(message.address());

      try
        {
          handlers_[message.address()]->Handle(message, response);
        }
      catch(CommandError e)
        {
          set_response(error, e);
          Encode<CommandResponseCodec>(response, error);
        }
      catch(std::exception e)
        {
          set_response(error, error_other, e.what());
          Encode<CommandResponseCodec>(response, error);
        }
    }

  socket.Send(response);
}

void
i3ds::set_response(CommandResponse& response, ResultCode result, std::string message)
{
  response.result = result;
  set_string(response.message, message);
}

void
i3ds::set_response(CommandResponse& response, const CommandError& e)
{
  response.result = e.result();
  set_string(response.message, e.what());
}
