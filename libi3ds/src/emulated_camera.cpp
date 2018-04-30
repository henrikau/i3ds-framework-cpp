///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "i3ds/emulated_camera.hpp"

i3ds::EmulatedCamera::EmulatedCamera(Context::Ptr context, NodeID node, FrameProperties prop)
  : Camera(node),
    prop_(prop),
    sampler_(std::bind(&i3ds::EmulatedCamera::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated camera...";

  shutter_ = 0;
  gain_ = 0.0;

  auto_exposure_enabled_ = false;
  max_shutter_ = 0;
  max_gain_ = 0.0;

  region_.size_x = prop.width;
  region_.size_y = prop.height;
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
  BOOST_LOG_TRIVIAL(info) << "do_activate() " << node();
}

void
i3ds::EmulatedCamera::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "do_start() " << node();
  sampler_.Start(rate());
}

void
i3ds::EmulatedCamera::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "do_stop() " << node();
  sampler_.Stop();
}

void
i3ds::EmulatedCamera::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "do_deactivate() " << node();
}

bool
i3ds::EmulatedCamera::is_rate_supported(SampleRate rate)
{
  BOOST_LOG_TRIVIAL(info) << "is_rate_supported() " << node();
  return 0 < rate && rate <= 10000000;
}

void
i3ds::EmulatedCamera::handle_exposure(ExposureService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "handle_exposure() " << node();
  auto_exposure_enabled_ = false;
  shutter_ = command.request.shutter;
  gain_ = command.request.gain;
}

void
i3ds::EmulatedCamera::handle_auto_exposure(AutoExposureService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "handle_auto_exposure() " << node();
  auto_exposure_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      max_shutter_ = command.request.max_shutter;
      max_gain_ = command.request.max_gain;
    }
}

void
i3ds::EmulatedCamera::handle_region(RegionService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "handle_region() " << node();
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

void
i3ds::EmulatedCamera::handle_flash(FlashService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "handle_flash() " << node();
  flash_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      flash_strength_ = command.request.strength;
    }
}

void
i3ds::EmulatedCamera::handle_pattern(PatternService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "do_pattern() " << node();
  pattern_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      pattern_sequence_ = command.request.sequence;
    }
}
