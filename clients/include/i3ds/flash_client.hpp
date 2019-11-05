///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_FLASH_CLIENT_HPP
#define __I3DS_FLASH_CLIENT_HPP

#include <i3ds/client.hpp>
#include <i3ds/flash.hpp>

namespace i3ds
{

class FlashClient : public Client
{
public:

  typedef std::shared_ptr<FlashClient> Ptr;

  FlashClient(Context::Ptr context, NodeID node);

  void set_flash(ShutterTime duration, FlashStrength strength);
};

} // namespace i3ds

#endif
