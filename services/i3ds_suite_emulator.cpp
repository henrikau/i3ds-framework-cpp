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

void signal_handler(int)
{
  BOOST_LOG_TRIVIAL(info) << "SHUTDOWN";
  running = false;
}

int main(int argc, char** argv)
{
  unsigned int base_id;
  std::string sample_image_dir;
  std::vector<i3ds::Sensor::Ptr> sensors;
  std::vector<i3ds::Node::Ptr> nodes;

  po::options_description desc("Allowed suite emulator options");

  desc.add_options()
  ("help", "Produce this message")
  ("base", po::value(&base_id)->default_value(10), "Base node ID of sensors")
  ("samples", po::value(&sample_image_dir)->default_value(""), "Directory with sample images for HR and stereo camera")
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

  // Need two servers due to nested service calls, one for sensors and one for trigger etc.
  BOOST_LOG_TRIVIAL(trace) << "Create servers";
  i3ds::Server server_1(context);
  i3ds::Server server_2(context);

  BOOST_LOG_TRIVIAL(trace) << "Create factory";
  i3ds::EmulatorFactory factory(context, base_id);

  BOOST_LOG_TRIVIAL(trace) << "Create trigger";
  nodes.push_back(factory.CreateTrigger());

  BOOST_LOG_TRIVIAL(trace) << "Create flash";
  nodes.push_back(factory.CreateFlash());

#if CAMERA_EMULATORS
  BOOST_LOG_TRIVIAL(trace) << "Create TIR camera";
  sensors.push_back(factory.CreateTIRCamera());

  BOOST_LOG_TRIVIAL(trace) << "Create HR camera";
  sensors.push_back(factory.CreateHRCamera(sample_image_dir));

  BOOST_LOG_TRIVIAL(trace) << "Create stereo camera";
  sensors.push_back(factory.CreateStereoCamera(sample_image_dir));

  BOOST_LOG_TRIVIAL(trace) << "Create ToF camera";
  sensors.push_back(factory.CreateToFCamera());
#endif

  BOOST_LOG_TRIVIAL(trace) << "Create LIDAR";
  sensors.push_back(factory.CreateLIDAR());

  BOOST_LOG_TRIVIAL(trace) << "Create radar";
  sensors.push_back(factory.CreateRadar());

  BOOST_LOG_TRIVIAL(trace) << "Create star tracker";
  sensors.push_back(factory.CreateStarTracker());

  BOOST_LOG_TRIVIAL(trace) << "Create IMU";
  sensors.push_back(factory.CreateIMU());

  BOOST_LOG_TRIVIAL(trace) << "Create tactile sensor";
  sensors.push_back(factory.CreateTactile());

  BOOST_LOG_TRIVIAL(trace) << "Create force/torque sensor";
  sensors.push_back(factory.CreateForceTorque());

  for (auto s : sensors)
    {
      BOOST_LOG_TRIVIAL(trace) << "Attach sensor " << s->node() << " to server 1";
      s->Attach(server_1);
    }

  for (auto n : nodes)
    {
      BOOST_LOG_TRIVIAL(trace) << "Attach node " << n->node() << " to server 2";
      n->Attach(server_2);
    }

  running = true;
  signal(SIGINT, signal_handler);

  BOOST_LOG_TRIVIAL(trace) << "Starting servers...";

  server_1.Start();
  server_2.Start();

  BOOST_LOG_TRIVIAL(trace) << "Servers running!";

  while(running)
    {
      sleep(1);
    }

  BOOST_LOG_TRIVIAL(trace) << "Stopping servers...";

  server_1.Stop();
  server_2.Stop();

  BOOST_LOG_TRIVIAL(trace) << "Servers running!";

  return 0;
}
