///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <unistd.h>
#include <atomic>

#include <i3ds/measurement_proxy.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

std::atomic<bool> running;

void signal_handler(int)
{
  running = false;
}

int main(int argc, char *argv[])
{
  int sub_port;
  int pub_port;

  po::options_description desc("Run a publish/subscribe proxy\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("sub,s", po::value<int>(&sub_port)->default_value(9001), "Subscribe-port (publishers connect to this)")
  ("pub,p", po::value<int>(&pub_port)->default_value(9002), "Publish-port subscribers connect to this)")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
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

  running = true;
  signal(SIGINT, signal_handler);

  i3ds::MeasurementProxy proxy(sub_port, pub_port);

  proxy.Start();

  while(running)
    {
      sleep(1);
    }

  proxy.Stop();

  return 0;
}
