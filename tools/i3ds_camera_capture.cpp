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
#include <cmath>
#include <cstring>

#include <i3ds/subscriber.hpp>
#include <i3ds/camera_sensor.hpp>
#include <i3ds/frame.hpp>
#include <i3ds/opencv_convertion.hpp>

#include <boost/program_options.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace po = boost::program_options;

volatile bool running;

void
signal_handler(int signum)
{
  running = false;
}

void
render_image(std::string window_name, const i3ds::Frame& frame, int image_number)
{
  cv::Mat mat = frame_to_cv_mat(frame, image_number);
  cv::imshow(window_name, mat);
  cv::waitKey(5); // Apparently needed to render image properly
}

void
handle_frame(i3ds::Camera::FrameTopic::Data& data)
{
  switch (data.descriptor.image_count)
    {
    case 1:
      render_image("Camera feed", data, 0);
      break;

    case 2:
      render_image("Left camera feed", data, 0);
      render_image("Right camera feed", data, 1);
      break;

    default:
      break;
    }
}

int main(int argc, char *argv[])
{
  int node;
  bool stereo;
  po::options_description desc("Displays mono or stereo video stream\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value<int>(&node)->default_value(10), "Node ID of camera")
  ("stereo", po::value<bool>(&stereo)->default_value(false), "capture stereo feed")
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

  if (stereo)
    {
      cv::namedWindow("Left camera feed", cv::WINDOW_AUTOSIZE);
      cv::namedWindow("Right camera feed", cv::WINDOW_AUTOSIZE);
    }
  else
    {
      cv::namedWindow("Camera feed", cv::WINDOW_AUTOSIZE);
    }

  subscriber.Attach<i3ds::Camera::FrameTopic>(node, &handle_frame);
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
