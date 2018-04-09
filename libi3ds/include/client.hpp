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

#include "communication.hpp"
#include "codec.hpp"

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

  // Execute call for client, returns true if successful.
  template<typename T>
  bool Call(typename T::Data& data, int timeout_ms = -1)
  {
    Message request, response;

    Encode<typename T::RequestCodec>(request, data.request);

    if (!Execute(T::endpoint, request, response, timeout_ms))
      {
	return false;
      }

    Decode<typename T::ResponseCodec>(response, data.response);

    return true;
  }

  // Releases socket
  void Stop();

private:

  // Reset socket if receive fails.
  void Reset();

  // Execute call.
  bool Execute(EndpointID endpoint, Message& request, Message& response, int timeout_ms);

  // Node ID.
  const NodeID node_;

  // Context reference.
  Context::Ptr context_;

  // Client socket.
  Socket::Ptr socket_;
};

} // namespace i3ds

#endif
