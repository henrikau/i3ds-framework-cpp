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

#include "message.hpp"

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

  static inline flag Encode(const Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints) {
    return true;
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode) {
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
/// Template class for encoding ASN.1 data into messages.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Encoder
{
public:

  typename T::Data data;

  Encoder()
  {
    T::Initialize(data);
  }

  void Encode(Message& message) const
  {
    BitStream bs;

    bzero(buffer_, sizeof(buffer_));

    bs.buf = buffer_;
    bs.count = sizeof(buffer_);
    bs.currentByte = 0;
    bs.currentBit = 0;

    int errcode = 0;

    if (!T::Encode(&data, &bs, &errcode, true))
      {
	throw std::runtime_error("Cannot encode: Bad data " + std::to_string(errcode));
      }

    message.data = bs.buf;
    message.size = bs.currentByte + 1;
  }

private:

  mutable byte buffer_[T::max_size];
};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for null codec, sets null message with size 0.
////////////////////////////////////////////////////////////////////////////////

template<>
class Encoder<NullCodec>
{
public:

  NullCodec::Data data;

  void Encode(Message& message)
  {
    message.data = NULL;
    message.size = 0;
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Template class for decoding ASN.1 data from messages.
////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Decoder
{
public:

  typename T::Data data;

  Decoder()
  {
    T::Initialize(data);
  }

  void Decode(const Message& message)
  {
    BitStream bs;

    bs.buf = message.data;
    bs.count = message.size;
    bs.currentByte = 0;
    bs.currentBit = 0;

    int errcode = 0;

    if (!T::Decode(&data, &bs, &errcode))
      {
	throw std::runtime_error("Cannot decode: Bad data " + std::to_string(errcode));
      }
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for null codec, expects message with size 0.
////////////////////////////////////////////////////////////////////////////////

template<>
class Decoder<NullCodec>
{
public:

  NullCodec::Data data;

  void Decode(const Message& message)
  {
    if (message.size > 0)
      {
	throw std::runtime_error("Cannot decode: Expected zero data for null codec");
      }
  }
};

} // namespace i3ds

#endif
