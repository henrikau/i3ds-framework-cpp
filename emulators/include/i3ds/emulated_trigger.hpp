///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_TRIGGER_HPP
#define __I3DS_EMULATED_TRIGGER_HPP

#include <memory>
#include <vector>

#include <i3ds/trigger.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

namespace i3ds
{

class EmulatedTrigger : public Trigger
{
public:

  typedef std::shared_ptr<EmulatedTrigger> Ptr;

  static Ptr Create(NodeID id)
  {
    return std::make_shared<EmulatedTrigger>(id);
  }

  EmulatedTrigger(NodeID id);
  ~EmulatedTrigger();

  TriggerPeriod period(TriggerGenerator generator) const;
  TriggerGenerator source(TriggerOutput channel) const;
  TriggerDuration duration(TriggerOutput channel) const;
  TriggerOffset offset(TriggerOutput channel) const;
  bool inverted(TriggerOutput channel) const;
  bool enabled(TriggerOutput channel) const;

protected:

  // Handler for trigger generator command, must be overloaded.
  virtual void handle_generator(GeneratorService::Data& command);

  // Handler for trigger internal channel command, must be overloaded.
  virtual void handle_internal_channel(InternalChannelService::Data& command);

  // Handler for trigger external channel command, must be overloaded.
  virtual void handle_external_channel(ExternalChannelService::Data& command);

  // Handler for channel enable command, must be overloaded.
  virtual void handle_enable_channel(ChannelEnableService::Data& command);

  // Handler for channel disable command, must be overloaded.
  virtual void handle_disable_channel(ChannelDisableService::Data& command);

private:

  std::vector<TriggerPeriod> generators_;
  std::vector<ChannelInternal> channels_;
  std::vector<bool> enabled_;
};

} // namespace i3ds

#endif
