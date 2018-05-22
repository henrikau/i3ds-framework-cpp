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
#include <cstdlib>

#include <i3ds/sensor_client.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_sensor_settings(i3ds::SensorClient *sensor)
{
  sensor->load_status();

  int state = sensor->state();
  std::string state_string;
  switch (state)
    {
    case inactive:
      state_string = "inactive";
      break;
    case standby:
      state_string = "standby";
      break;
    case operational:
      state_string = "operational";
      break;
    case failure:
      state_string = "failure";
      break;
    default:
      state_string = "UNKNOWN STATE";
    }

  std::cout << "{" << std::endl;
  std::cout << "\"node\" : " << sensor->node() << "," << std::endl;
  std::cout << "\"state\" : \"" << state_string << "\"," << std::endl;
  std::cout << "\"temperature\" : " << sensor->temperature().kelvin << "," << std::endl;
  std::cout << "}" << std::endl;
}

int main(int argc, char *argv[])
{
  unsigned int node_id;
  unsigned int rate;

  po::options_description desc("Allowed sensor control options");

  desc.add_options()
  ("help", "Produce this message")
  ("node", po::value<unsigned int>(&node_id)->required(), "Node ID of sensor")

  ("activate", "Activate the sensor")
  ("start", "Start the sensor")
  ("stop", "Stop the sensor")
  ("deactivate", "Deactivate the sensor")
  ("rate", po::value(&rate), "Sensor rate in microseconds")

  ("verbose,v", "Print verbose output")
  ("quite,q", "Quiet ouput")
  ("print", "Print the sensor configuration")
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

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to sensor with node ID: " << node_id;
  i3ds::SensorClient sensor(context, node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  // Can either command activate, allow more commands.
  if (vm.count("activate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command ACTIVATE";
      sensor.set_state(activate);
    }

  // Command stop, allow more commands.
  if (vm.count("stop"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command STOP";
      sensor.set_state(stop);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Command deactivate, no more commands.
  if (vm.count("deactivate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command DEACTIVATE";
      sensor.set_state(deactivate);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
      return 0;
    }

  // Set rate, allow further commands.
  if (vm.count("rate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Set rate: " << rate << " [us]";
      sensor.set_rate(rate);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Start sensor, allow further commands.
  if (vm.count("start"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command START";
      sensor.set_state(start);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Print config, this is the final command.
  if (vm.count("print"))
    {
      print_sensor_settings(&sensor);
    }

  return 0;
}
