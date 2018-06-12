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
#include <functional>
#include <csignal>
#include <chrono>

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

void
write_msg_to_file(std::ofstream &output_file, long long delay, const i3ds::Message &msg)
{
  output_file.write((char*)&delay, sizeof(long long));
  output_file.write((char*)msg.data(), msg.size());
}

int
main(int argc, char *argv[])
{
  unsigned int node_id;
  unsigned int endpoint_id;
  unsigned int n_messages;
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
  i3ds::Message msg;

  std::ofstream output_file(file_name, std::ios::binary);

  running = true;
  signal(SIGINT, signal_handler);

  long long delay = 0;
  long long current_msg_time;
  long long prev_msg_time;
  output_file.write((char*)&node_id, sizeof(unsigned int));
  output_file.write((char*)&endpoint_id, sizeof(unsigned int));
  bool first_message = true;
  unsigned int messages_received = 0;
  while(running)
    {
      subscriber->Receive(msg);
      current_msg_time = get_current_time_in_us();
      if (first_message)
        {
          size_t msg_size = msg.size();
          output_file.write((char*)&msg_size, sizeof(size_t));
          first_message = false;
        }
      else
        {
          delay = current_msg_time - prev_msg_time;;
        }
      BOOST_LOG_TRIVIAL(trace) << "Receviced message after: " << delay << " microseconds";
      write_msg_to_file(output_file, delay, msg);
      messages_received++;
      if (n_messages > 0 && messages_received >= n_messages)
        {
          break;
        }
      prev_msg_time = current_msg_time;
    }

  output_file.close();

  BOOST_LOG_TRIVIAL(info) << "Stopping. Received " << messages_received << " messages.";
  BOOST_LOG_TRIVIAL(info) << "Output written to " << file_name;

  return 0;
}
