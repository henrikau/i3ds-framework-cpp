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

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <i3ds/time.hpp>
#include <i3ds/message_recording.hpp>

void
i3ds::MessageRecord::store(std::ofstream &output_file)
{
  uint32_t payloads = msg->payloads();
  output_file.write((char *)&I3DS_MESSAGE_START, sizeof(uint32_t));
  output_file.write(&I3DS_DATA_PACKAGE, 1);
  i3ds::Address address = msg->address();
  output_file.write((char*)&address.node, sizeof(NodeID));
  output_file.write((char*)&address.endpoint, sizeof(EndpointID));
  output_file.write((char*)&timestamp, sizeof(uint64_t));
  output_file.write((char*)&payloads, sizeof(uint32_t));
  for (uint32_t i = 0; i < payloads; ++i)
    {
      size_t msg_size = msg->size(i);
      output_file.write((char*)(&msg_size), sizeof(size_t));
      output_file.write((char*)(msg->data(i)), msg_size);
    }
}

void
i3ds::MessageRecord::load(std::ifstream &input_file)
{
  //TODO: Synchronize
  uint32_t sync;
  char type;
  uint32_t payloads;
  was_loaded = true;
  if (input_file.eof()) {
    throw i3ds::exceptions::end_of_file();
  }
  input_file.read((char *)&sync, sizeof(sync));
  BOOST_LOG_TRIVIAL(trace) << "Data start: " << sync;
  input_file.read(&type, 1);
  BOOST_LOG_TRIVIAL(trace) << "Type: " << type;
  NodeID node;
  EndpointID endpoint;
  input_file.read((char*)&node, sizeof(NodeID));
  input_file.read((char*)&endpoint, sizeof(EndpointID));
  i3ds::Address address(node, endpoint);
  address.node = node;
  address.endpoint = endpoint;
  msg->set_address(address);

  input_file.read((char*)&timestamp, sizeof(uint64_t));
  input_file.read((char*)&payloads, sizeof(uint32_t));
  BOOST_LOG_TRIVIAL(trace) << "Payloads: " << payloads;

  if (input_file.eof()) {
    throw i3ds::exceptions::end_of_file();
  }

  for (uint32_t i = 0; i < payloads; ++i)
  {
    if (input_file.eof()) {
      throw i3ds::exceptions::end_of_file();
    }

    size_t msg_size;
    input_file.read((char*)(&msg_size), sizeof(size_t));
    // This should be free'd by the zmq message
    byte* buffer = (byte*)malloc(msg_size);
    input_file.read((char*)buffer, msg_size);
    msg->append_payload(buffer, msg_size, false);
  }
}

i3ds::SessionRecording::SessionRecording(std::string filename) :
  output_file(filename, std::ios::binary)
{
  this->filename = filename;
  this->_message_count = 0;
}
void
i3ds::SessionRecording::store(MessageRecord &record)
{
  record.store(this->output_file);
  this->_message_count++;
}

void
i3ds::SessionRecording::store_header()
{
  std::ofstream hdr_file(this->filename + ".hdr");
  hdr_file.write((char*)&I3DS_MESSAGE_START, sizeof(uint32_t));
  hdr_file.write(&I3DS_HEADER_PACKAGE, 1);
  hdr_file.write((char*)&start_time, sizeof(uint64_t));
  hdr_file.write((char*)&end_time, sizeof(uint64_t));
  size_t node_id_size = node_ids.size();
  hdr_file.write((char*)&node_id_size, sizeof(size_t));
  for (NodeID nid : node_ids) {
      hdr_file.write((char*)&nid, sizeof(NodeID));
  }
  hdr_file.write((char*)&endpoint_id_set, sizeof(bool));
  hdr_file.write((char*)&endpoint_id, sizeof(EndpointID));
  hdr_file.write((char*)&_message_count, sizeof(uint32_t));
}


i3ds::SessionReader::SessionReader(std::string filename) : input_file(filename, std::ios::binary)
{
  std::ifstream hdr_file(filename + ".hdr");
  endpoint_id_set = false;
  if (!hdr_file.good()) {
    _header_found = false;
    return;
  }
  _header_found = true;

  //TODO: synchronize
  uint32_t message_start;
  char message_type;
  size_t node_id_count;
  hdr_file.read((char*)&message_start, sizeof(uint32_t));
  BOOST_LOG_TRIVIAL(trace) << "Header start: " << message_start;

  hdr_file.read(&message_type, 1);
  hdr_file.read((char*)&start_time, sizeof(uint64_t));
  hdr_file.read((char*)&end_time, sizeof(uint64_t));
  hdr_file.read((char*)&node_id_count, sizeof(node_id_count));
  node_ids.resize(node_id_count);
  for (size_t i=0; i< node_id_count; i++) {
    hdr_file.read((char*)&node_ids[i], sizeof(NodeID));
  }
  hdr_file.read((char*)&endpoint_id_set, sizeof(bool));
  hdr_file.read((char*)&endpoint_id, sizeof(EndpointID));
  hdr_file.read((char*)&_message_count, sizeof(uint32_t));

  hdr_file.close();
};

i3ds::MessageRecord i3ds::SessionReader::get_message() {
  i3ds::MessageRecord record;
  record.load(input_file);
  return record;
}
