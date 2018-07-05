///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_TRIGGER_CLIENT_HPP
#define __I3DS_TRIGGER_CLIENT_HPP

#include <i3ds/client.hpp>
#include <i3ds/trigger.hpp>

#include <set>

namespace i3ds
{

typedef std::set<TriggerOutput> TriggerOutputSet;

class TriggerClient : public Client
{
public:

  typedef std::shared_ptr<TriggerClient> Ptr;

  TriggerClient(Context::Ptr context, NodeID node);

  void set_generator(TriggerGenerator generator, TriggerPeriod period);

  void set_internal_channel(TriggerOutput channel, TriggerGenerator source, TriggerOffset offset,
                            TriggerDuration duration, bool invert);

  void set_external_channel(TriggerOutput channel, TriggerInput source, TriggerOffset offset, TriggerDuration duration,
                            bool bypass, bool invert);

  void enable_channels(const TriggerOutputSet& channels);
  void disable_channels(const TriggerOutputSet& channels);
};

} // namespace i3ds

#endif
