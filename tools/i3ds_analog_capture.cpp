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
#include <i3ds/analog_sensor.hpp>

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
handle_frame(i3ds::Analog::MeasurementTopic::Data& data)
{
  const size_t N = data.descriptor.batch_size;
  const size_t M = data.descriptor.series_count;
  
  for (unsigned int i = 0; i < N; i++)
    {
      std::cout << data.descriptor.attributes.timestamp <<  ',' << i << ',';

      for (unsigned int j = 0; j < M; j++)
        {
          const double value = data.samples[i * N + j];

          if (j < (M - 1))
            {
              std::cout << value << ',';
            }
          else
            {
              std::cout << value << std::endl;
            }
        }
    }
}

int main(int argc, char *argv[])
{
  int node;
  po::options_description desc("Displays measurements from analog sensor");

  desc.add_options()
  ("help,h", "Produce this message")
  ("node,n", po::value<int>(&node)->default_value(10), "Node ID of analog sensor")
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

  subscriber.Attach<i3ds::Analog::MeasurementTopic>(node, &handle_frame);
  subscriber.Start();

  while(running)
    {
      sleep(1);
    }

  subscriber.Stop();

  return 0;
}
