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

#include <i3ds/tof_camera_client.hpp>
#include <i3ds/sensor_configurator.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_tof_settings(i3ds::ToFCameraClient& tof)
{
  tof.load_all();
  std::cout << "\"region-enabled\" : " << tof.region_enabled() << "," << std::endl;
  std::cout << "\"region\" : [" << tof.region().size_x;
  std::cout << "," << tof.region().size_y;
  std::cout << "," << tof.region().offset_x;
  std::cout << "," << tof.region().offset_y << "]" << "," << std::endl;
  std::cout << "\"min_depth\" : " << tof.min_depth() << "," << std::endl;
  std::cout << "\"max_depth\" : " << tof.max_depth() << std::endl;
  std::cout << "}" << std::endl;
}

int main(int argc, char *argv[])
{
  i3ds::SensorConfigurator configurator;
  bool enable_region;
  PlanarRegion region;
  double min_depth, max_depth;

  po::options_description desc("Allowed ToF camera control options");

  configurator.add_common_options(desc);
  desc.add_options()
  ("print,p", "Print the ToF camera configuration")

  ("region", po::value(&enable_region), "Enable region of interest (ROI). Region sizes must be greater than 0.")
  ("region-size-x,w", po::value(&region.size_x)->implicit_value(640), "ROI horisontal size")
  ("region-size-y,r", po::value(&region.size_y)->implicit_value(480), "ROI vertical size")
  ("region-offset-x,x", po::value(&region.offset_x)->default_value(0), "ROI horisontal offset from left")
  ("region-offset-y,y", po::value(&region.offset_y)->default_value(0), "ROI vertical offset from top")

  //("min-depth,d", po::value(&min_depth), "Min depth range for ToF")
  //("max-depth,D", po::value(&max_depth), "Max depth range for ToF")
  ("min-depth,d", po::value(&min_depth)->implicit_value(0.0), "Min depth range for ToF")
  ("max-depth,D", po::value(&max_depth)->implicit_value(5.0), "Max depth range for ToF")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to ToF camera with node ID: " << configurator.node_id;
  i3ds::ToFCameraClient tof(context, configurator.node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  configurator.handle_sensor_commands(vm, tof);

  // Configure region.
  if (vm.count("region"))
    {
      if (enable_region)
        {
          if (region.size_x == 0 || region.size_y == 0)
            {
              BOOST_LOG_TRIVIAL(error) << "Must set region sizes larger than 0!";
              return -1;
            }

          BOOST_LOG_TRIVIAL(info) << "Enable region: ("
                                  << region.offset_x << ","
                                  << region.offset_y << ","
                                  << region.size_x << ","
                                  << region.size_y << ")";
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable region";
        }

      tof.set_region(enable_region, region);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  if (vm.count("min-depth") || vm.count("max-depth")){
    if (! (vm["min-depth"].defaulted() && vm["max-depth"].defaulted()))
      {
	BOOST_LOG_TRIVIAL(info) << "Set range: "
				<< min_depth << " [m],"
				<< max_depth << " [m]";

	tof.set_range(min_depth, max_depth);

	BOOST_LOG_TRIVIAL(info) << "---> [OK]";
      }
    }

  // Print config.
  if (vm.count("print"))
    {
      configurator.print_sensor_status(tof);
      print_tof_settings(tof);
    }

  return 0;
}
