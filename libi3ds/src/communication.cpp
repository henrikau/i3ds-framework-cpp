///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "communication.hpp"

#include <iostream>

void i3ds_message_free(void* data, void* hint)
{
  free(data);
}

i3ds::Address::Address(std::string address)
{
  if (address.size() != 8)
    {
      throw std::runtime_error("Hex address must be 8 bytes is " + address);
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

i3ds::Socket::Socket(zmq::socket_t socket)
  : socket_(std::move(socket))
{
}

i3ds::Socket::~Socket()
{
}

void
i3ds::Socket::Bind(std::string address)
{
  socket_.bind(address);
}

void
i3ds::Socket::Connect(std::string address)
{
  socket_.connect(address);
}

void
i3ds::Socket::Send(Message& message)
{
  const std::string a = message.address().to_string();
  zmq::message_t header(a.c_str(), a.size());

  socket_.send(header, ZMQ_SNDMORE);
  socket_.send(message.payload_);
}

bool
i3ds::Socket::Receive(Message& message, int timeout_ms)
{
  socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));

  zmq::message_t header;

  if (!socket_.recv(&header))
    {
      return false;
    }

  if (header.size() != 8)
    {
      throw std::runtime_error("Header must be 8 bytes is " + std::to_string(header.size()));
    }

  if (!header.more())
    {
      throw std::runtime_error("Received message without payload frame");
    }

  message.set_address(Address(std::string(header.data<char>(), header.size())));

  socket_.recv(&message.payload_);

  return true;
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

i3ds::Context::Context()
  : context_(1)
{
}

i3ds::Socket::Ptr
i3ds::Context::CreateSocket(int type)
{
  return Socket::Ptr(new Socket(zmq::socket_t(context_, type)));
}
