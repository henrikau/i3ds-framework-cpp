///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_MESSAGE_RECORDING_HPP
#define __I3DS_MESSAGE_RECORDING_HPP 

#include <stdio.h>
#include <fstream>
#include <memory>

#include <i3ds/communication.hpp>

namespace i3ds
{

uint64_t get_current_time_in_us();

struct MessageRecord
{
  uint64_t delay;
  std::shared_ptr<Message> msg;
  MessageRecord() : delay(0), msg(std::make_shared<Message>()) {}

  void write_to_file(std::ofstream &output_file);
};

struct SessionRecording
{
  NodeID node_id;
  EndpointID endpoint_id;
  std::vector<MessageRecord> records;

  void write_to_file(std::string file_name);

  void load_from_file(std::string file_name);
};

} // namespace i3ds

#endif
