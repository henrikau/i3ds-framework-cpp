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

#include <i3ds/subscriber.hpp>
#include <i3ds/camera_sensor.hpp>
#include <i3ds/Camera.h>

#include <boost/program_options.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace po = boost::program_options;

typedef i3ds::FrameSensor<i3ds::CameraMeasurement1MCodec>::FrameTopic FrameTopic1M;
typedef i3ds::FrameSensor<i3ds::CameraMeasurement4MCodec>::FrameTopic FrameTopic4M;
typedef i3ds::FrameSensor<i3ds::CameraMeasurement8MCodec>::FrameTopic FrameTopic8M;
typedef i3ds::FrameSensor<i3ds::StereoCameraMeasurement4MCodec>::FrameTopic StereoFrameTopic4M;
typedef i3ds::FrameSensor<i3ds::StereoCameraMeasurement8MCodec>::FrameTopic StereoFrameTopic8M;

volatile bool running;

void
signal_handler(int signum)
{
  running = false;
}

void
render_image(std::string window_name, unsigned char* image, int rows, int cols, int type, int pixel_size)
{
  int cv_type = CV_16UC1;
  if (type == mode_rgb)
    {
      if (pixel_size == 3) cv_type = CV_8UC3;
      if (pixel_size == 6) cv_type = CV_16UC3;
    }
  else
    {
      if (pixel_size == 1) cv_type = CV_8UC1;
      if (pixel_size == 2) cv_type = CV_16UC1;
    }
  cv::Mat frame(rows, cols, cv_type, image);
  cv::imshow(window_name, frame);
  cv::waitKey(5); // Apparently needed to render image properly
}

template <typename T>
void
handle_mono_frame(typename T::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
  int rows = data.region.size_y;
  int cols = data.region.size_x;
  render_image("Camera feed", static_cast<unsigned char*>(data.image.arr), rows, cols, data.frame_mode, data.pixel_size);
}

template <typename T>
void
handle_stereo_frame(typename T::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
  int rows = data.region.size_y;
  int cols = data.region.size_x / 2;
  render_image("Left camera feed", static_cast<unsigned char*>(data.image_left.arr), rows, cols, data.frame_mode, data.pixel_size);
  render_image("Right camera feed", static_cast<unsigned char*>(data.image_right.arr), rows, cols, data.frame_mode, data.pixel_size);
}

int main(int argc, char *argv[])
{
  int node;
  int size;
  bool stereo;
  po::options_description desc("Allowed options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value<int>(&node)->default_value(10), "Node ID of camera")
  ("stereo", po::value<bool>(&stereo)->default_value(false), "capture stereo feed")
  ("size,s", po::value<int>(&size)->default_value(1), "Image size: 1, 4 or 8 MiB")
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
      switch(size)
        {
        case 4:
          subscriber.Attach<StereoFrameTopic4M>(node, &handle_stereo_frame<StereoFrameTopic4M>);
          break;
        case 8:
          subscriber.Attach<StereoFrameTopic8M>(node, &handle_stereo_frame<StereoFrameTopic8M>);
          break;
        default:
          std::cout << "Invalid image size: " << size;
          std::cout << ". Available options for stereo are: 4 or 8" << std::endl;
          exit(-1);
        }
    }
  else
    {
      cv::namedWindow("Camera feed", cv::WINDOW_AUTOSIZE);
      switch(size)
        {
        case 1:
          subscriber.Attach<FrameTopic1M>(node, &handle_mono_frame<FrameTopic1M>);
          break;
        case 4:
          subscriber.Attach<FrameTopic4M>(node, &handle_mono_frame<FrameTopic4M>);
          break;
        case 8:
          subscriber.Attach<FrameTopic8M>(node, &handle_mono_frame<FrameTopic8M>);
          break;
        default:
          std::cout << "Invalid image size: " << size;
          std::cout << ". Available options are: 1, 4 or 8" << std::endl;
          exit(-1);
        }
    }
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
