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

#include <i3ds/address_server.hpp>
#include <i3ds/configurator.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

std::atomic<bool> running;

void signal_handler(int)
{
  running = false;
}

int main(int argc, char *argv[])
{
  std::string filename;
  int port;

  po::options_description desc("Run a service for querying zmq addresses for nodes on the network\n  Available options");
  i3ds::Configurator configurator;

  configurator.add_common_options(desc);
  desc.add_options()
  ("config,c", po::value<std::string>(&filename)->default_value(""), "Config file with addresses")
  ("port,p",   po::value<int>(&port)->default_value(i3ds::AddressServer::DEFAULT_PORT), "Port to bind service to")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

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
