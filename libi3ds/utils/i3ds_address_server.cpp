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

#include "i3ds/utils/address_server.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

volatile bool running;

void signal_handler(int signum)
{
  running = false;
}

int main(int argc, char *argv[])
{
  std::string filename;
  int port;

  po::options_description desc("Allowed manager options");

  desc.add_options()
    ("help", "Produce this message")
    ("config", po::value<std::string>(&filename)->default_value(""), "Config file with addresses")
    ("port", po::value<int>(&port)->default_value(i3ds::AddressServer::DEFAULT_PORT), "Port to bind service to")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  running = true;
  signal(SIGINT, signal_handler);

  i3ds::AddressServer srv(filename, port);

  srv.Start();

  while(running)
    {
      sleep(1);
    }

  srv.Stop();

  return 0;
}
