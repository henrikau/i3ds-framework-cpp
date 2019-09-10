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
#include <vector>
#include <unistd.h>
#include <atomic>

#include <boost/program_options.hpp>

#include <i3ds/communication.hpp>
#include <i3ds/server.hpp>
#include <i3ds/emulated_flash.hpp>
#include <i3ds/configurator.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

std::atomic<bool> running;

void signal_handler(int)
{
  BOOST_LOG_TRIVIAL(info) << "SHUTDOWN";
  running = false;
}

int main(int argc, char** argv)
{
  unsigned int node;

  po::options_description desc("Allowed flash emulator options");
  i3ds::Configurator configurator;

  configurator.add_common_options(desc);
  desc.add_options()
  ("node,n",    po::value(&node)->default_value(20), "Node ID of flash emulator")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  BOOST_LOG_TRIVIAL(trace) << "Create context";
  i3ds::Context::Ptr context = i3ds::Context::Create();;

  BOOST_LOG_TRIVIAL(trace) << "Create server";
  i3ds::Server server(context);

  BOOST_LOG_TRIVIAL(trace) << "Create flash";
  i3ds::EmulatedFlash flash(node);

  flash.Attach(server);

  running = true;
  signal(SIGINT, signal_handler);

  server.Start();

  while(running)
    {
      sleep(1);
    }

  server.Stop();

  return 0;
}
