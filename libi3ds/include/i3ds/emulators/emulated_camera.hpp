///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_CAMERA_HPP
#define __I3DS_EMULATED_CAMERA_HPP

#include <memory>

#include "i3ds/core/topic.hpp"
#include "i3ds/core/publisher.hpp"
#include "i3ds/sensors/camera.hpp"
#include "i3ds/emulators/periodic_sampler.hpp"

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

namespace i3ds
{

struct FrameProperties
{
  Frame_mode_t mode;
  int data_depth;
  int pixel_size;
  int width;
  int height;
};

class EmulatedCamera : public Camera
{
public:

  EmulatedCamera(Context::Ptr context, NodeID id, FrameProperties prop);
  virtual ~EmulatedCamera();

  // Getters.
  virtual ExposureTime exposure() const {return exposure_;}
  virtual SensorGain gain() const {return gain_;}
  virtual bool auto_exposure_enabled() const {return auto_exposure_enabled_;}
  virtual ExposureTime max_exposure() const {return max_exposure_;}
  virtual SensorGain max_gain() const {return max_gain_;}
  virtual bool region_enabled() const {return region_enabled_;}
  virtual PlanarRegion region() const {return region_;}
  virtual bool flash_enabled() const {return flash_enabled_;}
  virtual FlashStrength flash_strength() const {return flash_strength_;}
  virtual bool pattern_enabled() const {return pattern_enabled_;}
  virtual PatternSequence pattern_sequence() const {return pattern_sequence_;}

  // Supported rate.
  virtual bool is_rate_supported(SampleRate rate);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

  // Handlers.
  virtual void handle_exposure(ExposureService::Data& command);
  virtual void handle_region(RegionService::Data& command);
  virtual void handle_auto_exposure(AutoExposureService::Data& command);
  virtual void handle_flash(FlashService::Data& command);
  virtual void handle_pattern(PatternService::Data& command);

  virtual bool send_sample(unsigned long timestamp_us) = 0;

  template<typename T>
  int set_meta(typename T::Data& frame, unsigned long timestamp_us)
  {
    frame.attributes.timestamp.microseconds = timestamp_us;
    frame.attributes.validity = sample_valid;
    frame.frame_mode = prop_.mode;
    frame.data_depth = prop_.data_depth;
    frame.pixel_size = prop_.pixel_size;
    frame.region = region_;

    return region_.size_x * region_.size_y * prop_.pixel_size;
  }

  ExposureTime exposure_;
  SensorGain gain_;
  bool auto_exposure_enabled_;
  ExposureTime max_exposure_;
  SensorGain max_gain_;
  bool region_enabled_;
  PlanarRegion region_;
  bool flash_enabled_;
  FlashStrength flash_strength_;
  bool pattern_enabled_;
  PatternSequence pattern_sequence_;

  FrameProperties prop_;

  Sampler sampler_;

  Publisher publisher_;

};

template<typename T>
class EmulatedMonoCamera : public EmulatedCamera
{
public:

  typedef Topic<128, T> ImageMeasurement;

  EmulatedMonoCamera(Context::Ptr context, NodeID id, FrameProperties prop)
    : EmulatedCamera(context, id, prop)
  {
    ImageMeasurement::Codec::Initialize(frame_);
  }

  virtual ~EmulatedMonoCamera() {};

protected:

  virtual bool send_sample(unsigned long timestamp_us)
  {
    int size = set_meta<T>(frame_, timestamp_us);

    BOOST_LOG_TRIVIAL(trace) << "Send mono frame: "
                             << timestamp_us << " "
                             << size / 1024.0 << "KiB";

    frame_.image.nCount = size;

    publisher_.Send<ImageMeasurement>(frame_);

    return true;
  }

  typename ImageMeasurement::Data frame_;
};


template<typename T>
class EmulatedStereoCamera : public EmulatedCamera
{
public:

  typedef Topic<128, T> ImageMeasurement;

  EmulatedStereoCamera(Context::Ptr context, NodeID id, FrameProperties prop)
    : EmulatedCamera(context, id, prop)
  {
    ImageMeasurement::Codec::Initialize(frame_);
  }

  virtual ~EmulatedStereoCamera() {};

protected:

  virtual bool send_sample(unsigned long timestamp_us)
  {
    int size = set_meta<T>(frame_, timestamp_us);

    frame_.image_left.nCount = size;
    frame_.image_right.nCount = size;

    BOOST_LOG_TRIVIAL(trace) << "Send stereo frame: "
                             << timestamp_us << " "
                             << size / 512.0 << "KiB";

    publisher_.Send<ImageMeasurement>(frame_);

    return true;
  }

  typename ImageMeasurement::Data frame_;
};

} // namespace i3ds

#endif
