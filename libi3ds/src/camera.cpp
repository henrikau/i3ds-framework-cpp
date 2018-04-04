///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "camera.hpp"

i3ds::Camera::Camera(Context::Ptr context, NodeID id)
  : Sensor(context, id)
{
  default_status_handler();
  set_command_handler();
  set_configuration_handler();
}

i3ds::Camera::~Camera()
{
}

PlanarRegion
i3ds::Camera::region() const
{
  PlanarRegion r;

  r.size_x = resolution_x();
  r.size_y = resolution_y();
  r.offset_x = 0;
  r.offset_y = 0;

  return r;
}

void
i3ds::Camera::set_command_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Camera::handle_command, this, _1);

  set_service<CommandService>(COMMAND, op);
}

void
i3ds::Camera::set_configuration_handler()
{
  using std::placeholders::_1;

  auto op = std::bind(&i3ds::Camera::handle_configuration_query, this, _1);

  set_service<ConfigurationService>(CONFIGURATION, op);
}

void
i3ds::Camera::get_camera_configuration(CameraConfiguration& config) const
{
  get_sensor_configuration(config.config_sensor);

  config.config_exposure = exposure();
  config.config_gain = gain();
  config.config_auto_exposure = auto_exposure();
  config.config_exposure_limit = exposure_limit();
  config.config_gain_limit = gain_limit();
  config.config_region = region();
  config.config_flash_illumination = flash_enabled();
  config.config_flash_strength = flash_strength();
  config.config_pattern_illumination = pattern_enabled();
  config.config_illumination_pattern = illumination_pattern();
}

ResultCode
i3ds::Camera::execute_camera_command(CameraCommand& command)
{
  switch (command.kind)
    {
    case CameraCommand::CameraCommand_sensor_command_PRESENT:
      return execute_sensor_command(command.u.sensor_command);
    case CameraCommand::set_exposure_PRESENT:
      return set_exposure(command.u.set_exposure);
    case CameraCommand::set_gain_PRESENT:
      return set_gain(command.u.set_gain);
    case CameraCommand::set_auto_exposure_PRESENT:
      return set_auto_exposure(command.u.set_auto_exposure);
    case CameraCommand::set_exposure_limit_PRESENT:
      return set_exposure_limit(command.u.set_exposure_limit);
    case CameraCommand::set_gain_limit_PRESENT:
      return set_gain_limit(command.u.set_gain_limit);
    case CameraCommand::CameraCommand_set_region_PRESENT:
      return set_region(command.u.set_region);
    case CameraCommand::set_flash_illumination_PRESENT:
      return set_flash_enabled(command.u.set_flash_illumination);
    case CameraCommand::set_flash_strength_PRESENT:
      return set_flash_strength(command.u.set_flash_strength);
    case CameraCommand::set_pattern_illumination_PRESENT:
      return set_pattern_enabled(command.u.set_pattern_illumination);
    case CameraCommand::set_illumination_pattern_PRESENT:
      return set_illumination_pattern(command.u.set_illumination_pattern);
    default:
      break;
    }

  return error_unsupported;
}

void
i3ds::Camera::handle_configuration_query(ConfigurationService::Data& config) const
{
  get_camera_configuration(config.response);
}

void
i3ds::Camera::handle_command(CommandService::Data& command)
{
  command.response.result = execute_camera_command(command.request);
}
