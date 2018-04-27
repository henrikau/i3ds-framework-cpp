///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CODEC_HPP
#define __I3DS_CODEC_HPP

#include <stdexcept>
#include <cstring>

#include "communication.hpp"
#include "exception.hpp"

////////////////////////////////////////////////////////////////////////////////
/// Macro to create codec wrapper for ASN.1 uPER encode/decode functions.
///
/// The resulting class (struct) has name of type plus codec. Example:
///
///     CODEC(Foo) => FooCodec
///
/// The class contains the type of the codec, the maximal buffer size and
/// encode/decode static function wrappers. It is used as template argument.
/////////////////////////////////////////////////////////////////////////////////

#define CODEC(NAME) struct NAME ## Codec { \
    typedef NAME Data; \
    static const int max_size = NAME ## _REQUIRED_BYTES_FOR_ENCODING; \
    static inline void Initialize(Data& val) { \
      NAME ## _Initialize(&val); \
    } \
    static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints) { \
      return NAME ## _Encode(val, pBitStrm, pErrCode, bCheckConstraints); \
    } \
    static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode) { \
      return NAME ## _Decode(pVal, pBitStrm, pErrCode); \
    } \
}

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// NullCodec for empty message payload, does nothing.
////////////////////////////////////////////////////////////////////////////////

struct NullCodec
{
  typedef struct {} Data;

  static const int max_size = 0;

  static inline void Initialize(Data& val) {};

  static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
  {
    return true;
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode)
  {
    return true;
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Utility function for creating std::string from T_String and similar.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline std::string to_string(const T& ts)
{
  return std::string((const char*) ts.arr, ts.nCount);
}

////////////////////////////////////////////////////////////////////////////////
/// Utility function for setting T_String and similar from std::string.
///
/// The result is truncated if the provided string is too long.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline size_t set_string(T& ts, std::string s)
{
  size_t n = std::min(sizeof(ts.arr), s.size());

  ts.nCount = n;
  memcpy(ts.arr, s.c_str(), n);

  return n;
}

////////////////////////////////////////////////////////////////////////////////
/// Template function for encoding ASN.1 data into messages.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
void Encode(Message& message, const typename T::Data& data)
{
  BitStream bs;

  bs.buf = (byte*) calloc(T::max_size, sizeof(byte));
  bs.count = T::max_size;
  bs.currentByte = 0;
  bs.currentBit = 0;

  int errcode = 0;

  if (!T::Encode(&data, &bs, &errcode, true))
    {
      free(bs.buf);
      throw CodecError("Cannot encode: Bad data " + std::to_string(errcode));
    }

  message.set_payload(bs.buf, bs.currentByte + 1, false);
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for null codec, sets null message with size 0.
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Encode<NullCodec>(Message& message, const NullCodec::Data& data)
{
}

////////////////////////////////////////////////////////////////////////////////
/// Template function for decoding ASN.1 data from messages.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
void Decode(const Message& message, typename T::Data& data)
{
  BitStream bs;

  bs.buf = (byte*) message.data();
  bs.count = message.size();
  bs.currentByte = 0;
  bs.currentBit = 0;

  int errcode = 0;

  if (!T::Decode(&data, &bs, &errcode))
    {
      throw CodecError("Cannot decode: Bad data " + std::to_string(errcode));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for null codec, expects message with size 0.
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<NullCodec>(const Message& message, NullCodec::Data& data)
{
  if (message.has_payload())
    {
      throw CodecError("Cannot decode: Expected no payload for null codec");
    }
}

} // namespace i3ds

#endif
