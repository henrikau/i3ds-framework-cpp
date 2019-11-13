///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/pose_estimator_client.hpp>

i3ds::PoseEstimatorClient::PoseEstimatorClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
  PoseEstimator::ConfigurationService::Initialize(config_);
}

void
i3ds::PoseEstimatorClient::load_config()
{
  i3ds::SensorClient::load_config();
  Call<PoseEstimator::ConfigurationService>(config_);
}

void
i3ds::PoseEstimatorClient::set_imaging_mode(bool imaging_mode)
{
  PoseEstimator::ImagingModeService::Data command;
  command.request.imaging_mode = imaging_mode;
  Call<PoseEstimator::ImagingModeService>(command);
}

void
i3ds::PoseEstimatorClient::set_selected_camera(uint8_t camera)
{
  PoseEstimator::CameraSelectService::Data command;
  command.request.camera_select = camera;
  Call<PoseEstimator::CameraSelectService>(command);
}
