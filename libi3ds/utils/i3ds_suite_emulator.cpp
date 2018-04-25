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
#include "i3ds/emulators/emulated_tof_camera.hpp"

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

volatile bool running;

void signal_handler(int signum)
{
  BOOST_LOG_TRIVIAL(info) << "SHUTDOWN";
  running = false;
}

int main(int argc, char** argv)
{
  unsigned int base_id;

  po::options_description desc("Allowed suite emulator options");

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

  BOOST_LOG_TRIVIAL(trace) << "Create context";
  i3ds::Context::Ptr context = i3ds::Context::Create();;

  BOOST_LOG_TRIVIAL(trace) << "Create server";
  i3ds::Server server(context);

  BOOST_LOG_TRIVIAL(trace) << "Create TIR camera";
  i3ds::EmulatedTIRCamera tir_camera(context, base_id++);

  BOOST_LOG_TRIVIAL(trace) << "Create HR camera";
  i3ds::EmulatedHRCamera hr_camera(context, base_id++);

  BOOST_LOG_TRIVIAL(trace) << "Create stereo camera";
  i3ds::EmulatedStereoCamera stereo_camera(context, base_id++);

  BOOST_LOG_TRIVIAL(trace) << "Create ToF camera";
  i3ds::EmulatedToFCamera tof_camera(context, base_id++, 800, 600);

  tir_camera.Attach(server);
  hr_camera.Attach(server);
  stereo_camera.Attach(server);
  tof_camera.Attach(server);

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
