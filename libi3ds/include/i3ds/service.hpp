///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SERVICE_HPP
#define __I3DS_SERVICE_HPP

#include <functional>

#include <i3ds/communication.hpp>
#include <i3ds/codec.hpp>

namespace i3ds
{

CODEC(CommandResponse);

template<EndpointID E, typename T1, typename T2>
struct Service
{
  static const EndpointID endpoint = E;

  typedef T1 RequestCodec;
  typedef T2 ResponseCodec;

  struct Data
  {
    typename RequestCodec::Data request;
    typename ResponseCodec::Data response;
  };

  static void Initialize(Data& data)
  {
    RequestCodec::Initialize(data.request);
    ResponseCodec::Initialize(data.response);
  }
};

template<EndpointID E, typename T>
struct Query : public Service<E, NullCodec, T> {};

template<EndpointID E, typename T>
struct Command : public Service<E, T, CommandResponseCodec> {};


} // namespace i3ds

#endif
