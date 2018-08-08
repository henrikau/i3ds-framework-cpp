///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POWER_HPP
#define __I3DS_POWER_HPP

#include <i3ds/Power.h>

#include <i3ds/server.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>

namespace i3ds
{

CODEC(PowerMask);

class Power
{
public:

  // Shared pointer for trigger interfaces.
  typedef std::shared_ptr<Power> Ptr;

  // Trigger services.
  typedef Command<1, PowerMaskCodec>         ChannelsEnableService;
  typedef Command<2, PowerMaskCodec>         ChannelsDisableService;
  typedef Command<3, PowerMaskCodec>         ChannelsSetService;

  // Constructor for trigger.
  Power(NodeID node) : node_(node) {};

  // Destructor for trigger.
  virtual ~Power() {};

  // Get the node ID.
  inline NodeID node() const {return node_;}

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for channel enable command, must be overloaded.
  virtual void handle_enable_channels(ChannelsEnableService::Data& command) = 0;

  // Handler for channel disable command, must be overloaded.
  virtual void handle_disable_channels(ChannelsDisableService::Data& command) = 0;

  // Handler for channel set command, must be overloaded.
  virtual void handle_set_channels(ChannelsSetService::Data& command) = 0;

private:

  NodeID node_;
};

} // namespace i3ds

#endif
