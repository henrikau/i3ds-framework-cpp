///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018-2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_DEPTHMAP_HPP
#define __I3DS_DEPTHMAP_HPP

#include <i3ds/DepthMap.h>
#include <i3ds/taste-types.h>
#include <i3ds/codec.hpp>
#include <vector>

namespace i3ds
{

CODEC(DepthMapDescriptor);

struct DepthMap
{
  DepthMapDescriptor descriptor;
  std::vector<float> depths;
};

struct DepthMapCodec
{
  typedef DepthMap Data;

  static inline void Initialize(Data& val)
  {
    DepthMapDescriptorCodec::Initialize(val.descriptor);
    val.descriptor.depth_format = depth_f;
    val.descriptor.depth_size = sizeof(float);
    val.depths.empty();
  };

  static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
  {
    return DepthMapDescriptorCodec::Encode(&(val->descriptor), pBitStrm, pErrCode, bCheckConstraints);
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode)
  {
    return DepthMapDescriptorCodec::Decode(&pVal->descriptor, pBitStrm, pErrCode);
  }

};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codec, sets multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Encode<DepthMapCodec>(Message& message, const DepthMapCodec::Data& data)
{
  Encode<DepthMapDescriptorCodec>(message, data.descriptor);

  const byte* d = reinterpret_cast<const byte*>(data.depths.data());
  const size_t s = data.depths.size() * sizeof(float);
  
  message.append_payload(d, s);
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codes, reads multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<DepthMapCodec>(const Message& message, DepthMapCodec::Data& data)
{
  Decode<DepthMapDescriptorCodec>(message, data.descriptor);

  const float* d = reinterpret_cast<const float*>(message.data(1));
  const size_t s = message.size(1) / sizeof(float);

  data.depths.assign(d, d + s);
}

} // namespace i3ds

#endif // __I3DS_POINTCLOUD_HPP
