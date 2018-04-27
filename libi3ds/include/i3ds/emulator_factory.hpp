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

#include "communication.hpp"
#include "sensor.hpp"
#include "camera_sensor.hpp"
#include "tof_camera_sensor.hpp"

namespace i3ds
{

class EmulatorFactory
{
public:

  EmulatorFactory(Context::Ptr context, NodeID base_id);
  virtual ~EmulatorFactory();

  ToFCamera::Ptr CreateToFCamera();
  Camera::Ptr CreateTIRCamera();
  Camera::Ptr CreateHRCamera();
  Camera::Ptr CreateStereoCamera();

private:

  Context::Ptr context_;
  NodeID next_id_;
};

} // namspace i3ds

#endif
