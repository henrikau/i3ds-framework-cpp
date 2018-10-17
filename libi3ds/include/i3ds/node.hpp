///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_NODE_HPP
#define __I3DS_NODE_HPP

#include <memory>

#include <i3ds/Common.h>
#include <i3ds/server.hpp>

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Base class for all I3DS nodes
////////////////////////////////////////////////////////////////////////////////

class Node
{
public:

  typedef std::shared_ptr<Node> Ptr;

  Node(NodeID node) : node_(node) {};
  virtual ~Node() {};

  // Get the node ID.
  inline NodeID node() const {return node_;}

  // Attach service handlers to the server.
  virtual void Attach(Server& server) = 0;

private:

  const NodeID node_;
};

} // namespace i3ds

#endif
