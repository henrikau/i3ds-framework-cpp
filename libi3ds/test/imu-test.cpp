///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE imu_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <iostream>

#include "i3ds/core/subscriber.hpp"
#include "i3ds/emulators/emulated_imu.hpp"
#include "i3ds/sensors/imu_client.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      imu(context, node),
      client(context, node)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    imu.Attach(server);
    server.Start();
    client.set_timeout(1000);
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    server.Stop();
  }

  const NodeID node;

  Context::Ptr context;
  Server server;
  EmulatedIMU imu;
  IMUClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(imu_creation)
{
  BOOST_CHECK_EQUAL(imu.node(), node);
  BOOST_CHECK_EQUAL(imu.state(), inactive);
  BOOST_CHECK_EQUAL(imu.rate(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(imu_state_command)
{
  BOOST_CHECK_EQUAL(imu.state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(imu.state(), standby);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(EmulatedIMU::EmulatedIMUMeasurement::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
  BOOST_CHECK_EQUAL(data.linear_accel.arr[0], 1.0);
  BOOST_CHECK_EQUAL(data.linear_accel.arr[1], 2.0);
  BOOST_CHECK_EQUAL(data.linear_accel.arr[2], 3.0);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[0], 4.0);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[1], 5.0);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[2], 6.0);
  received++;
}

BOOST_AUTO_TEST_CASE(camera_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<EmulatedIMU::EmulatedIMUMeasurement>(client.node(), &handle_measurement);


  SampleRate rate = 100000;

  client.set_state(activate);
  client.set_rate(rate);
  client.set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(rate * 2));

  client.set_state(stop);

  std::chrono::milliseconds(100);
  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
