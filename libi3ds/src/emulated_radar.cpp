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

#include <i3ds/emulated_radar.hpp>

i3ds::EmulatedRadar::Ptr
i3ds::EmulatedRadar::Create(Context::Ptr context, NodeID node)
{
  return std::make_shared<EmulatedRadar>(context, node);
}

i3ds::EmulatedRadar::EmulatedRadar(Context::Ptr context, NodeID node)
  : Radar(node),
    sampler_(std::bind(&i3ds::EmulatedRadar::send_sample, this, std::placeholders::_1)),
    publisher_(context, node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated radar with NodeID: " << node;

  region_.size_x = 640;
  region_.size_y = 480;
  region_.offset_x = 0;
  region_.offset_y = 0;

  MeasurementTopic::Codec::Initialize(frame_);

  frame_.region.size_x = region_.size_x;
  frame_.region.size_y = region_.size_y;

  set_device_name("Emulated radar");
}

i3ds::EmulatedRadar::~EmulatedRadar()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated radar with NodeID: " << node();
}

void
i3ds::EmulatedRadar::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedRadar::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedRadar::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedRadar::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedRadar::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

void
i3ds::EmulatedRadar::handle_region(RegionService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated radar with NodeID: " << node() << " handle_region()";
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

bool
i3ds::EmulatedRadar::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated radar with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.attributes.timestamp.microseconds = timestamp_us;
  frame_.attributes.validity = sample_valid;
  frame_.region = region_;

  publisher_.Send<MeasurementTopic>(frame_);

  return true;
}
