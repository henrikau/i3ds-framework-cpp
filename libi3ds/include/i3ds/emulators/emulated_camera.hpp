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

namespace i3ds
{

class EmulatedCamera : public Camera
{
public:

  EmulatedCamera(Context::Ptr context, NodeID id, int resx, int resy);
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

  const int resx_;
  const int resy_;

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

  Sampler sampler_;

  Publisher publisher_;
};

class EmulatedTIRCamera : public EmulatedCamera
{
public:

  typedef Topic<128, CameraMeasurement1MCodec> ImageMeasurement;

  EmulatedTIRCamera(Context::Ptr context, NodeID id);
  virtual ~EmulatedTIRCamera() {};

protected:

  virtual bool send_sample(unsigned long timestamp_us);

  std::unique_ptr<ImageMeasurement::Data> frame_;
};

class EmulatedHRCamera : public EmulatedCamera
{
public:

  typedef Topic<128, CameraMeasurement8MCodec> ImageMeasurement;

  EmulatedHRCamera(Context::Ptr context, NodeID id);
  virtual ~EmulatedHRCamera() {};

protected:

  virtual bool send_sample(unsigned long timestamp_us);

  std::unique_ptr<ImageMeasurement::Data> frame_;
};

class EmulatedStereoCamera : public EmulatedCamera
{
public:

  typedef Topic<128, StereoCameraMeasurement8MCodec> ImageMeasurement;

  EmulatedStereoCamera(Context::Ptr context, NodeID id);
  virtual ~EmulatedStereoCamera() {};

protected:

  virtual bool send_sample(unsigned long timestamp_us);

  std::unique_ptr<ImageMeasurement::Data> frame_;
};

} // namespace i3ds

#endif
