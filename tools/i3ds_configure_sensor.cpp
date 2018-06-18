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

#include <i3ds/sensor_client.hpp>
#include <i3ds/sensor_configurator.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;


int main(int argc, char *argv[])
{
  i3ds::SensorConfigurator configurator;

  po::options_description desc("Allowed sensor control options");

  configurator.add_common_options(desc);
  desc.add_options()
  ("print", "Print the sensor configuration")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to sensor with node ID: " << configurator.node_id;
  i3ds::SensorClient sensor(context, configurator.node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  configurator.handle_sensor_commands(vm, sensor);

  // Print config, this is the final command.
  if (vm.count("print"))
    {
      configurator.print_sensor_status(sensor);
    }

  return 0;
}
