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

  i3ds::SessionRecording recording;
  recording.load_from_file(file_name);

  BOOST_LOG_TRIVIAL(info) << "Replaying messages from node ID " <<  recording.node_id
                          << " with endpoint ID " << recording.endpoint_id;

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr publisher = i3ds::Socket::Publisher(context);
  publisher->Attach(recording.node_id);

  for (i3ds::MessageRecord r : recording.records)
    {
      std::this_thread::sleep_for(std::chrono::microseconds(r.delay));
      publisher->Send(*(r.msg));
      BOOST_LOG_TRIVIAL(trace) << "Sent message";
    }

  return 0;
}
