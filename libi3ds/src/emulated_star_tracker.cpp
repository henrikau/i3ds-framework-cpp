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

#include <i3ds/emulated_star_tracker.hpp>

i3ds::EmulatedStarTracker::EmulatedStarTracker(Context::Ptr context, NodeID node)
  : StarTracker(node),
    sampler_(std::bind(&i3ds::EmulatedStarTracker::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  StarTrackerMeasurementCodec::Initialize(frame_);
}

i3ds::EmulatedStarTracker::~EmulatedStarTracker()
{
}

void
i3ds::EmulatedStarTracker::do_activate()
{
}

void
i3ds::EmulatedStarTracker::do_start()
{
  sampler_.Start(rate());
}

void
i3ds::EmulatedStarTracker::do_stop()
{
  sampler_.Stop();
}

void
i3ds::EmulatedStarTracker::do_deactivate()
{
}

bool
i3ds::EmulatedStarTracker::is_rate_supported(SampleRate rate)
{
  return 0 < rate && rate <= 10000000;
}

bool
i3ds::EmulatedStarTracker::send_sample(unsigned long timestamp_us)
{
  std::cout << "Send: " << timestamp_us << std::endl;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.position.nCount = 4;
  frame_.position.arr[0] = 1.0;
  frame_.position.arr[1] = 2.0;
  frame_.position.arr[2] = 3.0;
  frame_.position.arr[3] = 4.0;

  publisher_.Send<EmulatedStarTrackerMeasurement>(frame_);

  return true;
}
