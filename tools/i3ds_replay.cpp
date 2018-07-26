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
#include <fstream>
#include <csignal>
#include <chrono>
#include <thread>
#include <cstdint>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

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
  std::string file_name;

  po::options_description desc("Replays a log file recorded with i3ds_record\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("input,i", po::value<std::string>(&file_name)->required(), "Name of log file")
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

  i3ds::SessionReader recording(file_name);
  NodeID node_id;
  i3ds::MessageRecord r = recording.get_message();

  if (recording.header_found()) {
    BOOST_LOG_TRIVIAL(trace) << "Header file found.";
    node_id = recording.node_id;
  } else {
    BOOST_LOG_TRIVIAL(trace) << "Header file not found.";
    node_id = r.node();
  }

  BOOST_LOG_TRIVIAL(info) << "Replaying messages from node ID " <<  node_id;
  if (recording.has_endpoint_id) {
    BOOST_LOG_TRIVIAL(info) << " with endpoint ID " << recording.endpoint_id;
  }

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr publisher = i3ds::Socket::Publisher(context);
  publisher->Attach(node_id);

  running = true;
  signal(SIGINT, signal_handler);

  uint32_t n_messages = 0;
  uint64_t prev_time = r.timestamp;

  BOOST_LOG_TRIVIAL(trace) << "Loading system...";
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  BOOST_LOG_TRIVIAL(trace) << "Done.";

  try {

    while (running)
    {
      n_messages++;
      if (prev_time < r.timestamp) {
	uint64_t delay = r.timestamp - prev_time;
	std::this_thread::sleep_for(std::chrono::microseconds(delay));
      }
      publisher->Send(*(r.msg));
      BOOST_LOG_TRIVIAL(trace) << "Sent message";
      prev_time = r.timestamp;

      r = recording.get_message();
    }
  } catch (i3ds::exceptions::end_of_file &e) {
  }
  BOOST_LOG_TRIVIAL(info) << "Played back " << n_messages << " messages.";

  return 0;
}
