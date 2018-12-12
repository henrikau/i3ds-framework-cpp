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
  auto camera_frame = std::make_shared<i3ds::FrameCodec::Data>();

  i3ds::FrameCodec::Initialize(*camera_frame);

  camera_frame->descriptor.attributes.timestamp = 123456789;
  camera_frame->descriptor.attributes.validity = sample_valid;
  camera_frame->descriptor.region.size_x = size_x;
  camera_frame->descriptor.region.size_y = size_y;
  camera_frame->descriptor.pixel_size = pixel_size;
  camera_frame->descriptor.data_depth = data_depth;

  unsigned int image_byte_count = i3ds::image_size(camera_frame->descriptor);

  byte* buffer = static_cast<byte*>(malloc(image_byte_count));

  memset(buffer, 127, image_byte_count);

  camera_frame->append_image(buffer, image_byte_count);

  if (stereo)
    {
      camera_frame->append_image(buffer, image_byte_count);
    }

  run_test<i3ds::FrameCodec>(name, camera_frame, n_replications);

  free(buffer);
}

template <typename Codec>
void
lidar_test(unsigned int n_replications, unsigned int n_points)
{
  auto lidar_data = create_measurement<Codec>();

  lidar_data->region.size_x = 2048;
  lidar_data->region.size_y = 2048;

  for (unsigned int i = 0; i < n_points; ++i)
    {
      lidar_data->points.arr[i].arr[0] = M_PI;
      lidar_data->points.arr[i].arr[1] = M_PI;
      lidar_data->points.arr[i].arr[2] = M_PI;
      lidar_data->points.arr[i].nCount = 3;
    }

  lidar_data->points.nCount = n_points;

  run_test<Codec>("lidar", lidar_data, n_replications);
}

template <typename Codec>
void
radar_test(unsigned int n_replications, unsigned int n_points)
{
  auto radar_data = create_measurement<Codec>();

  radar_data->region.size_x = 2048;
  radar_data->region.size_y = 2048;

  for (unsigned int i = 0; i < n_points; ++i)
    {
      radar_data->distances.arr[i] = M_PI;
      radar_data->validity.arr[i] = depth_range_error; // Not 0
    }

  radar_data->distances.nCount = n_points;
  radar_data->validity.nCount = n_points;

  run_test<Codec>("radar", radar_data, n_replications);
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
  lidar_test<i3ds::LIDARMeasurement750KCodec>(n_replications, 200000);
  std::cerr << " [done]" << std::endl;

  std::cerr << "Radar 400K...";
  radar_test<i3ds::RadarMeasurement400KCodec>(n_replications, 400000);
  std::cerr << " [done]" << std::endl;

  std::cerr << "Analog 1K...";
  analog_test<i3ds::AnalogMeasurement1KCodec>(n_replications, 1000);
  std::cerr << " [done]" << std::endl;

  return 0;
}
