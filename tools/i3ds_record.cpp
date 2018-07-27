///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <functional>
#include <csignal>
#include <chrono>
#include <cstdint>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include <i3ds/time.hpp>
#include <i3ds/communication.hpp>
#include <i3ds/message_recording.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

volatile bool running;

void
signal_handler(int signum)
{
  running = false;
}


int
main(int argc, char *argv[])
{
  std::vector<NodeID> node_ids;
  EndpointID endpoint_id;
  uint32_t n_messages;
  std::string file_name;

  po::options_description desc("Records measurements and stores them in a log file\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value(&node_ids)->required(), "Node ID of sensor")
  ("endpoint,e", po::value(&endpoint_id), "Endpoint ID of measurement. Subscribes to all messages if not specified")
  ("messages,m", po::value(&n_messages)->default_value(0), "Number of messages to record. 0 means no limit.")
  ("output,o", po::value<std::string>(&file_name)->default_value("out.log"), "File name to write output.")
  ("verbose,v", "Print verbose output")
  ("quiet,q", "Quiet ouput")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  if (vm.count("quiet"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
    }
  else if (!vm.count("verbose"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }

  try
    {
      po::notify(vm);
    }
  catch (std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
    }

  BOOST_LOG_TRIVIAL(trace) << "Recording messages from node IDs: ";
  for ( auto node_id : node_ids) {
    BOOST_LOG_TRIVIAL(trace) << "  " <<  node_id;
  }

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr subscriber = i3ds::Socket::Subscriber(context);

  for ( auto node_id : node_ids) {
    BOOST_LOG_TRIVIAL(info) << "Subscribing to node " << node_id;
    subscriber->Attach(node_id);
  }

  i3ds::SessionRecording recording(file_name);
  recording.node_ids = node_ids;

  if (vm.count("endpoint") && vm.count("node") == 1) {
    BOOST_LOG_TRIVIAL(info) <<  "Subscribing to endpoint ID " << endpoint_id << ".";
    subscriber->Filter(i3ds::Address(node_ids[0], endpoint_id));
    recording.endpoint_id = endpoint_id;
    recording.endpoint_id_set = true;
  } else {
    BOOST_LOG_TRIVIAL(info) <<  "Subscribing to all endpoints.";
    subscriber->FilterAll();
    recording.endpoint_id_set = false;
  }


  running = true;
  signal(SIGINT, signal_handler);

  i3ds::MessageRecord record;
  BOOST_LOG_TRIVIAL(trace) << "Waiting for messages.";
  try {
    subscriber->Receive(*(record.msg));
    record.timestamp = i3ds::get_timestamp();
    recording.start_time = record.timestamp;
    while(running)
    {

      BOOST_LOG_TRIVIAL(trace) << "Receviced message at: " << record.timestamp << " microseconds";
      BOOST_LOG_TRIVIAL(trace) << "message has: " << record.msg->payloads() << " paylods";
      recording.store(record);
      if (n_messages > 0 && recording.message_count() >= n_messages)
      {
	break;
      }
      subscriber->Receive(*(record.msg));
      record.timestamp = i3ds::get_timestamp();
    }
  } catch(zmq::error_t &ex) {
    BOOST_LOG_TRIVIAL(info) << "Received a zmq error: " << ex.what();
  }
  recording.end_time = record.timestamp;

  BOOST_LOG_TRIVIAL(info) << "Stopping. Received " << recording.message_count() << " messages.";

  recording.store_header();
  BOOST_LOG_TRIVIAL(info) << "Output written to " << file_name;
  BOOST_LOG_TRIVIAL(info) << "  with header " << file_name << ".hdr";

  return 0;
}
