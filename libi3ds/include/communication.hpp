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
  Address(SensorID s, EndpointID e) : sensor(s), endpoint(e) {};
  Address() : sensor(0), endpoint(0) {};

  SensorID sensor;
  EndpointID endpoint;

  std::string to_string() const;
};
  
class Message
{
public:

  Message(Address a) {set_address(a);}
  Message() {};
  virtual ~Message() {};

  Address address() const;

  byte* data() const {return (byte*) payload_.data();}
  size_t size() const {return payload_.size();}

  void set_address(Address address);
  void set_payload(byte* data, size_t size);

  bool has_payload() const {return size() > 0;}

private:

  friend class Socket;

  zmq::message_t header_;
  zmq::message_t payload_;
};

class Socket
{
public:

  typedef std::unique_ptr<Socket> Ptr;

  virtual ~Socket() {};

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

  Context();
  virtual ~Context() {};

  Socket::Ptr Publisher() {return CreateSocket(ZMQ_PUB);}
  Socket::Ptr Subscriber() {return CreateSocket(ZMQ_SUB);}
  Socket::Ptr Client() {return CreateSocket(ZMQ_REQ);}
  Socket::Ptr Server(){return CreateSocket(ZMQ_REP);}
  
  void Close();

private:

  Socket::Ptr CreateSocket(int type);

  zmq::context_t context_;
};



} // namespace i3ds

#endif
