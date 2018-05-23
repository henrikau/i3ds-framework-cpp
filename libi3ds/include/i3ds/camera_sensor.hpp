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

#include <i3ds/Camera.h>

#include <i3ds/sensor.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>
#include <i3ds/topic.hpp>

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

  // Shared pointer for camera interfaces.
  typedef std::shared_ptr<Camera> Ptr;

  // Camera services.
  typedef Command<16, CameraExposureCodec>      ExposureService;
  typedef Command<17, CameraAutoExposureCodec>  AutoExposureService;
  typedef Command<18, CameraRegionCodec>        RegionService;
  typedef Command<19, CameraFlashCodec>         FlashService;
  typedef Command<20, CameraPatternCodec>       PatternService;
  typedef Query  <21, CameraConfigurationCodec> ConfigurationService;

  // Camera topics
  typedef Topic<128, CameraMeasurement1MCodec> MonoFrame1MTopic;
  typedef Topic<129, CameraMeasurement4MCodec> MonoFrame4MTopic;
  typedef Topic<130, CameraMeasurement8MCodec> MonoFrame8MTopic;
  typedef Topic<131, StereoCameraMeasurement4MCodec> StereoFrame4MTopic;
  typedef Topic<132, StereoCameraMeasurement8MCodec> StereoFrame8MTopic;

  // Constructor for camera.
  Camera(NodeID node) : Sensor(node) {};

  // Destructor for camera.
  virtual ~Camera() {};

  // Get shutter time for camera in microseconds.
  virtual ShutterTime shutter() const = 0;

  // Get gain for camera in decibel.
  virtual SensorGain gain() const = 0;

  // Get auto exposure for camera.
  virtual bool auto_exposure_enabled() const {return false;}

  // Get shutter time limit for camera with auto exposure in microseconds.
  virtual ShutterTime max_shutter() const {return 0;}

  // Get gain limit for camera with auto exposure in decibel.
  virtual SensorGain max_gain() const {return 0.0;}

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

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for camera exposure command, must be overloaded.
  virtual void handle_exposure(ExposureService::Data& command) = 0;

  // Handler for camera auto exposure command.
  virtual void handle_auto_exposure(AutoExposureService::Data& command);

  // Handler for camera region command.
  virtual void handle_region(RegionService::Data& command);

  // Handler for camera flash command.
  virtual void handle_flash(FlashService::Data& command);

  // Handler for camera pattern command.
  virtual void handle_pattern(PatternService::Data& command);

  // Handler for camera configuration query.
  virtual void handle_configuration(ConfigurationService::Data& config) const;
};

} // namespace i3ds

#endif
