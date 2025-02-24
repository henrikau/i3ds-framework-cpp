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
#include <vector>
#include <mutex>
#include <zmq.hpp>

#include <i3ds/Common.h>

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

  // Set address of message.
  void set_address(Address address) {address_ = address;}

  // Get number of payloads.
  int payloads() const {return payload_.size();}

  // Check if message has payloads.
  bool has_payload() const {return payloads() > 0;}

  // Get payload i.
  byte* data(int i = 0);

  // Get payload i as const.
  const byte* data(int i = 0) const;

  // Get size of payload i.
  size_t size(int i = 0) const;

  // Append payload to message.
  //
  // If copy is false the message will take ownership and free data after use.
  void append_payload(const byte* data, size_t size, bool copy = true);

private:

  friend class Socket;

  Address address_;
  std::vector<zmq::message_t> payload_;
};

class Context
{
public:

  typedef std::shared_ptr<Context> Ptr;

  // Constructor for Context
  //
  // The default constructor uses the environment variable I3DS_ADDR_SRV_URL as the address 
  // of the address server. If it is not found, the default_addr_ string is used. 
  // If the constructor taking a string is used, that overrides either.
  Context();
  Context(std::string addr_srv_url);
  virtual ~Context() = default;

  static Ptr Create() {return std::make_shared<Context>();}
  static Ptr Create(std::string addr_srv_url) {return std::make_shared<Context>(addr_srv_url);}

  std::string get_config(NodeID node, int type);

  std::string get_addr_srv_url() const;

  static const std::string DEFAULT_ADDR_SRV_URL;

private:

  friend class Socket;

  zmq::context_t context_;
  zmq::socket_t address_socket_;
  bool connected_to_addr_srv_;
  std::string addr_srv_url_;
  std::mutex mutex_;

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
  void Receive(Message& message, int timeout_ms = -1);

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
struct hash<i3ds::Address>
{
public:
  size_t operator()(const i3ds::Address& address) const
  {
    size_t a = address.node << 8 & address.endpoint;
    return a;
  }
};

template<>
struct equal_to<i3ds::Address>
{
public:
  size_t operator()(const i3ds::Address& a, const i3ds::Address& b) const
  {
    return a.node == b.node && a.endpoint == b.endpoint;
  }
};
}

#endif
