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

namespace po = boost::program_options;
namespace logging = boost::log;

volatile bool running;

void
signal_handler(int signum)
{
  running = false;
}


uint64_t
get_current_time_in_us()
{
  using namespace std::chrono;
  return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int
main(int argc, char *argv[])
{
  NodeID node_id;
  EndpointID endpoint_id;
  std::string file_name;
  size_t msg_size;

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

  std::ifstream input_file(file_name, std::ios::binary);
  input_file.read((char*)&node_id, sizeof(NodeID));
  input_file.read((char*)&endpoint_id, sizeof(EndpointID));

  BOOST_LOG_TRIVIAL(info) << "Replaying messages from node ID " <<  node_id
                          << " with endpoint ID " << endpoint_id;

  byte* buffer = (byte*)malloc(10000000);

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr publisher = i3ds::Socket::Publisher(context);
  publisher->Attach(node_id);

  running = true;
  signal(SIGINT, signal_handler);

  uint64_t delay;
  input_file.read((char*)&delay, sizeof(uint64_t)); // First delay in the file is 0
  uint64_t start_time;
  uint32_t payloads;
  while(running)
    {
      if (input_file.eof())
        {
          break;
        }
      start_time = get_current_time_in_us();
      i3ds::Message msg;
      msg.set_address(i3ds::Address(node_id, endpoint_id));
      input_file.read((char*)&payloads, sizeof(uint32_t));
      for (uint32_t i = 0; i < payloads; ++i)
        {
          input_file.read((char*)&msg_size, sizeof(size_t));
          input_file.read((char*)buffer, msg_size);
          msg.append_payload(buffer, msg_size);
        }
      publisher->Send(msg);
      BOOST_LOG_TRIVIAL(trace) << "Sent message";
      input_file.read((char*)&delay, sizeof(uint64_t));
      std::this_thread::sleep_for(std::chrono::microseconds(delay-(get_current_time_in_us()-start_time)));
    }

  input_file.close();
  free(buffer);

  return 0;
}
