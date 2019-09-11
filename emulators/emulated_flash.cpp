///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/emulated_flash.hpp>

i3ds::EmulatedFlash::EmulatedFlash(NodeID node)
  : Flash(node)
{
  BOOST_LOG_TRIVIAL(info) << "Create emulated flash with NodeID: " << node;

  strength_ = 0;
  duration_ = 0;
}

i3ds::EmulatedFlash::~EmulatedFlash()
{
  BOOST_LOG_TRIVIAL(info) << "Destroy emulated flash with NodeID: " << node();
}

void
i3ds::EmulatedFlash::handle_flash(FlashService::Data& command)
{
  strength_ = command.request.strength;
  duration_ = command.request.duration;

  BOOST_LOG_TRIVIAL(info) << "Flash strength = " << strength_ << " duration = " << duration_;
}
