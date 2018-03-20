///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_SERVICE_HANDLER_HPP
#define __I3DS_SERVICE_HANDLER_HPP

#include <functional>

#include "handler.hpp"
#include "codec.hpp"

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Service handler for request/response pattern.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
class ServiceHandler : public Handler
{
public:

  typedef std::function<void(typename T::Data&)> Operation;

  static inline Handler::Ptr Create(Operation operation)
  {
    return Handler::Ptr(new ServiceHandler<T>(operation));
  }

  ServiceHandler(Operation operation) : operation_(operation) {};

  virtual ~ServiceHandler() {};

  virtual void handle(const Message& request, Message& response)
  {
    T::RequestCodec::Initialize(data_.request);
    T::ResponseCodec::Initialize(data_.response);

    decoder_.Decode(request, data_.request);
    operation_(data_);
    encoder_.Encode(response, data_.response);
  }

private:

  const Operation operation_;

  typename T::Data data_;

  Decoder<typename T::RequestCodec> decoder_;
  Encoder<typename T::ResponseCodec> encoder_;
};

} // namespace i3ds

#endif
