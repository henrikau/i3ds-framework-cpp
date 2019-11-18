///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/pose_estimator_sensor.hpp>
#include <i3ds/exception.hpp>

void
i3ds::PoseEstimator::Attach(Server& server)
{
  using std::placeholders::_1;

  Sensor::Attach(server);

  server.Attach<ImagingModeService>(node(), std::bind(&i3ds::PoseEstimator::handle_imaging_mode, this, _1));
  server.Attach<CameraSelectService>(node(), std::bind(&i3ds::PoseEstimator::handle_camera_select, this, _1));
  server.Attach<ConfigurationService>(node(), std::bind(&i3ds::PoseEstimator::handle_configuration, this, _1));
}

void
i3ds::PoseEstimator::handle_imaging_mode(ImagingModeService::Data&)
{
  throw CommandError(error_unsupported, "Imaging mode not supported");
}

void
i3ds::PoseEstimator::handle_camera_select(CameraSelectService::Data&)
{
  throw CommandError(error_unsupported, "Camera select not supported");
}

void
i3ds::PoseEstimator::handle_configuration(ConfigurationService::Data& config)
{
  check_active();
  try
    {
      config.response.imaging_mode = imaging_mode();
      config.response.camera_select = selected_camera();
    }
  catch (DeviceError& e)
    {
      set_failure();
      throw CommandError(error_other, e.what());
    }
  catch (...)
    {
      /// \todo This forces it into failure state, but the first error message give wrong message on client side.
      set_failure();
      throw CommandError(error_other, "Probably lost connection. Going to failure mode.");
    }
}
