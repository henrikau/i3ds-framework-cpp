///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/emulated_trigger.hpp>

i3ds::EmulatedTrigger::EmulatedTrigger(NodeID id)
  : Trigger(id),
    generators_(4),
    channels_(8),
    enabled_(8)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated trigger with NodeID: " << node();

  for (int i = 0; i < 4; i++)
    {
      generators_[i] = 0;
    }

  for (int i = 0; i < 8; i++)
    {
      channels_[i].source = 1;
      channels_[i].duration = 0;
      channels_[i].offset = 0;
      channels_[i].invert = 0;
      enabled_[i] = false;
    }
}

i3ds::EmulatedTrigger::~EmulatedTrigger()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated trigger with NodeID: " << node();
}

TriggerPeriod
i3ds::EmulatedTrigger::period(TriggerGenerator generator) const
{
  return generators_[generator - 1];
}

TriggerGenerator
i3ds::EmulatedTrigger::source(TriggerOutput channel) const
{
  return channels_[channel - 1].source;
}

TriggerDuration
i3ds::EmulatedTrigger::duration(TriggerOutput channel) const
{
  return channels_[channel - 1].duration;
}

TriggerOffset
i3ds::EmulatedTrigger::offset(TriggerOutput channel) const
{
  return channels_[channel - 1].offset;
}

bool
i3ds::EmulatedTrigger::inverted(TriggerOutput channel) const
{
  return channels_[channel - 1].invert;
}

bool
i3ds::EmulatedTrigger::enabled(TriggerOutput channel) const
{
  return enabled_[channel - 1];
}


void
i3ds::EmulatedTrigger::handle_generator(GeneratorService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Handle generator " << command.request.generator
                          << " period " << command.request.period << " us";

  generators_[command.request.generator - 1] = command.request.period;
}

void
i3ds::EmulatedTrigger::handle_internal_channel(InternalChannelService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) << "Handle internal channel " << command.request.channel;

  channels_[command.request.channel - 1] = command.request;
}

// Handler for trigger external channel command, must be overloaded.
void
i3ds::EmulatedTrigger::handle_external_channel(ExternalChannelService::Data& command)
{
  BOOST_LOG_TRIVIAL(error) << "Handle external channel not implemented";
}

void
i3ds::EmulatedTrigger::handle_enable_channel(ChannelEnableService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) <<  "Enabling channels: ";

  for (int i = 0; i < 8; i++)
    {
      if (command.request.arr[i])
        {
          BOOST_LOG_TRIVIAL(info) << "   " << i + 1;
          enabled_[i] = true;
        }
    }
}

void
i3ds::EmulatedTrigger::handle_disable_channel(ChannelDisableService::Data& command)
{
  BOOST_LOG_TRIVIAL(info) <<  "Disabling channels: ";

  for (int i = 0; i < 8; i++)
    {
      if (command.request.arr[i])
        {
          BOOST_LOG_TRIVIAL(info) << "   " << i + 1;
          enabled_[i] = false;
        }
    }
}
