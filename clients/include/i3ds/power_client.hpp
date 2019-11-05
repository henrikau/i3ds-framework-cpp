///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_POWER_CLIENT_HPP
#define __I3DS_POWER_CLIENT_HPP

#include <i3ds/client.hpp>
#include <i3ds/power.hpp>

#include <set>

namespace i3ds
{

typedef std::set<PowerOutput> PowerOutputSet;

class PowerClient : public Client
{
public:

  typedef std::shared_ptr<PowerClient> Ptr;

  PowerClient(Context::Ptr context, NodeID node);

  void enable_channels(const PowerOutputSet& channels);
  void disable_channels(const PowerOutputSet& channels);
  void set_channels(const PowerOutputSet& channels);
};

} // namespace i3ds

#endif
