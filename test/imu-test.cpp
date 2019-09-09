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

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_imu.hpp>
#include <i3ds/imu_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      imu(EmulatedIMU::Create(context, node)),
      client(IMUClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");
    imu->Attach(server);
    server.Start();
    client->set_timeout(1000);
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    server.Stop();
  }

  const NodeID node;

  Context::Ptr context;
  Server server;
  EmulatedIMU::Ptr imu;
  IMUClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(imu_creation)
{
  test_sensor_creation(imu, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(imu_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(IMU::MeasurementTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recv: " << data.attributes.timestamp);

  BOOST_CHECK_EQUAL(data.samples.nCount, 1);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_x_rate, 1.0);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_x_acceleration, 2.0);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_y_rate, 3.0);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_y_acceleration, 4.0);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_z_rate, 5.0);
  BOOST_CHECK_EQUAL(data.samples.arr[0].axis_z_acceleration, 6.0);

  received++;
}

BOOST_AUTO_TEST_CASE(imu_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<IMU::MeasurementTopic>(client->node(), &handle_measurement);

  SamplePeriod period = 100000;

  client->set_state(activate);
  client->set_sampling(period);
  client->set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client->set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
