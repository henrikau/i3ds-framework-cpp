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

#include "i3ds/emulators/emulated_camera.hpp"

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

i3ds::EmulatedCamera::EmulatedCamera(Context::Ptr context, NodeID node, int resx, int resy)
  : Camera(node),
    resx_(resx),
    resy_(resy),
    sampler_(std::bind(&i3ds::EmulatedCamera::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated camera...";

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
  exposure_ = command.request.exposure;
  gain_ = command.request.gain;
}

void
i3ds::EmulatedCamera::handle_auto_exposure(AutoExposureService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "handle_auto_exposure() " << node();
  auto_exposure_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      max_exposure_ = command.request.max_exposure;
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

i3ds::Camera::Ptr
i3ds::EmulatedTIRCamera::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedTIRCamera>(context, node);
}

i3ds::EmulatedTIRCamera::EmulatedTIRCamera(Context::Ptr context, NodeID node)
  : EmulatedCamera(context, node, 640, 480)
{
  ImageMeasurement::Codec::Initialize(frame_);

  frame_.frame_mode = mode_mono;
  frame_.data_depth = 16;
  frame_.pixel_size = 2;
  frame_.region.size_x = resx_;
  frame_.region.size_y = resy_;
  frame_.image.nCount = resx_ * resy_ * frame_.pixel_size;

  BOOST_LOG_TRIVIAL(info) << "TIR image size: "
                          << frame_.image.nCount / 1024.0
                          << " KiB";
}

i3ds::Camera::Ptr
i3ds::EmulatedHRCamera::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedHRCamera>(context, node);
}

i3ds::EmulatedHRCamera::EmulatedHRCamera(Context::Ptr context, NodeID node)
  : EmulatedCamera(context, node, 2048, 2048)
{
  ImageMeasurement::Codec::Initialize(frame_);

  frame_.frame_mode = mode_mono;
  frame_.data_depth = 12;
  frame_.pixel_size = 2;
  frame_.region.size_x = resx_;
  frame_.region.size_y = resy_;
  frame_.image.nCount = resx_ * resy_ * frame_.pixel_size;

  BOOST_LOG_TRIVIAL(info) << "HR image size: "
                          << frame_.image.nCount / 1024.0
                          << " KiB";
}

i3ds::Camera::Ptr
i3ds::EmulatedStereoCamera::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedStereoCamera>(context, node);
}

i3ds::EmulatedStereoCamera::EmulatedStereoCamera(Context::Ptr context, NodeID node)
  : EmulatedCamera(context, node, 2048, 2048)
{
  ImageMeasurement::Codec::Initialize(frame_);

  frame_.frame_mode = mode_mono;
  frame_.data_depth = 12;
  frame_.pixel_size = 2;
  frame_.region.size_x = resx_;
  frame_.region.size_y = resy_;
  frame_.image_left.nCount = resx_ * resy_ * frame_.pixel_size;
  frame_.image_right.nCount = resx_ * resy_ * frame_.pixel_size;

  BOOST_LOG_TRIVIAL(info) << "Stereo image size: "
                          << 2 * frame_.image_left.nCount / 1024.0
                          << " KiB";
}

bool
i3ds::EmulatedTIRCamera::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Send TIR sample " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  publisher_.Send<ImageMeasurement>(frame_);

  return true;
}

bool
i3ds::EmulatedHRCamera::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Send HR sample " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  publisher_.Send<ImageMeasurement>(frame_);

  return true;
}

bool
i3ds::EmulatedStereoCamera::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Send stereo sample " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  publisher_.Send<ImageMeasurement>(frame_);

  return true;
}
