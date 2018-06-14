///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_FRAME_HPP
#define __I3DS_FRAME_HPP

#include <i3ds/Frame.h>
#include <i3ds/codec.hpp>
#include <vector>

namespace i3ds
{

CODEC(FrameDescriptor);

struct Image
{
  const byte* data;
  size_t size;
};

struct Frame
{
  FrameDescriptor descriptor;
  std::vector<Image> image;
};

struct FrameCodec
{
  typedef Frame Data;

  static const int max_size = FrameDescriptorCodec::max_size;

  static inline void Initialize(Data& val)
  {
    FrameDescriptorCodec::Initialize(val.descriptor);
  };

  static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
  {
    return FrameDescriptorCodec::Encode(&(val->descriptor), pBitStrm, pErrCode, bCheckConstraints);
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode)
  {
    return FrameDescriptorCodec::Decode(&pVal->descriptor, pBitStrm, pErrCode);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codec, sets multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Encode<FrameCodec>(Message& message, const FrameCodec::Data& data)
{
  Encode<FrameDescriptorCodec>(message, data.descriptor);

  for (unsigned int i = 0; i < data.image.size(); i++)
    {
      message.append_payload(data.image[i].data, data.image[i].size);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codes, reads multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<FrameCodec>(const Message& message, FrameCodec::Data& data)
{
  Decode<FrameDescriptorCodec>(message, data.descriptor);

  const int n = message.payloads();

  for (int i = 1; i < n; i++)
    {
      const Image image = {message.data(i), message.size(i)};

      data.image.push_back(image);
    }
}

} // namespace i3ds

#endif
