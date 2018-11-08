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
#include <exception>

/*** Message formats
 * Data package:
 * -----------------------------------
 * | START     | MESSAGE_TYPE_DATA   |
 * | TIMESTAMP | N_PAYLOADS          |
 * -----------------------------------
 * | PAYLOAD_1 SIZE | PAYLOAD_1 DATA |
 * | PAYLOAD_2 SIZE | PAYLOAD_2 DATA |
 * |               ...               |
 * | PAYLOAD_N SIZE | PAYLOAD_N DATA |
 * -----------------------------------
 *
 * Header package:
 * ---------------------------------------
 * | START           | MESSAGE_TYPE_HEAD |
 * | START_TIME      | END_TIME          |
 * | N_NODE_IDs      | NODE_IDs: 1 - N   |
 * | HAS_ENDPOINT_ID | ENDPOINT_ID       |
 * | MESSAGE_COUNT   |                   |
 * ---------------------------------------
 ***/

namespace i3ds
{
namespace exceptions
{
struct end_of_file : public std::exception
{
  const char * what () const throw ()
  {
    return "End of file reached.";
  }
};
} // namespace i3ds::exceptions

const uint32_t I3DS_MESSAGE_START = 0x53443349; // hex('I3DS')
const char I3DS_HEADER_PACKAGE = 'H';
const char I3DS_DATA_PACKAGE   = 'D';


struct MessageRecord
{
  uint64_t timestamp;
  std::shared_ptr<Message> msg;
  bool was_loaded = false;
  MessageRecord() : timestamp(0), msg(std::make_shared<Message>()) {}

  void store(std::ofstream &output_file);
  void load(std::ifstream &input_file);

  NodeID node()
  {
    return msg->node();
  }
};

class SessionRecording
{
public:
  SessionRecording(std::string filename);
  ~SessionRecording()
  {
    output_file.close();
  }
  void store(MessageRecord &record);
  void store_header();
  uint32_t message_count()
  {
    return _message_count;
  }
  std::vector<NodeID> node_ids;
  EndpointID endpoint_id;
  bool endpoint_id_set = false;
  uint64_t start_time;
  uint64_t end_time;

private:
  uint32_t _message_count;

  std::ofstream output_file;

  std::string filename;
};

class SessionReader
{
public:
  SessionReader(std::string filename);
  ~SessionReader()
  {
    input_file.close();
  }
  MessageRecord get_message();
  bool has_endpoint_id;
  bool endpoint_id_set;
  std::vector<NodeID> node_ids;
  bool _header_found;
  uint64_t start_time;
  uint64_t end_time;
  bool header_found()
  {
    return _header_found;
  }
  EndpointID endpoint_id;

private:
  std::ifstream input_file;
  uint32_t _message_count;
};
} // namespace i3ds

#endif
