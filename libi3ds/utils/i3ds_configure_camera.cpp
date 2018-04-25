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

#include "i3ds/sensors/camera_client.hpp"

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_camera_settings(i3ds::CameraClient *camera)
{
  camera->load_all();

  int state = camera->state();
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
  std::cout << "\"node\" : " << camera->node() << "," << std::endl;
  std::cout << "\"state\" : \"" << state_string << "\"," << std::endl;
  std::cout << "\"temperature\" : " << camera->temperature().kelvin << "," << std::endl;

  std::cout << "\"exposure\" : " << camera->exposure() << "," << std::endl;
  std::cout << "\"gain\" : " << camera->gain() << "," << std::endl;

  std::cout << "\"auto-exposure-enabled\" : " << camera->auto_exposure_enabled() << "," << std::endl;
  std::cout << "\"max-exposure\" : " << camera->max_exposure() << "," << std::endl;
  std::cout << "\"max-gain\" : " << camera->max_gain() << "," << std::endl;

  std::cout << "\"region-enabled\" : " << camera->region_enabled() << "," << std::endl;
  std::cout << "\"region\" : [" << camera->region().size_x;
  std::cout << "," << camera->region().size_y;
  std::cout << "," << camera->region().offset_x;
  std::cout << "," << camera->region().offset_y << "]" << "," << std::endl;

  std::cout << "\"flash-enabled\" : " << camera->flash_enabled() << "," << std::endl;
  std::cout << "\"flash-strength\" : " << camera->flash_strength() << "," << std::endl;

  std::cout << "\"pattern-enabled\" : " << camera->pattern_enabled() << "," << std::endl;
  std::cout << "\"pattern-sequence\" : " << camera->pattern_sequence() << std::endl;
  std::cout << "}" << std::endl;
}

int main(int argc, char *argv[])
{
  unsigned int node_id;
  unsigned int rate, exposure, min_exposure, max_exposure;
  double gain, min_gain, max_gain;
  unsigned int flash_strength, pattern_sequence;
  bool enable_auto, enable_region, enable_flash, enable_pattern;
  PlanarRegion region;

  po::options_description desc("Allowed camera control options");

  desc.add_options()
  ("help", "Produce this message")
  ("node", po::value<unsigned int>(&node_id)->required(), "Node ID of camera")

  ("activate", "Activate the sensor")
  ("start", "Start the sensor")
  ("stop", "Stop the sensor")
  ("deactivate", "Deactivate the sensor")

  ("rate", po::value(&rate), "Sensor rate in microseconds")

  ("verbose,v", "Print verbose output")
  ("quite,q", "Quiet ouput")
  ("print", "Print the camera configuration")

  ("exposure", po::value(&exposure), "Exposure time in microseconds, gain is optional")
  ("gain", po::value(&gain)->default_value(0.0), "Sensor gain in decibel, must also set exposure")

  ("auto-exposure", po::value(&enable_auto), "Enable camera auto exposure")
  ("auto-min-exposure", po::value(&min_exposure)->default_value(0), "Min auto exposure time in microseconds")
  ("auto-max-exposure", po::value(&max_exposure)->default_value(100000), "Max auto exposure time in microseconds")
  ("auto-min-gain", po::value(&min_gain)->default_value(0.0), "Min auto gain in decibel")
  ("auto-max-gain", po::value(&max_gain)->default_value(100.0), "Max auto gain in decibel")

  ("region", po::value(&enable_region), "Enable camera region of interest (ROI)")
  ("region-size-x,w", po::value(&region.size_x)->default_value(0), "ROI horisontal size")
  ("region-size-y,h", po::value(&region.size_y)->default_value(0), "ROI vertical size")
  ("region-offset-x,x", po::value(&region.offset_x)->default_value(0), "ROI horisontal offset from left")
  ("region-offset-y,y", po::value(&region.offset_y)->default_value(0), "ROI vertical offset from top")

  ("flash", po::value(&enable_flash), "Enable camera flash")
  ("flash-strength", po::value(&flash_strength)->default_value(255), "Flash strength from 0 (off) to 255 (full)")

  ("pattern", po::value(&enable_pattern), "Enable camera pattern illumination")
  ("pattern-sequence", po::value(&pattern_sequence)->default_value(1), "Pattern sequence to use")
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

  BOOST_LOG_TRIVIAL(info) << "Connecting to camera with node ID: " << node_id;
  i3ds::CameraClient camera(context, node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  // Can either command activate, allow more commands.
  if (vm.count("activate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command ACTIVATE";
      camera.set_state(activate);
    }

  // Command stop, allow more commands.
  if (vm.count("stop"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command STOP";
      camera.set_state(stop);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Command deactivate, no more commands.
  if (vm.count("deactivate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command DEACTIVATE";
      camera.set_state(deactivate);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
      return 0;
    }

  // Set rate, allow further commands.
  if (vm.count("rate"))
    {
      BOOST_LOG_TRIVIAL(info) << "Set rate: " << rate << " [us]";
      camera.set_rate(rate);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Either set manual exposure or auto exposure, allow further commands.
  if (vm.count("exposure"))
    {
      BOOST_LOG_TRIVIAL(info) << "Set exposure: " << exposure << " [us] " << gain << " [dB]";
      camera.set_exposure(exposure, gain);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
  else if (vm.count("auto-exposure"))
    {
      if (enable_auto)
        {
          BOOST_LOG_TRIVIAL(info) << "Enable auto exposure: " << max_exposure << " [us] " << max_gain << " [dB]";
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable auto exposure";
        }

      camera.set_auto_exposure(enable_auto, max_exposure, max_gain);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Enable region, allow further commands.
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

      camera.set_region(enable_region, region);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Enable flash, allow further commands.
  if (vm.count("flash"))
    {
      if (enable_flash)
        {
          BOOST_LOG_TRIVIAL(info) << "Enable flash: " << flash_strength;
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable flash";
        }

      camera.set_flash(enable_flash, flash_strength);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Enable pattern illumination, allow further commands.
  if (vm.count("pattern"))
    {
      if (enable_pattern)
        {
          BOOST_LOG_TRIVIAL(info) << "Enable pattern: " << pattern_sequence;
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable pattern";
        }

      camera.set_pattern(enable_pattern, pattern_sequence);

      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Start sensor, allow further commands.
  if (vm.count("start"))
    {
      BOOST_LOG_TRIVIAL(info) << "Command START";
      camera.set_state(start);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }

  // Print config, this is the final command.
  if (vm.count("print"))
    {
      print_camera_settings(&camera);
    }

  return 0;
}
