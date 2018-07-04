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
  NodeID node_id;
  EndpointID endpoint_id;
  uint32_t n_messages;
  std::string file_name;

  po::options_description desc("Records measurements and stores them in a log file\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value(&node_id)->required(), "Node ID of sensor")
  ("endpoint,e", po::value(&endpoint_id)->required(), "Endpoint ID of measurement")
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

  BOOST_LOG_TRIVIAL(info) << "Recording messages from node ID " <<  node_id
                          << " with endpoint ID " << endpoint_id;

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr subscriber = i3ds::Socket::Subscriber(context);
  subscriber->Attach(node_id);
  subscriber->Filter(i3ds::Address(node_id, endpoint_id));

  i3ds::SessionRecording recording;
  recording.node_id = node_id;
  recording.endpoint_id = endpoint_id;

  running = true;
  signal(SIGINT, signal_handler);

  uint64_t current_msg_time = 0;
  uint64_t prev_msg_time = 0;
  while(running)
    {
      i3ds::MessageRecord record;
      subscriber->Receive(*(record.msg));
      current_msg_time = i3ds::get_timestamp();
      if (!recording.records.empty()) // Keep first message delay 0
        {
          record.delay = current_msg_time - prev_msg_time;;
        }
      BOOST_LOG_TRIVIAL(trace) << "Receviced message after: " << record.delay << " microseconds";
      BOOST_LOG_TRIVIAL(trace) << "message has: " << record.msg->payloads() << " paylods";
      recording.records.push_back(record);
      if (n_messages > 0 && recording.records.size() >= n_messages)
        {
          break;
        }
      prev_msg_time = current_msg_time;
    }

  BOOST_LOG_TRIVIAL(info) << "Stopping. Received " << recording.records.size() << " messages.";

  recording.write_to_file(file_name);
  BOOST_LOG_TRIVIAL(info) << "Output written to " << file_name;

  return 0;
}
