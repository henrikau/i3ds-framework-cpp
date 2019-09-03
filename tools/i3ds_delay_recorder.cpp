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
#include <atomic>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include <i3ds/time.hpp>
#include <i3ds/communication.hpp>
#include <i3ds/subscriber.hpp>
#include <i3ds/sensor_client.hpp>
#include <i3ds/camera_sensor.hpp>
#include <i3ds/tof_camera_sensor.hpp>
#include <i3ds/radar_sensor.hpp>
#include <i3ds/lidar_sensor.hpp>
#include <i3ds/star_tracker_sensor.hpp>
#include <i3ds/imu_sensor.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

std::atomic<bool> running;

void
signal_handler(int)
{
  running = false;
}


class DelayRecorder
{
private:

  struct MessageRecord
  {
    unsigned int node_id;
    long long sent_time;
    long long received_time;
    long long delay;
  };

  template <typename T>
  void
  handle_measurement(typename T::Data& message)
  {
    long long current_time_as_int = i3ds::get_timestamp();
    long long delay = current_time_as_int - message.attributes.timestamp;
    records.push_back({node_id_, message.attributes.timestamp,
                       current_time_as_int, delay
                      });
    BOOST_LOG_TRIVIAL(trace) << "Received message at " << current_time_as_int;
  }

  void
  handle_frame(i3ds::Camera::FrameTopic::Data& message)
  {
    long long current_time_as_int = i3ds::get_timestamp();
    long long delay = current_time_as_int - message.descriptor.attributes.timestamp;
    records.push_back({node_id_, message.descriptor.attributes.timestamp,
                       current_time_as_int, delay
                      });
    BOOST_LOG_TRIVIAL(trace) << "Received message at " << current_time_as_int;
  }

  i3ds::Context::Ptr context_;
  i3ds::Subscriber subscriber_;
  unsigned int node_id_;
  std::string file_name_;
  std::ofstream output_file_;
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

    output_file_ << "node,sent,received" << std::endl;

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
                     << m.sent_time << ","
                     << m.received_time << std::endl;
      }
    BOOST_LOG_TRIVIAL(info) << "Output written to " << file_name_;
  }

  unsigned int n_measurements()
  {
    return records.size();
  }
};

template <>
void
DelayRecorder::Attach<i3ds::Camera::FrameTopic>()
{
  using std::placeholders::_1;
  subscriber_.Attach<i3ds::Camera::FrameTopic>(node_id_, std::bind(&DelayRecorder::handle_frame, this, _1));
}

int
main(int argc, char *argv[])
{
  unsigned int node_id;
  unsigned int max_measurements;
  std::string sensor_type;
  std::string output_file;

  po::options_description
  desc("Measures message timestamp and time of reception and stores the results in a CSV file.\n  Available options");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value(&node_id)->required(), "Node ID of sensor")
  ("type,t", po::value<std::string>(&sensor_type)->required(),
   "Type of sensor. Can be one of: cam, tof, lidar, radar, st or imu")
  ("max_measurements,m", po::value(&max_measurements)->default_value(0), "Number of measurements to record (not precise). 0 means no limit.")
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

  if (sensor_type == "cam")
    {
      delay_recorder.Attach<i3ds::Camera::FrameTopic>();
    }
  else if (sensor_type == "tof")
    {
      delay_recorder.Attach<i3ds::ToFCamera::MeasurementTopic>();
    }
  else if (sensor_type == "lidar")
    {
      delay_recorder.Attach<i3ds::LIDAR::MeasurementTopic>();
    }
  else if (sensor_type == "radar")
    {
      delay_recorder.Attach<i3ds::Radar::MeasurementTopic>();
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
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return -1;
    }

  // Wait for max_measurements if it is not 0, and always for exit-signal
  while(running && (max_measurements == 0 || delay_recorder.n_measurements() < max_measurements))
    {
      sleep(1);
    }

  delay_recorder.Stop();
  delay_recorder.write_output();

  return 0;
}
