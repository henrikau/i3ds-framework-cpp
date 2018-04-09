///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CAMERA_HPP
#define __I3DS_CAMERA_HPP

#include "Camera.h"

#include "sensor.hpp"
#include "service.hpp"
#include "codec.hpp"

namespace i3ds
{

CODEC(CameraExposure);
CODEC(CameraAutoExposure);
CODEC(CameraRegion);
CODEC(CameraFlash);
CODEC(CameraPattern);
CODEC(CameraConfiguration);

CODEC(CameraMeasurement1M);
CODEC(CameraMeasurement4M);
CODEC(CameraMeasurement8M);
CODEC(StereoCameraMeasurement4M);
CODEC(StereoCameraMeasurement8M);

class Camera : public Sensor
{
public:

  // Camera services.
  typedef Service<16, CameraExposureCodec, CommandResponseCodec> ExposureService;
  typedef Service<17, CameraAutoExposureCodec, CommandResponseCodec> AutoExposureService;
  typedef Service<18, CameraRegionCodec, CommandResponseCodec> RegionService;
  typedef Service<19, CameraFlashCodec, CommandResponseCodec> FlashService;
  typedef Service<20, CameraPatternCodec, CommandResponseCodec> PatternService;
  typedef Service<21, NullCodec, CameraConfigurationCodec> ConfigurationService;

  // Constructor and destructor.
  Camera(Context::Ptr context, NodeID id);
  virtual ~Camera();

  // Get exposure time for camera.
  virtual ExposureTime exposure() const = 0;

  // Get gain for camera.
  virtual SensorGain gain() const = 0;

  // Get auto exposure for camera.
  virtual bool auto_exposure_enabled() const = 0;

  // Get exposure time limit for camera with auto exposure.
  virtual ExposureTime max_exposure() const = 0;

  // Get gain limit for camera with auto exposure.
  virtual SensorGain max_gain() const = 0;

  // Get the region of interest enabled for camera.
  virtual bool region_enabled() const = 0;

  // Get the region of interest for the camera.
  virtual PlanarRegion region() const = 0;

  // Get the flash illumination for the camera.
  virtual bool flash_enabled() const = 0;

  // Get the flash strength for the camera.
  virtual FlashStrength flash_strength() const = 0;

  // Get the pattern illumination for the camera.
  virtual bool pattern_enabled() const = 0;

  // Get the pattern sequence for the camera.
  virtual PatternSequence pattern_sequence() const = 0;

  // Set exposure time for camera.
  virtual void set_exposure(ExposureTime exposure, SensorGain gain) = 0;

  // Set auto exposure for camera.
  virtual void set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain) = 0;

  // Set the region of interest for the camera.
  virtual void set_region(bool enable, PlanarRegion region) = 0;

  // Set the flash illumination for the camera.
  virtual void set_flash(bool enable, FlashStrength strength) = 0;

  // Set the pattern illumination for the camera.
  virtual void set_pattern(bool enable, PatternSequence sequence) = 0;

private:

  // Handler for camera exposure command.
  void handle_exposure(ExposureService::Data& command);

  // Handler for camera auto exposure command.
  void handle_region(RegionService::Data& command);

  // Handler for camera auto exposure command.
  void handle_auto_exposure(AutoExposureService::Data& command);

    // Handler for camera auto exposure command.
  void handle_flash(FlashService::Data& command);

  // Handler for camera auto exposure command.
  void handle_pattern(PatternService::Data& command);

  // Handler for camera configuration query.
  void handle_configuration(ConfigurationService::Data& config) const;
};

} // namespace i3ds

#endif
