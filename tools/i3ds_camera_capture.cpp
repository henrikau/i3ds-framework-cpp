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

#include <i3ds/tof_camera_sensor.hpp>

#include <i3ds/camera_sensor.hpp>


#include <i3ds/frame.hpp>
#include <i3ds/opencv_convertion.hpp>

#include <boost/program_options.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <iomanip>

namespace po = boost::program_options;

volatile bool running;

std::chrono::time_point<std::chrono::high_resolution_clock>previous_time;



void
signal_handler(int signum)
{
  running = false;
}



template <typename T>
void
render_image(std::string window_name, const T& frame, int image_number, std::string fps_text)
{
  cv::Mat mat = i3ds::frame_to_cv_mat(frame, image_number);
#if CV_MAJOR_VERSION == 3
  cv::setWindowTitle (window_name, window_name + " " + fps_text);
#endif
  cv::imshow(window_name, mat);
  cv::waitKey(5); // Apparently needed to render image properly
}


template <typename T>
void
handle_frame2(T& data)
{
  auto time_now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = time_now - previous_time;

  std::stringstream buffer;
  buffer <<  "(fps: "  << std::setprecision(1) << std::fixed << 1000. / (std::chrono::duration <double, std::milli> (diff).count()) << ")" << std::endl;;
  std::cout << "fps: "<< buffer.str();

  previous_time = time_now;

  render_image("Camera feed", data, 0, buffer.str());

}



/*
 *Template meant to replace the two function below but has problem with parameter overloading
template <typename T>
void
handle_frame(typename T::Data& data)
{
  handle_frame2(data);
}
*/


/// Just a wrapper to since I did not get the overloading to work, and could replace it with a template.
void
handle_frame_tof(i3ds::ToFCamera::MeasurementTopic::Data& data)
{
  handle_frame2(data);
}

/// Just a wrapper to since I did not get the overloading to work, and could replace it with a template.
void
handle_frame_camera(i3ds::Camera::FrameTopic::Data& data)
{
  handle_frame2(data);
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

#ifdef __TOF_VERSION__
  cv::namedWindow("ToF Camera feed", cv::WINDOW_AUTOSIZE);
  subscriber.Attach<i3ds::ToFCamera::MeasurementTopic>(node, &handle_frame_tof);
#else
  if (stereo)
    {
      cv::namedWindow("Left camera feed", cv::WINDOW_AUTOSIZE);
      cv::namedWindow("Right camera feed", cv::WINDOW_AUTOSIZE);
    }
  else
    {
      cv::namedWindow("Camera feed", cv::WINDOW_AUTOSIZE);
    }
  subscriber.Attach<i3ds::Camera::FrameTopic>(node, &handle_frame_camera);
#endif
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
