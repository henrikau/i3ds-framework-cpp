///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "emulated_camera.hpp"


i3ds::EmulatedCamera::EmulatedCamera(Context::Ptr context, NodeID id, int resx, int resy)
  : Camera(context, id),
    resx_(resx),
    resy_(resy)
{
  exposure_ = 0.0;
  gain_ = 1.0;
  auto_exposure_ = false;
  exposure_limit_ = 0.0;
  gain_limit_ = 1.0;

  region_.size_x = resx;
  region_.size_y = resy;
  region_.offset_x = 0;
  region_.offset_y = 0;

  flash_enabled_ = false;
  flash_strength_ = 0.0;

  pattern_enabled_ = false;
  pattern_ = 0.0;
}

i3ds::EmulatedCamera::~EmulatedCamera()
{
}

void
i3ds::EmulatedCamera::do_activate()
{
}

void
i3ds::EmulatedCamera::do_start()
{
}

void
i3ds::EmulatedCamera::do_stop()
{
}

void
i3ds::EmulatedCamera::do_deactivate()
{
}

bool
i3ds::EmulatedCamera::support_rate(SensorRate rate)
{
  return 0.0 < rate && rate <= 10.0;
}

ResultCode
i3ds::EmulatedCamera::set_exposure(ExposureTime exposure)
{
  exposure_ = exposure;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_gain(SensorGain gain)
{
  gain_ = gain;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_auto_exposure(bool auto_exposure)
{
  auto_exposure_ = auto_exposure;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_exposure_limit(ExposureTime exposure_limit)
{
  exposure_limit_ = exposure_limit;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_gain_limit(SensorGain gain_limit)
{
  gain_limit_ = gain_limit;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_region(PlanarRegion region)
{
  region_ = region;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_flash_enabled(bool flash_enabled)
{
  flash_enabled_ = flash_enabled;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_flash_strength(FlashStrength flash_strength)
{
  flash_strength_ = flash_strength;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_pattern_enabled(bool pattern_enabled)
{
  pattern_enabled_ = pattern_enabled;
  return success;
}

ResultCode
i3ds::EmulatedCamera::set_illumination_pattern(IlluminationPattern pattern)
{
  pattern_ = pattern;
  return success;
}
