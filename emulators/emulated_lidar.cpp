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

#include <i3ds/emulated_lidar.hpp>

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
  BOOST_LOG_TRIVIAL(info) << "Create emulated LIDAR with NodeID: " << node;

  region_.size_x = 640;
  region_.size_y = 480;
  region_.offset_x = 0;
  region_.offset_y = 0;

  MeasurementTopic::Codec::Initialize(frame_);

  frame_.descriptor.width = 200000;
  frame_.descriptor.height = 1;

  set_device_name("Emulated LIDAR");
}

i3ds::EmulatedLIDAR::~EmulatedLIDAR()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated LIDAR with NodeID: " << node();
}

void
i3ds::EmulatedLIDAR::do_activate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " do_activate()";
}

void
i3ds::EmulatedLIDAR::do_start()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " do_start()";
  sampler_.Start(period());
}

void
i3ds::EmulatedLIDAR::do_stop()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " do_stop()";
  sampler_.Stop();
}

void
i3ds::EmulatedLIDAR::do_deactivate()
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " do_deactivate()";
}

bool
i3ds::EmulatedLIDAR::is_sampling_supported(SampleCommand sample)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " is_period_supported()";
  return sample.batch_size == 1 && (0 < sample.period && sample.period <= 10000000);
}

void
i3ds::EmulatedLIDAR::handle_region(RegionService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Emulated LIDAR with NodeID: " << node() << " handle_region()";
  region_enabled_ = command.request.enable;

  if (command.request.enable)
    {
      region_ = command.request.region;
    }
}

bool
i3ds::EmulatedLIDAR::send_sample(unsigned long timestamp_us)
{
  BOOST_LOG_TRIVIAL(trace) << "Emulated LIDAR with NodeID: " << node() << " sends sample at " << timestamp_us;

  frame_.descriptor.attributes.timestamp = timestamp_us;
  frame_.descriptor.attributes.validity = sample_valid;

  std::normal_distribution<float> x(100.0, 10.0);
  std::normal_distribution<float> y(0.0, 5.0);
  std::normal_distribution<float> z(0.0, 10.0);

  for (unsigned int i = 0; i < frame_.descriptor.width; i++)
    {
      PointXYZ p;

      p.x = x(generator_);
      p.y = y(generator_);
      p.z = z(generator_);

      frame_.points.push_back(p);
    }

  publisher_.Send<MeasurementTopic>(frame_);

  frame_.points.clear();

  return true;
}
