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

#include <i3ds/emulated_tof_camera.hpp>

i3ds::EmulatedToFCamera::Ptr
i3ds::EmulatedToFCamera::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedToFCamera>(context, node);
}

i3ds::EmulatedToFCamera::EmulatedToFCamera(Context::Ptr context, NodeID node)
  : ToFCamera(node),
    sampler_(std::bind(&i3ds::EmulatedToFCamera::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated ToF camera with NodeID: " << node;

  region_.size_x = 640;
  region_.size_y = 480;
  region_.offset_x = 0;
  region_.offset_y = 0;

  MeasurementTopic::Codec::Initialize(frame_);

  frame_.region.size_x = region_.size_x;
  frame_.region.size_y = region_.size_y;
}

i3ds::EmulatedToFCamera::~EmulatedToFCamera()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated ToF camera with NodeID: " << node();
}

void
i3ds::EmulatedToFCamera::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedToFCamera::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedToFCamera::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedToFCamera::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedToFCamera::is_period_supported(SamplePeriod period)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " is_period_supported()";
  return 0 < period && period <= 10000000;
}

void
i3ds::EmulatedToFCamera::handle_region(RegionService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated ToF camera with NodeID: " << node() << " handle_region()";
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

bool
i3ds::EmulatedToFCamera::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated ToF camera with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;

  publisher_.Send<MeasurementTopic>(frame_);

  return true;
}
