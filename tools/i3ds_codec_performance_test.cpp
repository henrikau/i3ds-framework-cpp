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
#include <iomanip>
#include <chrono>
#include <cmath>

#include <i3ds/communication.hpp>
#include <i3ds/camera_sensor.hpp>
#include <i3ds/lidar_sensor.hpp>
#include <i3ds/radar_sensor.hpp>
#include <i3ds/analog_sensor.hpp>
#include <i3ds/frame.hpp>

template <typename Codec>
void
run_test(std::string name, std::shared_ptr<typename Codec::Data> data, unsigned int n_replications)
{
  using namespace std::chrono;

  for (unsigned int i = 0; i < n_replications; ++i)
    {
      i3ds::Message message;

      auto decode_buffer = std::make_shared<typename Codec::Data>();

      const auto t0 = high_resolution_clock::now();
      i3ds::Encode<Codec>(message, *data);
      const auto t1 = high_resolution_clock::now();
      i3ds::Decode<Codec>(message, *decode_buffer);
      const auto t2 = high_resolution_clock::now();

      const auto encoding_time = duration_cast<nanoseconds>(t1 - t0);
      const auto decoding_time = duration_cast<nanoseconds>(t2 - t1);

      size_t size = 0;

      for (int j = 0; j < message.payloads(); j++)
        {
          size += message.size(j);
        }

      std::cout << name << ","
                << size << ","
                << encoding_time.count() << ","
                << decoding_time.count() << std::endl;
    }
}

template <typename Codec>
std::shared_ptr<typename Codec::Data>
create_measurement()
{
  auto measurement = std::make_shared<typename Codec::Data>();
  Codec::Initialize(*measurement);
  measurement->attributes.timestamp = 123456789;
  measurement->attributes.validity = sample_valid;
  return measurement;
}

void
camera_test(std::string name,
            unsigned int n_replications,
            unsigned int size_x, unsigned int size_y, unsigned int pixel_size,
            unsigned int data_depth, bool stereo)
{
  auto frame = std::make_shared<i3ds::FrameCodec::Data>();

  i3ds::FrameCodec::Initialize(*frame);

  frame->descriptor.attributes.timestamp = 123456789;
  frame->descriptor.attributes.validity = sample_valid;
  frame->descriptor.region.size_x = size_x;
  frame->descriptor.region.size_y = size_y;
  frame->descriptor.pixel_size = pixel_size;
  frame->descriptor.data_depth = data_depth;

  unsigned int image_byte_count = i3ds::image_size(frame->descriptor);

  byte* buffer = static_cast<byte*>(malloc(image_byte_count));

  memset(buffer, 127, image_byte_count);

  frame->append_image(buffer, image_byte_count);

  if (stereo)
    {
      frame->append_image(buffer, image_byte_count);
    }

  run_test<i3ds::FrameCodec>(name, frame, n_replications);

  free(buffer);
}

void
lidar_test(unsigned int n_replications, unsigned int n_points)
{
  auto cloud = std::make_shared<i3ds::PointCloudCodec::Data>();

  i3ds::PointCloudCodec::Initialize(*cloud);

  cloud->descriptor.attributes.timestamp = 123456789;
  cloud->descriptor.attributes.validity = sample_valid;
  cloud->descriptor.width = n_points;
  cloud->descriptor.height = 1;

  for (unsigned int i = 0; i < n_points; ++i)
    {
      i3ds::PointXYZ p = {M_PI, M_PI, M_PI};
      
      cloud->points.push_back(p);
    }

  run_test<i3ds::PointCloudCodec>("lidar", cloud, n_replications);
}

void
radar_test(unsigned int n_replications, unsigned int n_points)
{
  auto map = std::make_shared<i3ds::DepthMapCodec::Data>();

  i3ds::DepthMapCodec::Initialize(*map);

  map->descriptor.attributes.timestamp = 123456789;
  map->descriptor.attributes.validity = sample_valid;
  map->descriptor.width = 1000;
  map->descriptor.height = n_points / 1000;

  for (unsigned int i = 0; i < n_points; ++i)
    {
      map->depths.push_back(M_PI);
    }

  run_test<i3ds::DepthMapCodec>("radar", map, n_replications);
}

template <typename Codec>
void
analog_test(unsigned int n_replications, unsigned int n_points)
{
  auto analog_data = create_measurement<Codec>();

  for (unsigned int i = 0; i < n_points; ++i)
    {
      analog_data->samples.arr[i] = M_PI;
    }

  analog_data->samples.nCount = n_points;
  analog_data->series = 1;
  analog_data->batch_size = n_points;

  run_test<Codec>("analog", analog_data, n_replications);
}

int
main(int argc, char *argv[])
{
  unsigned int n_replications = 100;

  std::cerr << "I3DS ASN.1 codec performance tests" << std::endl;
  std::cout << "name,size,encode,decode" << std::endl;

  std::cerr << "TIR 640 x 480 px mono frame...";
  camera_test("tir_mono", n_replications, 640, 480, 2, 16, false);
  std::cerr << " [done]" << std::endl;

  std::cerr << "HR 2048 x 2048 px mono frame...";
  camera_test("hr_mono", n_replications, 2048, 2048, 2, 12, false);
  std::cerr << " [done]" << std::endl;

  std::cerr << "Stereo 1920 x 1080 px stereo frame...";
  camera_test("stereo", n_replications, 1920, 1080, 2, 12, true);
  std::cerr << " [done]" << std::endl;

  std::cerr << "LIDAR 200K...";
  lidar_test(n_replications, 200000);
  std::cerr << " [done]" << std::endl;

  std::cerr << "Radar 400K...";
  radar_test(n_replications, 400000);
  std::cerr << " [done]" << std::endl;

  std::cerr << "Analog 1K...";
  analog_test<i3ds::AnalogMeasurement1KCodec>(n_replications, 1000);
  std::cerr << " [done]" << std::endl;

  return 0;
}
