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
}

ResultCode
i3ds::Camera::execute_camera_command(CameraCommand& command)
{
  return error_unsupported;
}

void
i3ds::Camera::handle_configuration_query(ConfigurationService::Data& config) const
{
}

void
i3ds::Camera::handle_command(CommandService::Data& command)
{
}
