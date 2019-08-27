///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_TOF_CAMERA_HPP
#define __I3DS_EMULATED_TOF_CAMERA_HPP

#include <memory>
#include <random>

#include <i3ds/topic.hpp>
#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/tof_camera_sensor.hpp>

namespace i3ds
{

class EmulatedToFCamera : public ToFCamera
{
public:

  typedef std::shared_ptr<EmulatedToFCamera> Ptr;

  static Ptr Create(Context::Ptr context, NodeID node);

  EmulatedToFCamera(Context::Ptr context, NodeID node);
  virtual ~EmulatedToFCamera();

  // Getters.
  virtual bool region_enabled() const {return region_enabled_;}
  virtual PlanarRegion region() const {return region_;}
  virtual double range_min_depth() const {return min_depth_;}
  virtual double range_max_depth() const {return max_depth_;}

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

  // Handlers.
  virtual void handle_region(RegionService::Data& command);
  virtual void handle_range(RangeService::Data& command);

private:

  bool send_sample(unsigned long timestamp_us);

  bool region_enabled_;
  PlanarRegion region_;

  float min_depth_, max_depth_;

  Sampler sampler_;

  Publisher publisher_;
  MeasurementTopic::Data frame_;

  std::default_random_engine generator_;
};

} // namespace i3ds

#endif
