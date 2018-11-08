///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_FLASH_HPP
#define __I3DS_FLASH_HPP

#include <i3ds/Flash.h>

#include <i3ds/node.hpp>
#include <i3ds/server.hpp>
#include <i3ds/service.hpp>
#include <i3ds/codec.hpp>

namespace i3ds
{

CODEC(FlashSetup);

class Flash : public Node
{
public:

  // Shared pointer for flash interfaces.
  typedef std::shared_ptr<Flash> Ptr;

  // Flash services.
  typedef Command<1, FlashSetupCodec> FlashService;

  // Constructor for flash.
  Flash(NodeID node) : Node(node) {};

  // Destructor for flash.
  virtual ~Flash() {};

  // Attach handlers to the server.
  virtual void Attach(Server& server);

protected:

  // Handler for flash setup command, must be overloaded.
  virtual void handle_flash(FlashService::Data& command) = 0;
};

} // namespace i3ds

#endif
