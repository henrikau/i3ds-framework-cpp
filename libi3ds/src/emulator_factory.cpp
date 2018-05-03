///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/emulator_factory.hpp>

#include <i3ds/emulated_camera.hpp>
#include <i3ds/emulated_tof_camera.hpp>
#include <i3ds/emulated_radar.hpp>
#include <i3ds/emulated_lidar.hpp>
#include <i3ds/emulated_star_tracker.hpp>
#include <i3ds/emulated_imu.hpp>

i3ds::EmulatorFactory::EmulatorFactory(Context::Ptr context, NodeID base_id)
  : context_(context),
    next_id_(base_id)
{
}

i3ds::EmulatorFactory::~EmulatorFactory()
{
}

i3ds::ToFCamera::Ptr
i3ds::EmulatorFactory::CreateToFCamera()
{
  return std::make_shared<EmulatedToFCamera>(context_, next_id_++);
}

i3ds::Radar::Ptr
i3ds::EmulatorFactory::CreateRadar()
{
  return std::make_shared<EmulatedRadar>(context_, next_id_++);
}

i3ds::LIDAR::Ptr
i3ds::EmulatorFactory::CreateLIDAR()
{
  return std::make_shared<EmulatedLIDAR>(context_, next_id_++);
}

i3ds::StarTracker::Ptr
i3ds::EmulatorFactory::CreateStarTracker()
{
  return std::make_shared<EmulatedStarTracker>(context_, next_id_++);
}

i3ds::IMU::Ptr
i3ds::EmulatorFactory::CreateIMU()
{
  return std::make_shared<EmulatedIMU>(context_, next_id_++);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateTIRCamera()
{
  FrameProperties prop;

  prop.mode = mode_mono;
  prop.data_depth = 16;
  prop.pixel_size = 2;
  prop.width = 640;
  prop.height = 480;

  return std::make_shared<EmulatedMonoCamera<CameraMeasurement8MCodec> >(context_, next_id_++, prop);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateHRCamera()
{
  FrameProperties prop;

  prop.mode = mode_mono;
  prop.data_depth = 12;
  prop.pixel_size = 2;
  prop.width = 2048;
  prop.height = 2048;

  return std::make_shared<EmulatedMonoCamera<CameraMeasurement8MCodec> >(context_, next_id_++, prop);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateStereoCamera()
{
  FrameProperties prop;

  prop.mode = mode_mono;
  prop.data_depth = 12;
  prop.pixel_size = 2;
  prop.width = 2048;
  prop.height = 2048;

  return std::make_shared<EmulatedStereoCamera<StereoCameraMeasurement8MCodec> >(context_, next_id_++, prop);
}
