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
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <iomanip>

namespace po = boost::program_options;

volatile bool running;

std::chrono::time_point<std::chrono::high_resolution_clock>previous_time;
float scale = 100;
int width = -1;
bool headless_mode = false;
std::string output = "", format;
bool do_output = false, do_scale = false, do_size = false;

unsigned int img_index = 0;

void
signal_handler(int signum)
{
  running = false;
}

void
handle_image(std::string window_name, const i3ds::Frame& frame, int image_number, std::string fps_text)
{
  cv::Mat mat = frame_to_cv_mat(frame, image_number);

  std::ostringstream path;
  path <<  output << "_";
  path << std::setw(5) << std::setfill('0') << img_index;
  path << "_" << image_number << "." << format;

  cv::imwrite(path.str(), mat);
  if (headless_mode) {
    return;
  }

  cv::Mat outmat;

  if(do_size){
    cv::Size size = mat.size();
    float height = (size.height * width)/size.width;
    cv::resize(mat, outmat, cv::Size(width, height), 0, 0, cv::INTER_AREA);
  } else if (do_scale) {
    cv::resize(mat, outmat, cv::Size(0, 0), scale/100, scale/100, cv::INTER_AREA);
  } else {
    outmat = mat;
  }

#if CV_MAJOR_VERSION == 3
  cv::setWindowTitle (window_name, window_name + " " + fps_text);
#endif
  
  cv::imshow(window_name, outmat);
  cv::waitKey(5); // Apparently needed to render image properly
}

void
handle_frame(i3ds::Camera::FrameTopic::Data& data)
{
  auto time_now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = time_now - previous_time;

  std::stringstream buffer;
  buffer <<  "(fps: "  << std::setprecision(1) << std::fixed << 1000. / (std::chrono::duration <double, std::milli> (diff).count()) << ")" << std::endl;;
  std::cout << "fps: "<< buffer.str();

  previous_time = time_now;
  img_index++;


  switch (data.descriptor.image_count)
    {
    case 1:
      handle_image("Camera feed", data, 0, buffer.str());
      break;

    case 2:
      handle_image("Left camera feed", data, 0, buffer.str());
      handle_image("Right camera feed", data, 1, buffer.str());
      break;

    default:
      break;
    }
}

int main(int argc, char *argv[])
{
  int node;
  po::options_description desc("Displays mono or stereo video stream\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value<int>(&node)->default_value(10), "Node ID of camera")
  ("scale,x", po::value(&scale), "Camera scale [%]")
  ("width,w", po::value(&width), "Maximal image width [px]") 
  ("output,o", po::value(&output), "Output path template") 
  ("format,f", po::value(&format)->default_value("tiff"), "Output format") 
  ("nogui,g", po::bool_switch(&headless_mode), "Headless mode") 
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  if (vm.count("width")) {
    do_size = true;
    std::cout << "Scaling width to: " << width << " [px]" << std::endl;
  } else if (vm.count("scale")) {
    do_scale = true;
    std::cout << "Scale: " << scale << "%" << std::endl;
  }

  if (vm.count("output")) {
    do_output = true;
    std::cout << "Storing data to: " << output << "_XXXXX." << format << std::endl;
  }

  std::cout << "Connecting to Node with ID: " << node << std::endl;

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Subscriber subscriber(context);

  running = true;
  signal(SIGINT, signal_handler);

  subscriber.Attach<i3ds::Camera::FrameTopic>(node, &handle_frame);
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
