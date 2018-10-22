///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POINTCLOUD_HPP
#define __I3DS_POINTCLOUD_HPP

#include <i3ds/PointCloud.h>
#include <i3ds/taste-types.h>
#include <i3ds/codec.hpp>
#include <vector>

namespace i3ds
{

CODEC(T_UInt32);

class PointCloud
{
public:

  T_UInt32 numPoints = 0;

  void append_points(const std::vector<Point> points)
  {
    points_.reserve(points_.size() + points.size());
    points_.insert(points_.end(), points.begin(), points.end());
    numPoints = points_.size();
  }

  void clear_points()
  {
    points_.clear();
  }

  size_t message_size() const
  {
    return numPoints * sizeof(Point);
  }

  bool set_raw_data(const byte *data, const T_UInt32 nElements)
  {
    numPoints = nElements;
    points_.resize(numPoints);
    memcpy(points_.data(), data, message_size());
    return true;
  }

  const byte* bytes() const
  {
    return reinterpret_cast<const byte *>(points_.data());
  }

  const std::vector<Point> points() const
  {
    return points_;
  }

private:

  std::vector<Point> points_;
};

struct PointCloudCodec
{
  typedef PointCloud Data;

  static inline flag Encode(Data* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
  {
    return T_UInt32Codec::Encode(&(val->numPoints), pBitStrm, pErrCode, bCheckConstraints);
  }

  static inline flag Decode(Data* pVal, BitStream* pBitStrm, int* pErrCode)
  {
    return T_UInt32Codec::Decode(&pVal->numPoints, pBitStrm, pErrCode);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codec, sets multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Encode<PointCloudCodec>(Message& message, const PointCloudCodec::Data& data)
{
  Encode<T_UInt32Codec>(message, data.numPoints);

  message.append_payload(data.bytes(), data.message_size());
}

////////////////////////////////////////////////////////////////////////////////
/// Template specialization for frame codes, reads multipart message
////////////////////////////////////////////////////////////////////////////////

template<>
inline void Decode<PointCloudCodec>(const Message& message, PointCloudCodec::Data& data)
{
  T_UInt32 numPoints;
  Decode<T_UInt32Codec>(message, numPoints);

  data.clear_points();

  data.set_raw_data(message.data(1), numPoints);
}

} // namespace i3ds

#endif // __I3DS_POINTCLOUD_HPP
