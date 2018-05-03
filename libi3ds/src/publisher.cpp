///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <thread>

#include <i3ds/publisher.hpp>

i3ds::Publisher::Publisher(Context::Ptr context, NodeID node)
  : node_(node),
    context_(context)
{
}

i3ds::Publisher::~Publisher()
{
}

void i3ds::Publisher::Reset()
{
  socket_ = Socket::Publisher(context_);
  socket_->Attach(node_);
}
