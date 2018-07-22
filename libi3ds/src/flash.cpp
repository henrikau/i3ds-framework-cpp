///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/flash.hpp>

void
i3ds::Flash::Attach(Server& server)
{
  using std::placeholders::_1;

  server.Attach<FlashService>(node(), std::bind(&i3ds::Flash::handle_flash, this, _1));
}
