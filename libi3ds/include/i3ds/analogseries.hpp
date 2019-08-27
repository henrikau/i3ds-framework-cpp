///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_ANALOGSERIES_HPP
#define __I3DS_ANALOGSERIES_HPP

#include <i3ds/taste-types.h>
#include <i3ds/codec.hpp>

#include <vector>

namespace i3ds
{

CODEC(AnalogSeriesDescriptor);

struct AnalogSeries
{
  typedef float Sample;

  AnalogSeriesDescriptor descriptor;
  std::vector<float> samples;
};

struct AnalogSeriesCodec
{
  typedef AnalogSeries Data;

  static inline void Initialize(Data& val)
  {
    AnalogSeriesDescriptorCodec::Initialize(val.descriptor);
    val.samples.clear();
  };

  static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
  {
    return AnalogSeriesDescriptorCodec::Encode(&(val->descriptor), pBitStrm, pErrCode, bCheckConstraints);
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode)
  {
    return AnalogSeriesDescriptorCodec::Decode(&pVal->descriptor, pBitStrm, pErrCode);
  }

};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codec, sets multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Encode<AnalogSeriesCodec>(Message& message, const AnalogSeriesCodec::Data& data)
{
  Encode<AnalogSeriesDescriptorCodec>(message, data.descriptor);

  const byte* d = reinterpret_cast<const byte*>(data.samples.data());
  const size_t s = data.samples.size() * sizeof(float);
  
  message.append_payload(d, s);
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codes, reads multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<AnalogSeriesCodec>(const Message& message, AnalogSeriesCodec::Data& data)
{
  Decode<AnalogSeriesDescriptorCodec>(message, data.descriptor);

  const float* d = reinterpret_cast<const float*>(message.data(1));
  const size_t s = message.size(1) / sizeof(float);

  data.samples.assign(d, d + s);
}

} // namespace i3ds

#endif // __I3DS_ANALOGSERIES_HPP
