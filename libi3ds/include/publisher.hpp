///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_PUBLISHER_HPP
#define __I3DS_PUBLISHER_HPP

#include <memory>
#include <unordered_map>
#include <functional>

#include "communication.hpp"
#include "codec.hpp"

namespace i3ds
{
////////////////////////////////////////////////////////////////////////////////
/// Publisher in request/response pattern.
////////////////////////////////////////////////////////////////////////////////

class Publisher
{
public:

  Publisher(Context::Ptr context, SensorID sensor);
  virtual ~Publisher();

  // Get sensor ID of publisher.
  SensorID sensor() const {return sensor_;}

  // Publish the data for the given endpoint ID.
  template<typename T>
  void publish(EndpointID endpoint, const typename T::Data& data)
  {
    if (!socket_)
      {
	Reset();
      }
    
    Message message;

    message.set_address(Address(sensor_, endpoint));
    Encode<T>(message, data);
    socket_->Send(message);
  }

private:

  void Reset();

  // Sensor ID.
  const SensorID sensor_;

  // Context reference.
  Context::Ptr context_;

  // Publisher socket.
  Socket::Ptr socket_;
};

} // namespace i3ds

#endif
