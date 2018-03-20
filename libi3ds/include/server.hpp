///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SERVER_HPP
#define __I3DS_SERVER_HPP

#include <unordered_map>

#include "communication.hpp"
#include "service_handler.hpp"

namespace i3ds
{

class Server
{
public:

  Server(Context& context, SensorID sensor);
  virtual ~Server();

  // Get sensor ID of server.
  SensorID sensor() const {return sensor_;}
  
  // Register service handler for endpoint ID.
  template<typename RequestCodec, typename ResponseCodec>
  void set_service(EndpointID endpoint, typename ServiceHandler<RequestCodec,ResponseCodec>::Operation operation)
  {
    set_handler(endpoint, ServiceHandler<RequestCodec,ResponseCodec>::Create(operation));
  }

  // Register generic handler for endpoint ID.
  void set_handler(EndpointID endpoint, Handler::Ptr handler);

  // Delete handler for endpoint ID.
  void delete_handler(EndpointID endpoint);

  // Spin the server handling request until context is terminated.
  void Spin();

  // Spin the server handling one requests, return true if message was handled.
  bool SpinOnce(int timeout_ms = -1);

private:
  
  // Sensor ID.
  const SensorID sensor_;

  // Context reference.
  Context& context_;

  // Server socket.
  Socket::Ptr socket_;

  // Map with handlers for endpoints.
  std::unordered_map<EndpointID, Handler::Ptr> handlers_;
};  

} // namespace i3ds

#endif
