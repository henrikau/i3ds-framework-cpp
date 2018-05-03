///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CLIENT_HPP
#define __I3DS_CLIENT_HPP

#include <i3ds/communication.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/service.hpp>

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Client for request/response pattern.
////////////////////////////////////////////////////////////////////////////////

class Client
{
public:

  Client(Context::Ptr context, NodeID node);
  virtual ~Client();

  // Get node ID of client.
  NodeID node() const {return node_;}

  // Send request for client.
  template<typename T>
  void Send(typename T::Data& data)
  {
    Message request;
    Encode<typename T::RequestCodec>(request, data.request);
    Send(T::endpoint, request);
  }

  // Receive response for client.
  template<typename T>
  void Receive(typename T::Data& data)
  {
    Message response;
    Receive(T::endpoint, response);
    Decode<typename T::ResponseCodec>(response, data.response);
    Check<typename T::ResponseCodec::Data>(data.response);
  }

  // Execute call for client, returns true if successful.
  template<typename T>
  void Call(typename T::Data& data)
  {
    Send<T>(data);
    Receive<T>(data);
  }

  // Releases socket
  void Stop();

  // Check pending request status.
  bool pending() const {return pending_;}

  // Get timeout in milliseconds for client, -1 is forever, 0 immediate.
  int timeout() const {return timeout_;}

  // Set timeout in milliseconds for client, -1 is forever, 0 immediate.
  void set_timeout(int t) {timeout_ = t;}

private:

  // Check the response, do nothing in general case.
  template<typename T>
  void Check(T& response) {};

  // Reset socket if receive fails.
  void Reset();

  // Send request message for client.
  void Send(EndpointID endpoint, Message& request);

  // Receive response message for client.
  void Receive(EndpointID endpoint, Message& response);

  // Node ID.
  const NodeID node_;

  // Context reference.
  Context::Ptr context_;

  // Client socket.
  Socket::Ptr socket_;

  // Pending status.
  bool pending_;

  // The timeout for waiting on requests.
  int timeout_;
};

// Template specialization for commands to raise CommandError.
template<>
void Client::Check<CommandResponse>(CommandResponse& response);

} // namespace i3ds

#endif
