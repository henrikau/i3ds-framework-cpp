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

class Timeseries {
private: 
  double mean_time_;
  long long min_time_;
  long long max_time_;
  long long sum_times_ = 0;
  long long sum_times_squared_ = 0;
  unsigned int n_ = 0;
public:
  double mean() const {return mean_time_;}
  long long min() const {return min_time_;}
  long long max() const {return max_time_;}
  long long median() const {return max_time_ - min_time_;}
  unsigned int n() const {return n_;}
  double variance() const {return (sum_times_squared_ - (static_cast<double>(sum_times_*sum_times_)/n_))/(n_-1);}
  double std_dev() const {return sqrt(variance());}

  void add_observation(long t)
    {
      if (n_ == 0) 
        {
          max_time_ = t;
          min_time_ = t;
        }
      n_++;
      sum_times_ += t;
      sum_times_squared_ += t*t;
      mean_time_ = static_cast<double>(sum_times_) / n_;
      if (t>max_time_) {max_time_ = t;}
      if (t<min_time_) {min_time_ = t;}
    }
};

std::ostream&
operator<<(std::ostream& os, const Timeseries& rhs)
{
  os << "Mean: " << std::setw(9) << static_cast<long>(rhs.mean())
     << " Max: " << std::setw(9)<< rhs.max() 
     << " Min: " << std::setw(9)<< rhs.min() 
     << " Std.dev: " << std::setw(9)<< static_cast<long>(rhs.std_dev());
  return os;
}

template <typename Codec>
void
run_test(std::shared_ptr<typename Codec::Data> data, unsigned int n_replications, unsigned int floats_per_measurement)
{
  Timeseries encode_times;  
  Timeseries decode_times;  
  Timeseries total_times;  
  std::chrono::high_resolution_clock::time_point start_time, done_encoding_time, done_decoding_time;

  for (unsigned int i = 0; i < n_replications; ++i)
    {
      i3ds::Message message;
      auto decode_buffer = std::make_shared<typename Codec::Data>();
      start_time = std::chrono::high_resolution_clock::now();
      i3ds::Encode<Codec>(message, *data);
      done_encoding_time = std::chrono::high_resolution_clock::now();
      i3ds::Decode<Codec>(message, *decode_buffer); 
      done_decoding_time = std::chrono::high_resolution_clock::now();

      auto encoding_time = std::chrono::duration_cast<std::chrono::microseconds>(done_encoding_time-start_time);
      auto decoding_time = std::chrono::duration_cast<std::chrono::microseconds>(done_decoding_time-done_encoding_time);
      auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(done_decoding_time-start_time);
      encode_times.add_observation(encoding_time.count());
      decode_times.add_observation(decoding_time.count());
      total_times.add_observation(total_time.count());
    }
  std::cout << "  Encode time: " << encode_times 
            << " us/f: " << std::setw(9) << static_cast<double>(encode_times.mean())/floats_per_measurement << std::endl;
  std::cout << "  Decode time: " << decode_times 
            << " us/f: " << std::setw(9) << static_cast<double>(decode_times.mean())/floats_per_measurement << std::endl;
  std::cout << "  Total time:  " << total_times 
            << " us/f: " << std::setw(9) << static_cast<double>(total_times.mean())/floats_per_measurement << std::endl << std::endl;
}

template <typename Codec>
std::shared_ptr<typename Codec::Data>
create_measurement()
{
  auto measurement = std::make_shared<typename Codec::Data>();
  Codec::Initialize(*measurement);
  measurement->attributes.timestamp.microseconds = 123456789;
  measurement->attributes.validity = sample_valid;
  return measurement;
}

void
camera_test(unsigned int n_replications, unsigned int size_x, unsigned int size_y, unsigned int pixel_size, unsigned int data_depth, bool stereo)
{
  auto camera_frame = std::make_shared<i3ds::FrameCodec::Data>();
  i3ds::FrameCodec::Initialize(*camera_frame);
  camera_frame->descriptor.attributes.timestamp.microseconds = 123456789;
  camera_frame->descriptor.attributes.validity = sample_valid;
  camera_frame->descriptor.region.size_x = size_x;
  camera_frame->descriptor.region.size_y = size_y;
  camera_frame->descriptor.pixel_size = pixel_size;
  camera_frame->descriptor.data_depth = data_depth;
  unsigned int image_byte_count = camera_frame->descriptor.region.size_x
                                * camera_frame->descriptor.region.size_y
                                * camera_frame->descriptor.pixel_size;
  i3ds::Image img;
  img.size = image_byte_count;
  byte* buffer = static_cast<byte*>(malloc(image_byte_count));
  memset(buffer, 127, image_byte_count);
  img.data = buffer;
  camera_frame->image.push_back(img);
  if (stereo) 
    {
      camera_frame->image.push_back(img);
      run_test<i3ds::FrameCodec>(camera_frame, n_replications, image_byte_count*2);
    }
  else
    {
      run_test<i3ds::FrameCodec>(camera_frame, n_replications, image_byte_count);
    }
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
  
  std::cout << n_points << " point LIDAR measurement timing statistics:" << std::endl;
  run_test<Codec>(lidar_data, n_replications, n_points*3 + 4);
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
      radar_data->validity.arr[i] = measurement_error; // Not 0
    }
  radar_data->distances.nCount = n_points;
  radar_data->validity.nCount = n_points;
  
  std::cout << n_points << " point radar measurement timing statistics:" << std::endl;
  run_test<Codec>(radar_data, n_replications, n_points);
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
  
  std::cout << n_points << " analog measurements timing statistics:" << std::endl;
  run_test<Codec>(analog_data, n_replications, n_points);
}

int
main(int argc, char *argv[])
{
  unsigned int n_replications = 100;

  std::cout << "libi3ds codec performance test." << std::endl;
  std::cout << "The value 'us/f' is number of microseconds per byte for camera measurements" << std::endl;
  std::cout << "and microseconds per float for the rest." << std::endl << std::endl;

  std::cout << "640 x 480 px mono frame:" << std::endl;
  camera_test(n_replications, 640, 480, 2, 16, false);

  std::cout << "1920 x 1080 px mono frame:" << std::endl;
  camera_test(n_replications, 1920, 1080, 2, 12, false);

  std::cout << "2048 x 2048 px mono frame:" << std::endl;
  camera_test(n_replications, 2048, 2048, 2, 12, false);

  std::cout << "1920 x 1080 px stereo frame:" << std::endl;
  camera_test(n_replications, 1920, 1080, 2, 12, true);

  std::cout << "2048 x 2048 px stereo frame:" << std::endl;
  camera_test(n_replications, 2048, 2048, 2, 12, true);

  lidar_test<i3ds::LIDARMeasurement200KCodec>(n_replications, 200000);
  radar_test<i3ds::RadarMeasurement400KCodec>(n_replications, 400000);
  analog_test<i3ds::AnalogMeasurement10KCodec>(n_replications, 10000);

  return 0;
}
