///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <i3ds/power_client.hpp>

i3ds::PowerClient::PowerClient(Context::Ptr context, NodeID node)
  : Client(context, node)
{
}

void
i3ds::PowerClient::enable_channels(const PowerOutputSet& channels)
{
  Power::ChannelsEnableService::Data command;

  Power::ChannelsEnableService::Initialize(command);

  int code = 0;

  for (PowerOutput c : channels)
    {
      if (PowerOutput_IsConstraintValid(&c, &code))
        {
          command.request.arr[c - 1] = true;
        }
      else
        {
          throw i3ds::CommandError(error_value, "Invalid output channel " + std::to_string(c));
        }
    }

  Call<Power::ChannelsEnableService>(command);
}

void
i3ds::PowerClient::disable_channels(const PowerOutputSet& channels)
{
  Power::ChannelsDisableService::Data command;

  Power::ChannelsDisableService::Initialize(command);

  int code = 0;

  for (PowerOutput c : channels)
    {
      if (PowerOutput_IsConstraintValid(&c, &code))
        {
          command.request.arr[c - 1] = true;
        }
      else
        {
          throw i3ds::CommandError(error_value, "Invalid output channel " + std::to_string(c));
        }
    }

  Call<Power::ChannelsDisableService>(command);
}

void
i3ds::PowerClient::set_channels(const PowerOutputSet& channels)
{
  Power::ChannelsSetService::Data command;

  Power::ChannelsSetService::Initialize(command);

  int code = 0;

  for (PowerOutput c : channels)
    {
      if (PowerOutput_IsConstraintValid(&c, &code))
        {
          command.request.arr[c - 1] = true;
        }
      else
        {
          throw i3ds::CommandError(error_value, "Invalid output channel " + std::to_string(c));
        }
    }

  Call<Power::ChannelsSetService>(command);
}
