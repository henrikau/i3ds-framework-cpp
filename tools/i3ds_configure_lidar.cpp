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

#include <i3ds/lidar_client.hpp>
#include <i3ds/configurator.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_lidar_settings(i3ds::LIDARClient& lidar)
{
  lidar.load_all();
  std::cout << "\"region-enabled\" : " << lidar.region_enabled() << "," << std::endl;
  std::cout << "\"region\" : [" << lidar.region().size_x;
  std::cout << "," << lidar.region().size_y;
  std::cout << "," << lidar.region().offset_x;
  std::cout << "," << lidar.region().offset_y << "]" << "," << std::endl;
  std::cout << "}" << std::endl;
}

int main(int argc, char *argv[])
{
  i3ds::SensorConfigurator configurator;
  bool enable_region;
  PolarRegion region;

  po::options_description desc("Allowed LIDAR control options");

  configurator.add_common_options(desc);
  desc.add_options()
  ("print", "Print the LIDAR configuration")

  ("region", po::value(&enable_region), "Enable LIDAR region of interest (ROI). All or no region parameters must be set, or 0 will be used as default.")
  ("region-size-x,X", po::value(&region.size_x)->default_value(0), "ROI horisontal size")
  ("region-size-y,Y", po::value(&region.size_y)->default_value(0), "ROI vertical size")
  ("region-offset-x,x", po::value(&region.offset_x)->default_value(0), "ROI horisontal offset from left")
  ("region-offset-y,y", po::value(&region.offset_y)->default_value(0), "ROI vertical offset from top")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to lidar with node ID: " << configurator.node_id;
  i3ds::LIDARClient lidar(context, configurator.node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  configurator.handle_sensor_commands(vm, lidar);

  // Configure region.
  if (vm.count("region"))
    {
      if (enable_region)
        {
          BOOST_LOG_TRIVIAL(info) << "Enable region: ("
                                  << region.size_x << ","
                                  << region.size_y << ","
                                  << region.offset_x << ","
                                  << region.offset_y << ")";
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable region";
        }

      lidar.set_region(enable_region, region);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Print config.
  if (vm.count("print"))
    {
      configurator.print_sensor_status(lidar);
      print_lidar_settings(lidar);
    }

  return 0;
}
