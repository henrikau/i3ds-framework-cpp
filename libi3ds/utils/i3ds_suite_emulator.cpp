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

#include <boost/program_options.hpp>

#include "i3ds/core/communication.hpp"
#include "i3ds/emulators/emulated_camera.hpp"

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>





namespace po = boost::program_options;
namespace logging = boost::log;




volatile bool running;

void signal_handler(int signum)
{
  BOOST_LOG_TRIVIAL(info) << "do_deactivate()";
  running = false;
}

int main(int argc, char** argv)
{
  unsigned int base_id;

  po::options_description desc("Allowed camera control options");

  desc.add_options()
  ("help", "Produce this message")
  ("base", po::value(&base_id)->default_value(10), "Base node ID of sensors")
  ("verbose,v", "Print verbose output")
  ("quite,q", "Quiet ouput")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  if (vm.count("quite"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
    }
  else if (!vm.count("verbose"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }

  po::notify(vm);

  i3ds::Context::Ptr context = i3ds::Context::Create();;

  i3ds::Server server(context);
  i3ds::EmulatedCamera camera(context, base_id + 0, 800, 600);

  camera.Attach(server);

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
