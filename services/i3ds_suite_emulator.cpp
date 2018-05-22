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

#include <boost/program_options.hpp>

#include <i3ds/communication.hpp>
#include <i3ds/emulator_factory.hpp>

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
  std::vector<i3ds::Sensor::Ptr> sensors;

  po::options_description desc("Allowed suite emulator options");

  desc.add_options()
  ("help", "Produce this message")
  ("base", po::value(&base_id)->default_value(10), "Base node ID of sensors")
  ("verbose,v", "Print verbose output")
  ("quiet,q", "Quiet output")
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

  po::notify(vm);

  BOOST_LOG_TRIVIAL(trace) << "Create context";
  i3ds::Context::Ptr context = i3ds::Context::Create();;

  BOOST_LOG_TRIVIAL(trace) << "Create server";
  i3ds::Server server(context);

  BOOST_LOG_TRIVIAL(trace) << "Create factory";
  i3ds::EmulatorFactory factory(context, base_id);

  BOOST_LOG_TRIVIAL(trace) << "Create TIR camera";
  sensors.push_back(factory.CreateTIRCamera());

  BOOST_LOG_TRIVIAL(trace) << "Create HR camera";
  sensors.push_back(factory.CreateHRCamera());

  BOOST_LOG_TRIVIAL(trace) << "Create stereo camera";
  sensors.push_back(factory.CreateStereoCamera());

  BOOST_LOG_TRIVIAL(trace) << "Create ToF camera";
  sensors.push_back(factory.CreateToFCamera());

  BOOST_LOG_TRIVIAL(trace) << "Create LIDAR";
  sensors.push_back(factory.CreateLIDAR());

  BOOST_LOG_TRIVIAL(trace) << "Create radar";
  sensors.push_back(factory.CreateRadar());

  BOOST_LOG_TRIVIAL(trace) << "Create star tracker";
  sensors.push_back(factory.CreateStarTracker());

  BOOST_LOG_TRIVIAL(trace) << "Create IMU";
  sensors.push_back(factory.CreateIMU());

  for (auto s : sensors)
    {
      s->Attach(server);
    }

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
