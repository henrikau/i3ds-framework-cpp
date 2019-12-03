///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018-2019 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>

#include <i3ds/communication.hpp>
#include <i3ds/emulated_camera.hpp>
#include <i3ds/configurator.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <atomic>

namespace po = boost::program_options;
namespace logging = boost::log;

std::atomic<bool> running;

void signal_handler(int)
{
  BOOST_LOG_TRIVIAL(info) << "STOP";
  running = false;
}

int main(int argc, char** argv)
{
  unsigned int node_id;

  i3ds::EmulatedCamera::Parameters param;
  bool rgb;

  po::options_description desc("Allowed camera control options");
  i3ds::Configurator configurator;
  configurator.add_common_options(desc);

  desc.add_options()
  ("node,n", po::value<unsigned int>(&node_id)->default_value(10), "Node ID of camera")

  ("samples,S", po::value<std::string>(&param.sample_dir), "Directory holding image samples")

  ("camera-name,c",   po::value<std::string>(&param.camera_name), "Connect via (UserDefinedName) of Camera")
  ("package-size,p",  po::value<int>(&param.packet_size)->default_value(8192), "Transport-layer buffersize (MTU).")
  ("package-delay,d", po::value<int>(&param.packet_delay)->default_value(20), "Inter-package delay parameter of camera.")

  ("rgb",            po::bool_switch(&rgb)->default_value(false), "RGB output for the camera.")
  ("depth",          po::value<int>(&param.data_depth)->default_value(12), "Data depth for camera")
  ("width",          po::value<long>(&param.width)->default_value(2048), "Horizontal resolution for camera")
  ("height",         po::value<long>(&param.height)->default_value(2048), "Vertical resolution for camera")
    
  ("trigger",        po::bool_switch(&param.external_trigger)->default_value(false), "External trigger.")
  ("trigger-node",   po::value<NodeID>(&param.trigger_node)->default_value(20), "Node ID of trigger service.")
  ("trigger-source", po::value<int>(&param.trigger_source)->default_value(1), "Trigger generator for camera.")

  ("trigger-camera-output", po::value<int>(&param.camera_output)->default_value(2), "Trigger output for camera.")
  ("trigger-camera-offset", po::value<int>(&param.camera_offset)->default_value(5000), "Trigger offset for camera (us).")

  ("flash",                po::bool_switch(&param.support_flash)->default_value(false), "Support wide-angle flash.")
  ("flash-node",           po::value<NodeID>(&param.flash_node)->default_value(21), "Node ID of flash service.")
  ("trigger-flash-output", po::value<int>(&param.flash_output)->default_value(8), "Trigger output for flash.")
  ("trigger-flash-offset", po::value<int>(&param.flash_offset)->default_value(4200), "Trigger offset for flash (us).")

  ("pattern",                po::bool_switch(&param.support_pattern)->default_value(false), "Support pattern illumination.")
  ("trigger-pattern-output", po::value<int>(&param.pattern_output)->default_value(6), "Trigger output for pattern.")
  ("trigger-pattern-offset", po::value<int>(&param.pattern_offset)->default_value(0), "Trigger offset for pattern (us).")

  ("print",     "Print the camera configuration")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  BOOST_LOG_TRIVIAL(info) << "Node ID:     " << node_id;
  BOOST_LOG_TRIVIAL(info) << "Camera name: " << param.camera_name;
  BOOST_LOG_TRIVIAL(info) << "Camera type: Emulated HR";

  param.pixel_size = (param.data_depth - 1) / 8 + 1;

  if (rgb)
    {
      param.pixel_size = param.pixel_size * 3;
      param.frame_mode = mode_rgb;
      BOOST_LOG_TRIVIAL(info) << "Camera mode: RGB";
    }
  else
    {
      param.frame_mode = mode_mono;
      BOOST_LOG_TRIVIAL(info) << "Camera mode: monochrome";
    }

  BOOST_LOG_TRIVIAL(info) << "Pixel depth: " << param.data_depth;
  BOOST_LOG_TRIVIAL(info) << "Pixel size:  " << param.pixel_size;
  BOOST_LOG_TRIVIAL(info) << "Sensor size: " << param.width << "x" << param.width << " px";
  
  param.image_count = 1;

  i3ds::Context::Ptr context = i3ds::Context::Create();;

  i3ds::Server server(context);

  i3ds::EmulatedCamera camera(context, node_id, param);

  camera.Attach(server);

  running = true;
  signal(SIGINT, signal_handler);

  server.Start();

  while (running)
    {
      sleep(1);
    }

  server.Stop();

  return 0;
}
