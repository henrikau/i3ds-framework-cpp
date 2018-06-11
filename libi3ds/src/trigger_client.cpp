///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/trigger_client.hpp>

i3ds::TriggerClient::TriggerClient(Context::Ptr context, NodeID node)
  : Client(context, node)
{
}

void
i3ds::TriggerClient::set_generator(TriggerGenerator generator, TriggerPeriod period)
{
  Trigger::GeneratorService::Data command;

  command.request.generator = generator;
  command.request.period = period;

  Call<Trigger::GeneratorService>(command);
}

void
i3ds::TriggerClient::set_interal_channel(TriggerOutput channel, TriggerGenerator source, TriggerOffset offset,
    TriggerDuration duration)
{
  Trigger::InternalChannelService::Data command;

  command.request.channel = channel;
  command.request.source = source;
  command.request.offset = offset;
  command.request.duration = duration;

  Call<Trigger::InternalChannelService>(command);
}

void
i3ds::TriggerClient::set_exteral_channel(TriggerOutput channel, TriggerInput source, TriggerOffset offset,
    TriggerDuration duration,
    bool bypass, bool invert)
{
  Trigger::ExternalChannelService::Data command;

  command.request.channel = channel;
  command.request.source = source;
  command.request.offset = offset;
  command.request.duration = duration;
  command.request.bypass = bypass;
  command.request.invert = invert;

  Call<Trigger::ExternalChannelService>(command);
}

void
i3ds::TriggerClient::enable_channels(TriggerMask channels)
{
  Trigger::ChannelEnableService::Data command;

  command.request = channels;

  Call<Trigger::ChannelEnableService>(command);
}

void
i3ds::TriggerClient::disable_channels(TriggerMask channels)
{
  Trigger::ChannelDisableService::Data command;

  command.request = channels;

  Call<Trigger::ChannelDisableService>(command);
}

