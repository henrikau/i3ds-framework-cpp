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

#include <i3ds/emulated_radar.hpp>
#include <i3ds/emulated_lidar.hpp>
#include <i3ds/emulated_star_tracker.hpp>
#include <i3ds/emulated_imu.hpp>
#include <i3ds/emulated_analog.hpp>
#include <i3ds/emulated_trigger.hpp>
#include <i3ds/emulated_flash.hpp>
#include <i3ds/emulated_pose_estimator.hpp>

#if CAMERA_EMULATORS
#include <i3ds/emulated_camera.hpp>
#include <i3ds/emulated_tof_camera.hpp>
#endif


i3ds::EmulatorFactory::EmulatorFactory(Context::Ptr context, NodeID base_id)
  : context_(context),
    next_id_(base_id),
    trigger_id_(0),
    flash_id_(0)
{
}

i3ds::EmulatorFactory::~EmulatorFactory()
{
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

i3ds::Analog::Ptr
i3ds::EmulatorFactory::CreateTactile()
{
  return EmulatedAnalog::CreateTactile(context_, next_id_++);
}

i3ds::Analog::Ptr
i3ds::EmulatorFactory::CreateForceTorque()
{
  return EmulatedAnalog::CreateForceTorque(context_, next_id_++);
}

i3ds::PoseEstimator::Ptr
i3ds::EmulatorFactory::CreatePoseEstimator()
{
  return std::make_shared<EmulatedPoseEstimator>(context_, next_id_++);
}

i3ds::Trigger::Ptr
i3ds::EmulatorFactory::CreateTrigger()
{
  trigger_id_ = next_id_++;
  return EmulatedTrigger::Create(trigger_id_);
}

i3ds::Flash::Ptr
i3ds::EmulatorFactory::CreateFlash()
{
  flash_id_ = next_id_++;
  return EmulatedFlash::Create(flash_id_);
}

#if CAMERA_EMULATORS
i3ds::ToFCamera::Ptr
i3ds::EmulatorFactory::CreateToFCamera()
{
  return std::make_shared<EmulatedToFCamera>(context_, next_id_++);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateTIRCamera(std::string sample_dir)
{
  EmulatedCamera::Parameters param;

  param.frame_mode = mode_mono;
  param.data_depth = 16;
  param.pixel_size = 2;
  param.width = 640;
  param.height = 480;
  param.image_count = 1;
  param.sample_dir = sample_dir;

  param.external_trigger = trigger_id_ > 0;
  param.trigger_node = trigger_id_;
  param.trigger_source = 1;
  param.camera_output = 1;
  param.camera_offset = 0;

  param.support_flash = false;
  param.support_pattern = false;

  return std::make_shared<EmulatedCamera>(context_, next_id_++, param);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateHRCamera(std::string sample_dir)
{
  EmulatedCamera::Parameters param;

  param.frame_mode = mode_mono;
  param.data_depth = 12;
  param.pixel_size = 2;
  param.width = 2048;
  param.height = 2048;
  param.image_count = 1;
  param.sample_dir = sample_dir;

  param.external_trigger = trigger_id_ > 0;
  param.trigger_node = trigger_id_;
  param.trigger_source = 1;
  param.camera_output = 2;
  param.camera_offset = 1000;

  param.support_flash = flash_id_ > 0;
  param.flash_node = flash_id_;
  param.flash_output = 5;
  param.flash_offset = 500;

  param.support_pattern = true;
  param.pattern_output = 4;
  param.pattern_offset = 0;

  return std::make_shared<EmulatedCamera>(context_, next_id_++, param);
}

i3ds::Camera::Ptr
i3ds::EmulatorFactory::CreateStereoCamera(std::string sample_dir)
{
  EmulatedCamera::Parameters param;

  param.frame_mode = mode_mono;
  param.data_depth = 12;
  param.pixel_size = 2;
  param.width = 2048;
  param.height = 2048;
  param.image_count = 2;
  param.sample_dir = sample_dir;

  param.external_trigger = trigger_id_ > 0;
  param.trigger_node = trigger_id_;
  param.trigger_source = 1;
  param.camera_output = 3;
  param.camera_offset = 2000;

  param.support_flash = flash_id_ > 0;
  param.flash_node = flash_id_;
  param.flash_output = 5;
  param.flash_offset = 0;

  param.support_pattern = false;

  return std::make_shared<EmulatedCamera>(context_, next_id_++, param);
}
#endif
