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

template<typename RequestCodec, typename ResponseCodec>
class ServiceHandler : public Handler
{
public:

  typedef std::function<void(const typename RequestCodec::Data&, typename ResponseCodec::Data&)> Operation;

  static inline Handler::Ptr Create(Operation operation)
  {
    return Handler::Ptr(new ServiceHandler<RequestCodec, ResponseCodec>(operation));
  }

  ServiceHandler(Operation operation) : operation_(operation) {};

  virtual ~ServiceHandler() {};

  virtual void handle(const Message& request, Message& response)
  {
    ResponseCodec::Initialize(encoder_.data);

    decoder_.Decode(request);
    operation_(decoder_.data, encoder_.data);
    encoder_.Encode(response);
  }

private:

  const Operation operation_;

  Decoder<RequestCodec> decoder_;
  Encoder<ResponseCodec> encoder_;
};

////////////////////////////////////////////////////////////////////////////////
/// Service handler for request/response pattern with empty request.
////////////////////////////////////////////////////////////////////////////////

template<typename ResponseCodec>
class ServiceHandler<NullCodec, ResponseCodec> : public Handler
{
public:

  typedef std::function<void(typename ResponseCodec::Data&)> Operation;

  ServiceHandler(Operation operation) : operation_(operation) {};

  static inline Handler::Ptr Create(Operation operation)
  {
    return Handler::Ptr(new ServiceHandler<NullCodec, ResponseCodec>(operation));
  }

  virtual ~ServiceHandler() {};

  virtual void handle(const Message& request, Message& response)
  {
    ResponseCodec::Initialize(encoder_.data);

    decoder_.Decode(request);
    operation_(encoder_.data);
    encoder_.Encode(response);
  }

private:

  const Operation operation_;

  Decoder<NullCodec> decoder_;
  Encoder<ResponseCodec> encoder_;
};

////////////////////////////////////////////////////////////////////////////////
/// Service handler for request/response pattern with empty response.
////////////////////////////////////////////////////////////////////////////////

template<typename RequestCodec>
class ServiceHandler<RequestCodec, NullCodec> : public Handler
{
public:

  typedef std::function<void(const typename RequestCodec::Data&)> Operation;

  ServiceHandler(Operation operation) : operation_(operation) {};

  virtual ~ServiceHandler() {};

  static inline Handler::Ptr Create(Operation operation)
  {
    return Handler::Ptr(new ServiceHandler<RequestCodec, NullCodec>(operation));
  }

  virtual void handle(const Message& request, Message& response)
  {
    decoder_.Decode(request);
    operation_(decoder_.data);
    encoder_.Encode(response);
  }

private:

  const Operation operation_;

  Decoder<RequestCodec> decoder_;
  Encoder<NullCodec> encoder_;
};

} // namespace i3ds

#endif
