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

#include <i3ds/configurator.hpp>
#include <i3ds/sensor_client.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
i3ds::Configurator::add_common_options(po::options_description& desc)
{
  desc.add_options()
  ("help,h",    "Show help")
  ("verbose,v", "Verbose output")
  ("quiet,q",   "Quiet output")
    ;
}

po::variables_map
i3ds::Configurator::parse_common_options(po::options_description desc, int argc, char *argv[])
{
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help")) {
      std::cout << desc << std::endl;
      exit(0);
  }

  if (vm.count("quiet")) {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
  } else if (!vm.count("verbose")) {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
  }

  try {
      po::notify(vm);
  } catch (std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      exit(-1);
  }
  return vm;
}


void
i3ds::SensorConfigurator::add_common_options(po::options_description& desc)
{
  i3ds::Configurator::add_common_options(desc);
  desc.add_options()
  ("node,n", po::value<NodeID>(&node_id)->required(), "Node ID")

  ("activate",   "Activate the sensor")
  ("start",      "Start the sensor")
  ("stop",       "Stop the sensor")
  ("deactivate", "Deactivate the sensor")

  ("period", po::value<SamplePeriod>(&period), "Sensor period in microseconds")
  ;
}

void
i3ds::SensorConfigurator::handle_sensor_commands(po::variables_map& vm, i3ds::SensorClient& client)
{
  if (vm.count("activate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command ACTIVATE";
      client.set_state(activate);
    }
  else if (vm.count("stop"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command STOP";
      client.set_state(stop);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
  else if (vm.count("deactivate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command DEACTIVATE";
      client.set_state(deactivate);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
  else if (vm.count("period"))
    {
      BOOST_LOG_TRIVIAL(info) << "Set period: " << period << " [us]";
      client.set_sampling(period);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
  else if (vm.count("start"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command START";
      client.set_state(start);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
}

void
i3ds::SensorConfigurator::print_sensor_status(i3ds::SensorClient& client)
{
  client.load_all();

  int state = client.state();
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
  std::cout << "\"device_name\" : \"" << client.device_name() << "\"," << std::endl;
  std::cout << "\"node\" : " << client.node() << "," << std::endl;
  std::cout << "\"state\" : \"" << state_string << "\"," << std::endl;
  std::cout << "\"temperature\" : " << client.temperature().kelvin << "," << std::endl;
  std::cout << "\"period\" : " << client.period() << "," << std::endl;
  std::cout << "\"batch-size\" : " << client.batch_size() << "," << std::endl;
  std::cout << "\"batch-count\" : " << client.batch_count() << "," << std::endl;
}
