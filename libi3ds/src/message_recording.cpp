///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <chrono>
#include <fstream>

#include <i3ds/time.hpp>
#include <i3ds/message_recording.hpp>

void
i3ds::MessageRecord::write_to_file(std::ofstream &output_file)
{
  uint32_t payloads = msg->payloads();
  output_file.write((char*)&delay, sizeof(uint64_t));
  output_file.write((char*)&payloads, sizeof(uint32_t));
  for (uint32_t i = 0; i < payloads; ++i)
    {
      size_t msg_size = msg->size(i);
      output_file.write((char*)(&msg_size), sizeof(size_t));
      output_file.write((char*)(msg->data(i)), msg_size);
    }
}

void
i3ds::SessionRecording::write_to_file(std::string file_name)
{
  std::ofstream output_file(file_name, std::ios::binary);
  output_file.write((char*)&node_id, sizeof(NodeID));
  output_file.write((char*)&endpoint_id, sizeof(EndpointID));
  for (MessageRecord r : records)
    {
      r.write_to_file(output_file);
    }
  output_file.close();
}

void
i3ds::SessionRecording::load_from_file(std::string file_name)
{
  std::ifstream input_file(file_name, std::ios::binary);
  input_file.read((char*)&node_id, sizeof(NodeID));
  input_file.read((char*)&endpoint_id, sizeof(EndpointID));
  while (!input_file.eof())
    {
      MessageRecord record;
      record.msg->set_address(i3ds::Address(node_id, endpoint_id));
      uint32_t payloads;
      size_t msg_size;
      input_file.read((char*)&record.delay, sizeof(uint64_t));
      input_file.read((char*)&payloads, sizeof(uint32_t));
      for (uint32_t i = 0; i < payloads; ++i)
        {
          input_file.read((char*)(&msg_size), sizeof(size_t));
          byte* buffer = (byte*)malloc(msg_size);
          input_file.read((char*)buffer, msg_size);
          record.msg->append_payload(buffer, msg_size, false);
        }
      records.push_back(record);
    }
  input_file.close();
}
