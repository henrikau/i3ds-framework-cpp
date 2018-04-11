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
  typedef Command<16, CameraExposureCodec>      ExposureService;
  typedef Command<17, CameraAutoExposureCodec>  AutoExposureService;
  typedef Command<18, CameraRegionCodec>        RegionService;
  typedef Command<19, CameraFlashCodec>         FlashService;
  typedef Command<20, CameraPatternCodec>       PatternService;
  typedef Query  <21, CameraConfigurationCodec> ConfigurationService;

  // Constructor and destructor.
  Camera(Context::Ptr context, NodeID id);
  virtual ~Camera();

  // Get exposure time for camera.
  virtual ExposureTime exposure() const = 0;

  // Get gain for camera.
  virtual SensorGain gain() const = 0;

  // Get auto exposure for camera.
  virtual bool auto_exposure_enabled() const {return false;}

  // Get exposure time limit for camera with auto exposure.
  virtual ExposureTime max_exposure() const {return 0;}

  // Get gain limit for camera with auto exposure.
  virtual SensorGain max_gain() const {return 0;}

  // Get the region of interest enabled for camera.
  virtual bool region_enabled() const {return false;}

  // Get the region of interest for the camera.
  virtual PlanarRegion region() const {return {0,0,0,0};}

  // Get the flash illumination for the camera.
  virtual bool flash_enabled() const {return false;}

  // Get the flash strength for the camera.
  virtual FlashStrength flash_strength() const {return 0;}

  // Get the pattern illumination for the camera.
  virtual bool pattern_enabled() const {return false;}

  // Get the pattern sequence for the camera.
  virtual PatternSequence pattern_sequence() const {return 0;}

  // Set exposure time for camera.
  virtual void set_exposure(ExposureTime exposure, SensorGain gain) = 0;

  // Set auto exposure for camera.
  virtual void set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain);

  // Set the region of interest for the camera.
  virtual void set_region(bool enable, PlanarRegion region);

  // Set the flash illumination for the camera.
  virtual void set_flash(bool enable, FlashStrength strength);

  // Set the pattern illumination for the camera.
  virtual void set_pattern(bool enable, PatternSequence sequence);

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
