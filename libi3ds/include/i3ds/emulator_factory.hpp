///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATOR_FACTORY_HPP
#define __I3DS_EMULATOR_FACTORY_HPP

#include <i3ds/communication.hpp>
#include <i3ds/sensor.hpp>
#include <i3ds/radar_sensor.hpp>
#include <i3ds/lidar_sensor.hpp>
#include <i3ds/imu_sensor.hpp>
#include <i3ds/analog_sensor.hpp>
#include <i3ds/star_tracker_sensor.hpp>
#include <i3ds/trigger.hpp>
#include <i3ds/flash.hpp>

#if CAMERA_EMULATORS
#include <i3ds/camera_sensor.hpp>
#include <i3ds/tof_camera_sensor.hpp>
#endif


namespace i3ds
{

class EmulatorFactory
{
public:

  EmulatorFactory(Context::Ptr context, NodeID base_id);
  virtual ~EmulatorFactory();

  Radar::Ptr CreateRadar();
  LIDAR::Ptr CreateLIDAR();
  IMU::Ptr CreateIMU();
  StarTracker::Ptr CreateStarTracker();
  Analog::Ptr CreateTactile();
  Analog::Ptr CreateForceTorque();
  Trigger::Ptr CreateTrigger();
  Flash::Ptr CreateFlash();

#if CAMERA_EMULATORS
  Camera::Ptr CreateTIRCamera(std::string sample_dir = "");
  Camera::Ptr CreateHRCamera(std::string sample_dir = "");
  Camera::Ptr CreateStereoCamera(std::string sample_dir = "");
  ToFCamera::Ptr CreateToFCamera();
#endif

private:

  Context::Ptr context_;
  NodeID next_id_;
};

} // namspace i3ds

#endif
