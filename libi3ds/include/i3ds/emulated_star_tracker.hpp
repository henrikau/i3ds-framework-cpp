///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_STAR_TRACKER_HPP
#define __I3DS_EMULATED_STAR_TRACKER_HPP

#include <i3ds/topic.hpp>
#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/star_tracker_sensor.hpp>

namespace i3ds
{

class EmulatedStarTracker : public StarTracker
{
public:

  typedef std::shared_ptr<EmulatedStarTracker> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id)
    {
      return std::make_shared<EmulatedStarTracker>(context, id);
    }

  EmulatedStarTracker(Context::Ptr context, NodeID id);
  virtual ~EmulatedStarTracker();

  // Supported rate.
  virtual bool is_rate_supported(SampleRate rate);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  bool send_sample(unsigned long timestamp_us);

  Sampler sampler_;

  Publisher publisher_;
  StarTrackerMeasurement frame_;
};

} // namespace i3ds

#endif
