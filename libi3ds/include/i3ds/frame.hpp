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

struct FrameCodec;

inline size_t image_size(const FrameDescriptor& desc)
{
  return desc.region.size_x  * desc.region.size_y * desc.pixel_size;
}

class Frame
{
public:

  FrameDescriptor descriptor;

  inline int images() const {return image_.size();}

  inline size_t image_size(int i) const {return image_.at(i).size;}
  inline const byte* image_data(int i) const {return image_.at(i).data;}

  inline void append_image(const byte* data, size_t size) {image_.push_back({data, size});}
  inline void clear_images() {image_.clear();}

private:

  struct Image
  {
    const byte* data;
    size_t size;
  };

  std::vector<Image> image_;
};

struct FrameCodec
{
  typedef Frame Data;

  static const int max_size = FrameDescriptorCodec::max_size;

  static inline void Initialize(Data& val)
  {
    FrameDescriptorCodec::Initialize(val.descriptor);
    val.clear_images();
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

  for (int i = 0; i < data.images(); i++)
    {
      message.append_payload(data.image_data(i), data.image_size(i));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codes, reads multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<FrameCodec>(const Message& message, FrameCodec::Data& data)
{
  Decode<FrameDescriptorCodec>(message, data.descriptor);

  data.clear_images();

  for (int i = 1; i < message.payloads(); i++)
    {
      data.append_image(message.data(i), message.size(i));
    }
}

} // namespace i3ds

#endif
