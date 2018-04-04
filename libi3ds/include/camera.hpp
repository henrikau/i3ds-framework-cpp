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

CODEC(CameraCommand);
CODEC(CameraConfiguration);

class Camera : public Sensor
{
public:

  // Camera services.
  typedef Service<CameraCommandCodec, SensorResponseCodec> CommandService;
  typedef Service<NullCodec, CameraConfigurationCodec> ConfigurationService;

  // Constructor and destructor.
  Camera(Context::Ptr context, NodeID id);
  virtual ~Camera();

  // Get exposure time for camera.
  virtual void get_exposure(ExposureTime& exposure) const {};

  // Get gain for camera.
  virtual void get_gain(SensorGain& gain) const {};

  // Get auto exposure for camera.
  virtual void get_auto_exposure(bool& auto_exposure) const {};

  // Get exposure time limit for camera with auto exposure.
  virtual void get_exposure_limit(ExposureTime& exposure_limit) const {};

  // Get gain limit for camera with auto exposure.
  virtual void get_gain_limit(SensorGain& gain_limit) const {};

  // Get the region of interest for the camera.
  virtual void get_region(PlanarRegion& region) const {};

  // Get the flash illumination for the camera.
  virtual void get_flash_illumination(bool& flash_enable) const {};

  // Get the flash strength for the camera.
  virtual void get_flash_strength(FlashStrength& flash_strength) const {};

  // Get the pattern illumination for the camera.
  virtual void get_pattern_illumination(bool& pattern_enabled) const {};

  // Get the pattern for the camera.
  virtual void get_illumination_pattern(IlluminationPattern& pattern) const {};

protected:

  // Set default handler for endpoints.
  void set_command_handler();
  void set_configuration_handler();

  // Get camera status.
  void get_camera_configuration(CameraConfiguration& config) const;

  // Execute camera command.
  ResultCode execute_camera_command(CameraCommand& command);

  // Set exposure time for camera.
  virtual ResultCode set_exposure(ExposureTime exposure) {return error_unsupported;}

  // Set gain for camera.
  virtual ResultCode set_gain(SensorGain gain) {return error_unsupported;}

  // Set auto exposure for camera.
  virtual ResultCode set_auto_exposure(bool auto_exposure) {return error_unsupported;}

  // Set exposure time limit for camera with auto exposure.
  virtual ResultCode set_exposure_limit(ExposureTime exposure_limit) {return error_unsupported;}

  // Set gain limit for camera with auto exposure.
  virtual ResultCode set_gain_limit(SensorGain gain_limit) {return error_unsupported;}

  // Set the region of interest for the camera.
  virtual ResultCode set_region(PlanarRegion region) {return error_unsupported;}

  // Set the flash illumination for the camera.
  virtual ResultCode set_flash_illumination(bool flash_enable) {return error_unsupported;}

  // Set the flash strength for the camera.
  virtual ResultCode set_flash_strength(FlashStrength flash_strength) {return error_unsupported;}

  // Set the pattern illumination for the camera.
  virtual ResultCode set_pattern_illumination(bool pattern_enabled) {return error_unsupported;}

  // Set the next pattern for the camera.
  virtual ResultCode set_illumination_pattern(IlluminationPattern pattern) {return error_unsupported;}

private:

  // Handler for camera configuration query.
  void handle_configuration_query(ConfigurationService::Data& config) const;

  // Handler for camera command.
  void handle_command(CommandService::Data& command);

};

} // namespace i3ds

#endif
