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
#include <i3ds/Camera.h>

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
render_image(std::string window_name, const byte* image, FrameDescriptor& desc)
{
  int rows = desc.region.size_y;
  int cols = desc.region.size_x;
  int pixel_size = desc.pixel_size;
  size_t size = rows * cols * pixel_size;

  std::cout << rows << "," << cols << "," << size << std::endl;

  int cv_type = CV_16UC1;
  double scaling_factor = 1;

  if (desc.frame_mode == mode_rgb)
    {
      std::cout << "RGB " << pixel_size << " " << desc.data_depth << std::endl;

      if (pixel_size == 3) { cv_type = CV_8UC3; }
      if (pixel_size == 6) { cv_type = CV_16UC3; }
      scaling_factor = pow(2,(8 * (pixel_size/3) - desc.data_depth));
    }
  else
    {
      std::cout << "Mono " << pixel_size << " " << desc.data_depth << std::endl;

      if (pixel_size == 1) { cv_type = CV_8UC1; }
      if (pixel_size == 2) { cv_type = CV_16UC1; }
      scaling_factor = pow(2,(8 * pixel_size - desc.data_depth));
    }

  void* data = strndup((const char*) image, size);

  cv::Mat frame(rows, cols, cv_type, data);

  if (scaling_factor != 1)
    {
      std::cout << "Scaling " << scaling_factor << std::endl;
      frame *= scaling_factor;
    }

  cv::imshow(window_name, frame);
  cv::waitKey(5); // Apparently needed to render image properly

  free(data);
}

void
handle_frame(i3ds::Camera::FrameTopic::Data& data)
{
  std::cout << "Recv: " << data.descriptor.attributes.timestamp.microseconds
            << " Count: " << data.image.size()
            << " Size: " << data.image.at(0).size << std::endl;

  switch (data.descriptor.image_count)
    {
    case 1:
      render_image("Camera feed", data.image[0].data, data.descriptor);
      break;

    case 2:
      render_image("Camera feed left", data.image[0].data, data.descriptor);
      render_image("Camera feed right", data.image[1].data, data.descriptor);
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
