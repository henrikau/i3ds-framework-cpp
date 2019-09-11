///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_RADAR_HPP
#define __I3DS_EMULATED_RADAR_HPP

#include <memory>
#include <random>

#include <i3ds/topic.hpp>
#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/radar_sensor.hpp>

namespace i3ds
{

class EmulatedRadar : public Radar
{
public:

  typedef std::shared_ptr<EmulatedRadar> Ptr;

  static Ptr Create(Context::Ptr context, NodeID node);

  EmulatedRadar(Context::Ptr context, NodeID node);
  virtual ~EmulatedRadar();

  // Getters.
  virtual bool region_enabled() const {return region_enabled_;}
  virtual PlanarRegion region() const {return region_;}

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

private:

  bool send_sample(unsigned long timestamp_us);

  bool region_enabled_;
  PlanarRegion region_;

  Sampler sampler_;

  Publisher publisher_;
  MeasurementTopic::Data frame_;

  std::default_random_engine generator_;
};

} // namespace i3ds

#endif
