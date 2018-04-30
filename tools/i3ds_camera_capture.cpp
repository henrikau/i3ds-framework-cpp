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
#include <csignal>

#include "i3ds/subscriber.hpp"
#include "i3ds/camera_sensor.hpp"
#include "i3ds/Camera.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

typedef i3ds::FrameSensor<i3ds::CameraMeasurement1MCodec>::FrameTopic FrameTopic1M;
typedef i3ds::FrameSensor<i3ds::CameraMeasurement4MCodec>::FrameTopic FrameTopic4M;
typedef i3ds::FrameSensor<i3ds::CameraMeasurement8MCodec>::FrameTopic FrameTopic8M;

volatile bool running;

void 
signal_handler(int signum)
{
  running = false;
}

void
handle_measurement1M(FrameTopic1M::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
}

void
handle_measurement4M(FrameTopic4M::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
}

void
handle_measurement8M(FrameTopic8M::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
}

void
render_image(char* image, size_t size)
{
  // TODO: Implement and call from handler-functions
}

int main(int argc, char *argv[])
{
  int node;
  int size;
  po::options_description desc("Allowed options");

  desc.add_options()
  ("help", "Produce this message")
  ("node", po::value<int>(&node)->default_value(10), "Node ID of camera")
  ("size", po::value<int>(&size)->default_value(1), "Image size: 1, 4 or 8 MiB")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Subscriber subscriber(context);

  running = true;
  signal(SIGINT, signal_handler);

  switch(size)
    {
    case 1:
      subscriber.Attach<FrameTopic1M>(node, &handle_measurement1M);
      break;
    case 4:
      subscriber.Attach<FrameTopic4M>(node, &handle_measurement4M);
      break;
    case 8:
      subscriber.Attach<FrameTopic8M>(node, &handle_measurement8M);
      break;
    default:
      std::cout << "Invalid image size: " << size;
      std::cout << ". Available options are: 1, 4 or 8" << std::endl;
      exit(-1);
    }
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }
  
  subscriber.Stop();

  return 0;
}
