///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>

#include <i3ds/emulated_star_tracker.hpp>

i3ds::EmulatedStarTracker::EmulatedStarTracker(Context::Ptr context, NodeID node)
  : StarTracker(node),
    sampler_(std::bind(&i3ds::EmulatedStarTracker::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated star tracker with NodeID: " << node;
  StarTrackerMeasurementCodec::Initialize(frame_);
}

i3ds::EmulatedStarTracker::~EmulatedStarTracker()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated star tracker with NodeID: " << node();
}

void
i3ds::EmulatedStarTracker::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated star tracker with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedStarTracker::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated star tracker with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedStarTracker::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated star tracker with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedStarTracker::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated star tracker with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedStarTracker::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated star tracker with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

bool
i3ds::EmulatedStarTracker::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated star tracker with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  frame_.position.nCount = 4;
  frame_.position.arr[0] = 1.0;
  frame_.position.arr[1] = 2.0;
  frame_.position.arr[2] = 3.0;
  frame_.position.arr[3] = 4.0;

  publisher_.Send<MeasurementTopic>(frame_);

  return true;
}
