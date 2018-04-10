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

i3ds::Server::Server(Context::Ptr context, NodeID node)
  : Receiver(context), node_(node)
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

i3ds::Socket::Ptr
i3ds::Server::Create(Context& context)
{
  int port = 8000 + (node_ & 0xFF);

  Socket::Ptr socket = context.Server();

  socket->Bind("tcp://*:" + std::to_string(port));

  return socket;
}

void
i3ds::Server::Handle(Message& message, Socket& socket) noexcept
{
  Message response;
  CommandResponseCodec::Data error;

  if (message.node() != node_)
    {
       response.set_address(Address(node_, 0));
       set_response(error, error_node_id, "Wrong node ID: " + std::to_string(message.node()));
       Encode<CommandResponseCodec>(response, error);
    }
  else if (handlers_.count(message.endpoint()) == 0)
    {
       response.set_address(Address(node_, 0));
       set_response(error, error_endpoint_id, "Unknown endpoint ID: " + std::to_string(message.endpoint()));
       Encode<CommandResponseCodec>(response, error);
    }
  else
    {
      try
	{
	  handlers_[message.endpoint()]->Handle(message, response);
	  response.set_address(Address(node_, message.endpoint()));
	}
      catch(CommandException e)
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
i3ds::set_response(CommandResponse& response, const CommandException& e)
{
  response.result = e.result();
  set_string(response.message, e.what());
}
