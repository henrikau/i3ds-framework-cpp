///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>

#include <i3ds/pose_estimator_client.hpp>
#include <i3ds/configurator.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_pose_estimator_settings(i3ds::PoseEstimatorClient& pose_estimator)
{
  pose_estimator.load_all();
  std::cout << "imaging mode: " << pose_estimator.imaging_mode() << std::endl;
  std::cout << "selected camera: " << pose_estimator.selected_camera() << std::endl;
}

int main(int argc, char *argv[])
{
  i3ds::SensorConfigurator configurator;
  bool imaging_mode;
  uint8_t selected_camera;

  po::options_description desc("Allowed pose_estimator control options");

  configurator.add_common_options(desc);
  desc.add_options()
  ("print", "Print the pose estimator configuration")

  ("imaging_mode,i", po::value(&imaging_mode), "Enable imaging mode.")
  ("camera_select,c", po::value(&selected_camera), "Select camera.")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to pose estimator with node ID: " << configurator.node_id;
  i3ds::PoseEstimatorClient pose_estimator(context, configurator.node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  configurator.handle_sensor_commands(vm, pose_estimator);

  
  // Configure imaging_mode.
  if (vm.count("imaging_mode"))
    {
      BOOST_LOG_TRIVIAL(info) << "Setting imaging mode: " << imaging_mode;
      pose_estimator.set_imaging_mode(imaging_mode);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Configure camera_select.
  if (vm.count("camera_select"))
    {
      BOOST_LOG_TRIVIAL(info) << "Setting selected camera: " << selected_camera;
      pose_estimator.set_selected_camera(selected_camera);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Print config.
  if (vm.count("print"))
    {
      configurator.print_sensor_status(pose_estimator);
      print_pose_estimator_settings(pose_estimator);
    }

  return 0;
}
