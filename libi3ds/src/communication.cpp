///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/communication.hpp>
#include <i3ds/exception.hpp>

#include <stdexcept>

void i3ds_message_free(void* data, void* hint)
{
  free(data);
}

i3ds::Address::Address(std::string address)
{
  if (address.size() != 8)
    {
      throw CommunicationError("Hex address must be 8 bytes is " + address);
    }

  char buffer[9];

  snprintf(buffer, sizeof(buffer), "%s", address.c_str());

  unsigned int a = strtoul(buffer, NULL, 16);

  this->node = (a >> 8) & 0xFFFFFF;
  this->endpoint = a & 0xFF;
}

std::string
i3ds::Address::to_string() const
{
  unsigned int a = (node << 8) + endpoint;

  char buffer[9];

  snprintf(buffer, sizeof(buffer), "%08X", a);

  return std::string(buffer, 8);
}

void
i3ds::Message::set_payload(byte* data, size_t size, bool copy)
{
  payload_ = zmq::message_t(data, size, copy ? NULL : &i3ds_message_free);
}

i3ds::Context::Context(std::string addr_srv_addr)
  : context_(1),
    address_socket_(context_, ZMQ_REQ),
    addr_srv_addr_(addr_srv_addr),
    connected_to_addr_srv(false)
{
}

std::string
i3ds::Context::get_config(NodeID node, int type)
{
  if (!connected_to_addr_srv)
    {
      int timeout_ms = 1000;
      address_socket_.connect(addr_srv_addr_);
      address_socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));
      connected_to_addr_srv = true;
    }

  std::string query = std::to_string(node);

  switch (type)
    {
    case ZMQ_PUB:
      query += ",pub";
      break;
    case ZMQ_SUB:
      query += ",sub";
      break;
    case ZMQ_REP:
      query += ",rep";
      break;
    case ZMQ_REQ:
      query += ",req";
      break;
    default:
      throw std::invalid_argument("type");
    }

  zmq::message_t request (query.length());
  memcpy (request.data (), query.c_str(), query.length());
  address_socket_.send(request);

  zmq::message_t reply;

  if (address_socket_.recv(&reply) < 1)
    {
      throw CommunicationError("Could not connect to address server");
    }

  std::string reply_string = std::string(static_cast<char*>(reply.data()), reply.size());

  if (reply_string == "ADDRESS_NOT_FOUND")
    {
      throw CommunicationError("No address found for node_id, type: " + std::to_string(node) + "," + std::to_string(type));
    }

  return reply_string;
}

i3ds::Socket::Ptr
i3ds::Socket::CreateSocket(i3ds::Context::Ptr context, int type)
{
  return i3ds::Socket::Ptr(new Socket(context, type));
}

i3ds::Socket::Socket(i3ds::Context::Ptr context, int type)
  : type_(type),
    context_(context),
    socket_(context->context_, type)
{
}

i3ds::Socket::~Socket()
{
}

void
i3ds::Socket::Attach(NodeID node)
{
  if (attached_.count(node) > 0)
    {
      return;
    }

  if (node < 256)
    {
      int port;
      switch(type_)
        {
        case ZMQ_PUB:
          port = 7000 + (node & 0xFF);
          socket_.bind("tcp://*:" + std::to_string(port));
          break;
        case ZMQ_SUB:
          port = 7000 + (node & 0xFF);
          socket_.connect("tcp://127.0.0.1:" + std::to_string(port));
          break;
        case ZMQ_REQ:
          port = 8000 + (node & 0xFF);
          socket_.connect("tcp://127.0.0.1:" + std::to_string(port));
          break;
        case ZMQ_REP:
          port = 8000 + (node & 0xFF);
          socket_.bind("tcp://*:" + std::to_string(port));
          break;
        }
    }
  else
    {
      std::string config = context_->get_config(node, type_);
      char operation = config[0];
      std::string address = config.substr(2);
      if (tolower(operation) == 'b')
        {
          socket_.bind(address);
        }
      else if (tolower(operation) == 'c')
        {
          socket_.connect(address);
        }
      else
        {
          throw CommunicationError("Invalid bind/connect field in config file for " + std::to_string(node));
        }
    }
  attached_.insert(node);
}

void
i3ds::Socket::Send(Message& message)
{
  const std::string a = message.address().to_string();
  zmq::message_t header(a.c_str(), a.size());

  socket_.send(header, ZMQ_SNDMORE);
  socket_.send(message.payload_);
}

void
i3ds::Socket::Receive(Message& message, int timeout_ms)
{
  socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));

  zmq::message_t header;

  if (!socket_.recv(&header))
    {
      throw Timeout();
    }

  if (header.size() != 8)
    {
      throw CommunicationError("Header must be 8 bytes is " + std::to_string(header.size()));
    }

  if (!header.more())
    {
      throw CommunicationError("Received message without payload frame");
    }

  message.set_address(Address(std::string(header.data<char>(), header.size())));

  socket_.recv(&message.payload_);
}

void
i3ds::Socket::Filter(Address address)
{
  std::string a = address.to_string();

  socket_.setsockopt(ZMQ_SUBSCRIBE, a.c_str(), a.size());
}

void
i3ds::Socket::FilterAll()
{
  socket_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}
