///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_TRIGGER_HPP
#define __I3DS_TRIGGER_HPP

#include <i3ds/Trigger.h>

#include <i3ds/server.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>

namespace i3ds
{

CODEC(GeneratorSetup);
CODEC(ChannelInternal);
CODEC(ChannelExternal);
CODEC(TriggerMask);

class Trigger
{
public:

  // Shared pointer for trigger interfaces.
  typedef std::shared_ptr<Trigger> Ptr;

  // Trigger services.
  typedef Command<1, GeneratorSetupCodec>      GeneratorService;
  typedef Command<2, ChannelInternalCodec>     InternalChannelService;
  typedef Command<3, ChannelExternalCodec>     ExternalChannelService;
  typedef Command<4, TriggerMaskCodec>         ChannelEnableService;
  typedef Command<5, TriggerMaskCodec>         ChannelDisableService;

  // Constructor for trigger.
  Trigger(NodeID node) : node_(node) {};

  // Destructor for trigger.
  virtual ~Trigger() {};

  // Get the node ID.
  inline NodeID node() const {return node_;}

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for trigger generator command, must be overloaded.
  virtual void handle_generator(GeneratorService::Data& command) = 0;

  // Handler for trigger internal channel command, must be overloaded.
  virtual void handle_internal_channel(InternalChannelService::Data& command) = 0;

  // Handler for trigger external channel command, must be overloaded.
  virtual void handle_external_channel(ExternalChannelService::Data& command) = 0;

  // Handler for channel enable command, must be overloaded.
  virtual void handle_enable_channel(ChannelEnableService::Data& command) = 0;

  // Handler for channel disable command, must be overloaded.
  virtual void handle_disable_channel(ChannelDisableService::Data& command) = 0;

private:

  NodeID node_;
};

} // namespace i3ds

#endif
