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
  exposure_ = 0;
  gain_ = 0.0;

  auto_exposure_enabled_ = false;
  max_exposure_ = 0;
  max_gain_ = 0.0;

  region_.size_x = resx;
  region_.size_y = resy;
  region_.offset_x = 0;
  region_.offset_y = 0;

  flash_enabled_ = false;
  flash_strength_ = 0.0;

  pattern_enabled_ = false;
  pattern_sequence_ = 0;
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
i3ds::EmulatedCamera::support_rate(SampleRate rate)
{
  return 0 < rate && rate <= 10000000;
}

void
i3ds::EmulatedCamera::set_exposure(ExposureTime exposure, SensorGain gain)
{
  auto_exposure_enabled_ = false;
  exposure_ = exposure;
  gain_ = gain;
}

void
i3ds::EmulatedCamera::set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain)
{
  auto_exposure_enabled_ = enable;

  if (enable)
    {
      max_exposure_ = max_exposure;
      max_gain_ = max_gain;
    }
}

void
i3ds::EmulatedCamera::set_region(bool enable, PlanarRegion region)
{
  region_enabled_ = enable;

  if (enable)
    {
      region_ = region;
    }
}

void
i3ds::EmulatedCamera::set_flash(bool enable, FlashStrength strength)
{
  flash_enabled_ = enable;

  if (enable)
    {
      flash_strength_ = strength;
    }
}

void
i3ds::EmulatedCamera::set_pattern(bool enable, PatternSequence sequence)
{
  pattern_enabled_ = enable;

  if (enable)
    {
      pattern_sequence_ = sequence;
    }
}
