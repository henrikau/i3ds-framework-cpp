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

i3ds::Address::Address(std::string address)
{
  if (address.size() != 8)
    {
      throw std::runtime_error("Hex address must be 8 bytes is " + address);
    }

  char buffer[9];

  snprintf(buffer, sizeof(buffer), "%s", address.c_str());

  unsigned int a = strtoul(buffer, NULL, 16);

  this->sensor = (a >> 8) & 0xFFFFFF;
  this->endpoint = a & 0xFF;  
}

std::string
i3ds::Address::to_string() const
{
  unsigned int a = (sensor << 8) + endpoint;
  
  char buffer[9];

  snprintf(buffer, sizeof(buffer), "%08x", a);
  
  return std::string(buffer, 8);
}

i3ds::Address
i3ds::Message::address() const
{
  if (header_.size() != 8)
    {
      throw std::runtime_error("Header must be 8 bytes is " + std::to_string(header_.size()));
    }

  return Address(std::string((const char* ) header_.data(), header_.size()));
}

void
i3ds::Message::set_address(Address address)
{
  std::string a = address.to_string();
  
  header_ = zmq::message_t(a.c_str(), a.size());
}

void
i3ds::Message::set_payload(byte* data, size_t size)
{
  payload_ = zmq::message_t(data, size);
}

i3ds::Socket::Socket(zmq::socket_t socket)
  : socket_(std::move(socket))
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
  if (message.header_.size() == 0)
    {
      std::runtime_error("Cannot send, missing message header");
    }

  socket_.send(message.header_, ZMQ_SNDMORE);
  socket_.send(message.payload_);
}

bool
i3ds::Socket::Receive(Message& message, int timeout_ms)
{
  socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));
  
  if (!socket_.recv(&message.header_))
    {
      return false;
    }

  if (!message.header_.size() == 0)
    {
      std::runtime_error("Received message with empty header frame");
    }

  if (!message.header_.more())
    {
      std::runtime_error("Received message without payload frame");
    }

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
