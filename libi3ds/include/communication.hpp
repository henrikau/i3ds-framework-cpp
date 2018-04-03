///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_COMMUNICATION_HPP
#define __I3DS_COMMUNICATION_HPP

#include <memory>
#include <string>
#include <zmq.hpp>

#include "SensorSuite.h"

namespace i3ds
{

struct Address
{
  Address(std::string address);
  Address(NodeID n, EndpointID e) : node(n), endpoint(e) {};
  Address() : node(0), endpoint(0) {};

  NodeID node;
  EndpointID endpoint;

  std::string to_string() const;
};

class Message
{
public:

  Message() {};
  virtual ~Message() {};

  inline Address address() const {return address_;}

  inline NodeID node() const {return address().node;}
  inline EndpointID endpoint() const {return address().endpoint;}

  inline byte* data() {return payload_.data<byte>();}
  inline const byte* data() const {return payload_.data<byte>();}
  inline size_t size() const {return payload_.size();}

  // Set address of message.
  void set_address(Address address) {address_ = address;}

  // Set payload allocated.
  //
  // If copy is false the message will take ownership and free data after use.
  void set_payload(byte* data, size_t size, bool copy = true);

  // Check if message has non-zero payload.
  bool has_payload() const {return size() > 0;}

private:

  friend class Socket;

  Address address_;
  zmq::message_t payload_;
};

class Socket
{
public:

  typedef std::unique_ptr<Socket> Ptr;

  virtual ~Socket();

  void Bind(std::string address);
  void Connect(std::string address);

  void Send(Message& message);
  bool Receive(Message& message, int timeout_ms = -1);

  void Filter(Address address);
  void FilterAll();

private:

  friend class Context;

  Socket(zmq::socket_t socket);

  zmq::socket_t socket_;
};

class Context
{
public:

  typedef std::shared_ptr<Context> Ptr;

  Context();
  virtual ~Context() {};

  static Ptr Create() {return std::make_shared<Context>();}

  Socket::Ptr Publisher() {return CreateSocket(ZMQ_PUB);}
  Socket::Ptr Subscriber() {return CreateSocket(ZMQ_SUB);}
  Socket::Ptr Client() {return CreateSocket(ZMQ_REQ);}
  Socket::Ptr Server(){return CreateSocket(ZMQ_REP);}

private:

  Socket::Ptr CreateSocket(int type);

  zmq::context_t context_;
};



} // namespace i3ds

#endif
