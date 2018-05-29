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
#include <fstream>
#include <functional>
#include <csignal>
#include <chrono>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include <i3ds/communication.hpp>
#include <i3ds/subscriber.hpp>
#include <i3ds/sensor_client.hpp>
#include <i3ds/camera_sensor.hpp>
#include <i3ds/tof_camera_sensor.hpp>
#include <i3ds/radar_sensor.hpp>
#include <i3ds/emulated_lidar.hpp>
#include <i3ds/star_tracker_sensor.hpp>
#include <i3ds/imu_sensor.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

volatile bool running;

void
signal_handler(int signum)
{
  running = false;
}


class DelayRecorder
{
private:

  struct MessageRecord
    {
        unsigned int node_id;
        unsigned int message_size;
        long long sent_time;
        long long received_time;
        long long delay;
    };

  template <typename T>
  void 
  handle_measurement(typename T::Data& message)
    {
      current_time_ = std::chrono::high_resolution_clock::now();
      long long current_time_as_int = std::chrono::duration_cast<std::chrono::microseconds>(current_time_.time_since_epoch()).count();
      long long delay = current_time_as_int - message.attributes.timestamp.microseconds;
      records.push_back({node_id_,T::Codec::max_size, message.attributes.timestamp.microseconds,
                        current_time_as_int, delay});
      BOOST_LOG_TRIVIAL(trace) << "Received message at " << current_time_as_int;
    }

  i3ds::Context::Ptr context_;
  i3ds::Subscriber subscriber_;
  unsigned int node_id_;
  std::string file_name_;
  std::ofstream output_file_;
  std::chrono::high_resolution_clock::time_point current_time_;
  std::vector<MessageRecord> records;

public:
  DelayRecorder(i3ds::Context::Ptr context, unsigned int node_id, std::string file_name) : 
       context_(context),
       subscriber_(context_),
       node_id_(node_id),
       file_name_(file_name),
       output_file_(file_name)
    {
    }

  virtual ~DelayRecorder() 
    {
      output_file_.close();
    }

  template <typename T>
  void
  Attach()
    {
      using std::placeholders::_1;
      subscriber_.Attach<T>(node_id_, std::bind(&DelayRecorder::handle_measurement<T>, this, _1));
    }

  void Start()
    {
      if (!output_file_.is_open())
        {
          throw std::runtime_error("Unable to open file: " + file_name_);
        }

      output_file_ << "#Node ID,data size[byte],sent timestamp[microsecond],received timestamp[microsecond],delay[microsecond]" << std::endl;
      
      subscriber_.Start();
    }

  void Stop()
    {
      subscriber_.Stop();
      BOOST_LOG_TRIVIAL(info) << "Stopping. Received " << records.size() << " messages.";
    }

  void write_output()
    {
      for (MessageRecord m : records)
        {
          output_file_ << m.node_id << ","
                       << m.message_size << ","
                       << m.sent_time << ","
                       << m.received_time << "," 
                       << m.delay << std::endl;
        }
      BOOST_LOG_TRIVIAL(info) << "Output written to " << file_name_;
    }
};


int
main(int argc, char *argv[])
{
  unsigned int node_id;
  std::string sensor_type;
  std::string output_file;

  po::options_description desc("Allowed delay recorder options");

  desc.add_options()
   ("help,h", "Produce this message")
   ("node,n", po::value(&node_id)->required(), "Node ID of sensor")
   ("type,t", po::value<std::string>(&sensor_type)->required(), "Type of sensor. Can be one of: tir, hr, stereo, tof, lidar, radar, st or imu")
   ("output,o", po::value<std::string>(&output_file)->default_value("out.csv"), "File name to write output. CSV format.")
   ("verbose,v", "Print verbose output")
   ("quiet,q", "Quiet ouput")
   ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);

  if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return -1;
    }

  if (vm.count("quiet"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::warning);
    }
  else if (!vm.count("verbose"))
    {
      logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }
  
  try 
    {
      po::notify(vm);
    }
  catch (std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
    }

  BOOST_LOG_TRIVIAL(info) << "Listening for messages of sensor type " << sensor_type
                          << " on node ID " << node_id;
  i3ds::Context::Ptr context = i3ds::Context::Create();
  DelayRecorder delay_recorder(context, node_id, output_file);

  running = true;
  signal(SIGINT, signal_handler);

  if (sensor_type == "mono1M")
    {
      delay_recorder.Attach<i3ds::Camera::MonoFrame1MTopic>();
    }
  else if (sensor_type == "mono4M")
    {
      delay_recorder.Attach<i3ds::Camera::MonoFrame4MTopic>();
    }
  else if (sensor_type == "mono8M" || sensor_type == "tir" || sensor_type == "hr")
    {
      delay_recorder.Attach<i3ds::Camera::MonoFrame8MTopic>();
    }
  else if (sensor_type == "stereo4M")
    {
      delay_recorder.Attach<i3ds::Camera::StereoFrame4MTopic>();
    }
  else if (sensor_type == "stereo8M" || sensor_type == "stereo")
    {
      delay_recorder.Attach<i3ds::Camera::StereoFrame8MTopic>();
    }
  else if (sensor_type == "tof250K")
    {
      delay_recorder.Attach<i3ds::ToFCamera::Measurement250KTopic>();
    }
  else if (sensor_type == "tof500K")
    {
      delay_recorder.Attach<i3ds::ToFCamera::Measurement500KTopic>();
    }
  else if (sensor_type == "tof1M")
    {
      delay_recorder.Attach<i3ds::ToFCamera::Measurement1MTopic>();
    }
  else if (sensor_type == "tof2M" || sensor_type == "tof")
    {
      delay_recorder.Attach<i3ds::ToFCamera::Measurement2MTopic>();
    }
  else if (sensor_type == "lidar")
    {
      delay_recorder.Attach<i3ds::EmulatedLIDAR::LIDARMeasurement>();
    }
  else if (sensor_type == "radar100K")
    {
      delay_recorder.Attach<i3ds::Radar::Measurement100KTopic>();
    }
  else if (sensor_type == "radar200K")
    {
      delay_recorder.Attach<i3ds::Radar::Measurement200KTopic>();
    }
  else if (sensor_type == "radar400K" || sensor_type == "radar")
    {
      delay_recorder.Attach<i3ds::Radar::Measurement400KTopic>();
    }
  else if (sensor_type == "st")
    {
      delay_recorder.Attach<i3ds::StarTracker::MeasurementTopic>();
    }
  else if (sensor_type == "imu")
    {
      delay_recorder.Attach<i3ds::IMU::MeasurementTopic>();
    }
  else
    {
      std::cerr << "Error: Invalid sensor type: " + sensor_type << std::endl;
      return -1;
    }

  try 
    {
      delay_recorder.Start();
    }
  catch (std::exception e)
    {
      std::cerr << e.what() << std::endl;
      return -1;
    }

  while(running)
    {
      sleep(1);
    }

  delay_recorder.Stop();
  delay_recorder.write_output();

  return 0;
}
