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


long long
get_current_time_in_us()
{
  using namespace std::chrono;
  return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int
main(int argc, char *argv[])
{
  unsigned int node_id;
  unsigned int endpoint_id;
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
  input_file.read((char*)&node_id, sizeof(unsigned int));
  input_file.read((char*)&endpoint_id, sizeof(unsigned int));
  input_file.read((char*)&msg_size, sizeof(size_t));

  BOOST_LOG_TRIVIAL(info) << "Replaying messages of size " << msg_size
                          << " from node ID " <<  node_id
                          << " with endpoint ID " << endpoint_id;

  byte* buffer = (byte*)malloc(msg_size);

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Socket::Ptr publisher = i3ds::Socket::Publisher(context);
  publisher->Attach(node_id);

  running = true;
  signal(SIGINT, signal_handler);

  long long delay;
  input_file.read((char*)&delay, sizeof(long long)); // First delay in the file is 0
  long long start_time;
  while(running)
    {
      if (input_file.eof())
        {
          break;
        }
      start_time = get_current_time_in_us();
      i3ds::Message msg;
      msg.set_address(i3ds::Address(node_id, endpoint_id));
      input_file.read((char*)buffer, msg_size);
      // FIXME: Handle multipart messages.
      msg.append_payload(buffer, msg_size);
      publisher->Send(msg);
      BOOST_LOG_TRIVIAL(trace) << "Sent message";
      // PROBLEM: message is not received in other end until after sleep
      input_file.read((char*)&delay, sizeof(long long));
      std::this_thread::sleep_for(std::chrono::microseconds(delay-(get_current_time_in_us()-start_time)));
    }

  input_file.close();
  free(buffer);

  return 0;
}
