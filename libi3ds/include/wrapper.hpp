///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_WRAPPER_HPP
#define __I3DS_WRAPPER_HPP

#include <functional>

#include "handler.hpp"
#include "codec.hpp"

namespace i3ds
{

template<typename RequestCodec, typename ResponseCodec>
class CommandWrapper : public Handler
{
public:

  typedef std::function<void(const typename RequestCodec::Data&, typename ResponseCodec::Data&)> Operation;

  CommandWrapper(Operation operation) : operation_(operation) {};
  
  virtual ~CommandWrapper() {};

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

template<typename ResponseCodec>
class QueryWrapper : public Handler
{
public:

  typedef std::function<void(typename ResponseCodec::Data&)> Operation;

  QueryWrapper(Operation operation) : operation_(operation) {};
  
  virtual ~QueryWrapper() {};

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

} // namespace i3ds

#endif
