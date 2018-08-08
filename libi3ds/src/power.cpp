///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/power.hpp>

void
i3ds::Power::Attach(Server& server)
{
  using std::placeholders::_1;

  server.Attach<ChannelsEnableService>(node(), std::bind(&i3ds::Power::handle_enable_channels, this, _1));
  server.Attach<ChannelsDisableService>(node(), std::bind(&i3ds::Power::handle_disable_channels, this, _1));
  server.Attach<ChannelsSetService>(node(), std::bind(&i3ds::Power::handle_set_channels, this, _1));
}

