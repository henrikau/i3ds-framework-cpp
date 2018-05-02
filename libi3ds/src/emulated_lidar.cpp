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

#include "i3ds/emulated_lidar.hpp"

i3ds::EmulatedLIDAR::Ptr
i3ds::EmulatedLIDAR::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedLIDAR>(context, node);
}

i3ds::EmulatedLIDAR::EmulatedLIDAR(Context::Ptr context, NodeID node)
  : LIDAR(node),
    sampler_(std::bind(&i3ds::EmulatedLIDAR::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  region_.size_x = 640;
  region_.size_y = 480;
  region_.offset_x = 0;
  region_.offset_y = 0;

  LIDARMeasurement100KCodec::Initialize(frame_);

  frame_.region.size_x = region_.size_x;
  frame_.region.size_y = region_.size_y;
}

i3ds::EmulatedLIDAR::~EmulatedLIDAR()
{
}

void
i3ds::EmulatedLIDAR::do_activate()
{
}

void
i3ds::EmulatedLIDAR::do_start()
{
  sampler_.Start(rate());
}

void
i3ds::EmulatedLIDAR::do_stop()
{
  sampler_.Stop();
}

void
i3ds::EmulatedLIDAR::do_deactivate()
{
}

bool
i3ds::EmulatedLIDAR::is_rate_supported(SampleRate rate)
{
  return 0 < rate && rate <= 10000000;
}

void
i3ds::EmulatedLIDAR::handle_region(RegionService::Data& command)
{
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

bool
i3ds::EmulatedLIDAR::send_sample(unsigned long timestamp_us)
{
  std::cout << "Send: " << timestamp_us << std::endl;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  publisher_.Send<LIDARMeasurement>(frame_);

  return true;
}
