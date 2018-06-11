///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/trigger.hpp>

void
i3ds::Trigger::Attach(Server& server)
{
  using std::placeholders::_1;

  server.Attach<GeneratorService>(node(), std::bind(&i3ds::Trigger::handle_generator, this, _1));
  server.Attach<InternalChannelService>(node(), std::bind(&i3ds::Trigger::handle_internal_channel, this, _1));
  server.Attach<ExternalChannelService>(node(), std::bind(&i3ds::Trigger::handle_external_channel, this, _1));
  server.Attach<ChannelEnableService>(node(), std::bind(&i3ds::Trigger::handle_enable_channel, this, _1));
  server.Attach<ChannelDisableService>(node(), std::bind(&i3ds::Trigger::handle_disable_channel, this, _1));
}

