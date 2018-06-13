///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CLIENT_FACTORY_HPP
#define __I3DS_CLIENT_FACTORY_HPP

#include <type_traits>

#include <i3ds/communication.hpp>
#include <i3ds/sensor_client.hpp>

namespace i3ds
{

class ClientFactory
{
public:

  typedef std::shared_ptr<ClientFactory> Ptr;

  static Ptr Create(Context::Ptr context);

  ClientFactory(Context::Ptr context);
  virtual ~ClientFactory();
  
  template<typename T>
  typename T::Ptr Create(NodeID node)
  {
    static_assert(std::is_base_of<SensorClient, T>::value, "Not a SensorClient");
    return std::make_shared<T>(context_, node);
  }

private:

  Context::Ptr context_;
};

} // namespace i3ds

#endif
