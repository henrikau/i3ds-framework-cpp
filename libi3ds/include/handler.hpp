///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_HANDLER_HPP
#define __I3DS_HANDLER_HPP

#include <memory>

#include "communication.hpp"

namespace i3ds
{

class Handler
{
public:

  typedef std::unique_ptr<Handler> Ptr;

  virtual ~Handler() {};

  virtual void handle(const Message& request, Message& response) = 0;
};

} // namespace i3ds

#endif
