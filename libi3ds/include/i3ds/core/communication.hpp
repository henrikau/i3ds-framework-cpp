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
#include <unordered_set>
#include <zmq.hpp>

#include "Common.h"

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

class Context
{
public:

  typedef std::shared_ptr<Context> Ptr;

  Context(std::string addr_srv_addr = "tcp://localhost:24680");
  virtual ~Context() {};

  static Ptr Create() {return std::make_shared<Context>();}

  std::string get_config(NodeID node, int type);

private:

  friend class Socket;

  zmq::context_t context_;
  zmq::socket_t address_socket_;
  std::string addr_srv_addr_;
  bool connected_to_addr_srv;

};

class Socket
{
public:

  typedef std::unique_ptr<Socket> Ptr;

  virtual ~Socket();

  static Socket::Ptr Publisher(Context::Ptr context) {return CreateSocket(context, ZMQ_PUB);}
  static Socket::Ptr Subscriber(Context::Ptr context) {return CreateSocket(context, ZMQ_SUB);}
  static Socket::Ptr Client(Context::Ptr context) {return CreateSocket(context, ZMQ_REQ);}
  static Socket::Ptr Server(Context::Ptr context) {return CreateSocket(context, ZMQ_REP);}

  void Attach(NodeID node);

  void Send(Message& message);
  bool Receive(Message& message, int timeout_ms = -1);

  void Filter(Address address);
  void FilterAll();

private:

  static Socket::Ptr CreateSocket(Context::Ptr context, int type);

  Socket(Context::Ptr context, int type);

  const int type_;
  const Context::Ptr context_;

  zmq::socket_t socket_;
  std::unordered_set<NodeID> attached_;
};


} // namespace i3ds

/// Hash and equal_to for Address.
namespace std
{

template <>
class hash<i3ds::Address>
{
public:
  size_t operator()(const i3ds::Address& address) const
  {
    size_t a = address.node << 8 & address.endpoint;
    return a;
  }
};

template<>
class equal_to<i3ds::Address>
{
public:
  size_t operator()(const i3ds::Address& a, const i3ds::Address& b) const
  {
    return a.node == b.node && a.endpoint == b.endpoint;
  }
};
}

#endif
