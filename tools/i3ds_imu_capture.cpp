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
#include <atomic>

#include <i3ds/subscriber.hpp>
#include <i3ds/imu_sensor.hpp>
#include <i3ds/configurator.hpp>

#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

std::atomic<bool> running;

void
signal_handler(int)
{
  running = false;
}

void
handle_frame(i3ds::IMU::MeasurementTopic::Data& data)
{
  for (unsigned int i = 0; i < data.batch_size; i++)
    {
      std::cout << data.attributes.timestamp <<  ',' << i << ','
                << data.samples.arr[i].axis_x_acceleration << ','
                << data.samples.arr[i].axis_y_acceleration << ','
                << data.samples.arr[i].axis_z_acceleration << ','
                << data.samples.arr[i].axis_x_rate << ','
                << data.samples.arr[i].axis_y_rate << ','
                << data.samples.arr[i].axis_z_rate
                << std::endl;
    }
}

int main(int argc, char *argv[])
{
  int node;
  po::options_description desc("Displays measurements from analog sensor");
  i3ds::Configurator configurator;
  configurator.add_common_options(desc);
  desc.add_options()
  ("node,n", po::value<int>(&node)->default_value(10), "Node ID of analog sensor")
  ;

  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context = i3ds::Context::Create();
  i3ds::Subscriber subscriber(context);

  running = true;
  signal(SIGINT, signal_handler);

  subscriber.Attach<i3ds::IMU::MeasurementTopic>(node, &handle_frame);
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
