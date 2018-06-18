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

#include <i3ds/camera_client.hpp>
#include <i3ds/sensor_configurator.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

void
print_camera_settings(i3ds::CameraClient& camera)
{
  camera.load_all();
  std::cout << "\"shutter\" : " << camera.shutter() << "," << std::endl;
  std::cout << "\"gain\" : " << camera.gain() << "," << std::endl;

  std::cout << "\"auto-exposure-enabled\" : " << camera.auto_exposure_enabled() << "," << std::endl;
  std::cout << "\"max-shutter\" : " << camera.max_shutter() << "," << std::endl;
  std::cout << "\"max-gain\" : " << camera.max_gain() << "," << std::endl;

  std::cout << "\"region-enabled\" : " << camera.region_enabled() << "," << std::endl;
  std::cout << "\"region\" : [" << camera.region().size_x;
  std::cout << "," << camera.region().size_y;
  std::cout << "," << camera.region().offset_x;
  std::cout << "," << camera.region().offset_y << "]" << "," << std::endl;

  std::cout << "\"flash-enabled\" : " << camera.flash_enabled() << "," << std::endl;
  std::cout << "\"flash-strength\" : " << camera.flash_strength() << "," << std::endl;

  std::cout << "\"pattern-enabled\" : " << camera.pattern_enabled() << "," << std::endl;
  std::cout << "\"pattern-sequence\" : " << camera.pattern_sequence() << std::endl;
  std::cout << "}" << std::endl;
}

int main(int argc, char *argv[])
{
  i3ds::SensorConfigurator configurator;
  unsigned int shutter, max_shutter;
  double gain, max_gain;
  unsigned int flash_strength, pattern_sequence;
  bool enable_auto, enable_region, enable_flash, enable_pattern;
  PlanarRegion region;

  po::options_description desc("Allowed camera control options");

  configurator.add_common_options(desc);
  desc.add_options()
  ("print", "Print the camera configuration")

  ("shutter", po::value(&shutter), "Shutter time in microseconds, gain is optional")
  ("gain", po::value(&gain)->default_value(0.0), "Sensor gain in decibel, must also set shutter time")

  ("auto-exposure", po::value(&enable_auto), "Enable camera auto exposure")
  ("auto-max-shutter", po::value(&max_shutter)->default_value(100000), "Max auto exposure shutter time in microseconds")
  ("auto-max-gain", po::value(&max_gain)->default_value(100.0), "Max auto exposure gain in decibel")

  ("region", po::value(&enable_region), "Enable camera region of interest (ROI). All or no egion parameters must be set, or 0 will be used as default.")
  ("region-size-x,w", po::value(&region.size_x)->default_value(0), "ROI horisontal size")
  ("region-size-y,h", po::value(&region.size_y)->default_value(0), "ROI vertical size")
  ("region-offset-x,x", po::value(&region.offset_x)->default_value(0), "ROI horisontal offset from left")
  ("region-offset-y,y", po::value(&region.offset_y)->default_value(0), "ROI vertical offset from top")

  ("flash", po::value(&enable_flash), "Enable camera flash")
  ("flash-strength", po::value(&flash_strength)->default_value(255), "Flash strength from 0 (off) to 255 (full)")

  ("pattern", po::value(&enable_pattern), "Enable camera pattern illumination")
  ("pattern-sequence", po::value(&pattern_sequence)->default_value(1), "Pattern sequence to use")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context(i3ds::Context::Create());

  BOOST_LOG_TRIVIAL(info) << "Connecting to camera with node ID: " << configurator.node_id;
  i3ds::CameraClient camera(context, configurator.node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  configurator.handle_sensor_commands(vm, camera);

  // Either set manual exposure or auto exposure, allow further commands.
  if (vm.count("shutter"))
    {
      BOOST_LOG_TRIVIAL(info) << "Set exposure: " << shutter << " [us] " << gain << " [dB]";
      camera.set_exposure(shutter, gain);
      BOOST_LOG_TRIVIAL(trace) << "---> [OK]";
    }
  else if (vm.count("auto-exposure"))
    {
      if (enable_auto)
        {
          BOOST_LOG_TRIVIAL(info) << "Enable auto exposure: " << max_shutter << " [us] " << max_gain << " [dB]";
        }
      else
        {
          BOOST_LOG_TRIVIAL(info) << "Disable auto exposure";
        }

      camera.set_auto_exposure(enable_auto, max_shutter, max_gain);

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

  // Print config, this is the final command.
  if (vm.count("print"))
    {
      configurator.print_sensor_status(camera);
      print_camera_settings(camera);
    }

  return 0;
}
