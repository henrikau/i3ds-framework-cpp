///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE measurement_proxy_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/measurement_proxy.hpp>
#include <i3ds/address_server.hpp>
#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_imu.hpp>
#include <i3ds/imu_client.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(325),
      context(Context::Create()),
      address_server("test_addresses.csv"),
      imu(EmulatedIMU::Create(context, node)),
      imu_command_server(context),
      client(context, node),
      proxy(9001,9002)
  {
    address_server.Start();
    imu->Attach(imu_command_server);
    imu_command_server.Start();
    client.set_timeout(1000);
    proxy.Start();
  }

  ~F()
  {
    proxy.Stop();
    imu_command_server.Stop();
    address_server.Stop();
  }

  const NodeID node;
  Context::Ptr context;
  AddressServer address_server;
  EmulatedIMU::Ptr imu;
  Server imu_command_server;
  IMUClient client;
  MeasurementProxy proxy;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

int subscriber_1_received_count;
int subscriber_2_received_count;

void
check_measurement(IMU::MeasurementTopic::Data& data)
{
  BOOST_CHECK_EQUAL(data.linear_accel.arr[0].nCount, 3);
  BOOST_CHECK_EQUAL(data.linear_accel.arr[0].arr[0], 1.0);
  BOOST_CHECK_EQUAL(data.linear_accel.arr[0].arr[1], 2.0);
  BOOST_CHECK_EQUAL(data.linear_accel.arr[0].arr[2], 3.0);

  BOOST_CHECK_EQUAL(data.angular_rate.arr[0].nCount, 3);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[0].arr[0], 4.0);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[0].arr[1], 5.0);
  BOOST_CHECK_EQUAL(data.angular_rate.arr[0].arr[2], 6.0);
}

void
subscriber_1_handle_measurement(IMU::MeasurementTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Subscriber 1 received msg at: " << data.attributes.timestamp.microseconds);
  check_measurement(data);
  subscriber_1_received_count++;
}

void
subscriber_2_handle_measurement(IMU::MeasurementTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Subscriber 2 received msg at: " << data.attributes.timestamp.microseconds);
  check_measurement(data);
  subscriber_2_received_count++;
}

BOOST_AUTO_TEST_CASE(pub_sub_test)
{
  // Sleep to allow address server time to start up
  std::this_thread::sleep_for(std::chrono::microseconds(10000));
  subscriber_1_received_count = 0;
  subscriber_2_received_count = 0;
  Subscriber subscriber1(context);
  Subscriber subscriber2(context);

  subscriber1.Attach<IMU::MeasurementTopic>(client.node(), &subscriber_1_handle_measurement);
  subscriber2.Attach<IMU::MeasurementTopic>(client.node(), &subscriber_2_handle_measurement);

  SamplePeriod period = 100000;

  client.set_state(activate);
  client.set_sampling(period);
  client.set_state(start);

  subscriber1.Start();
  subscriber2.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client.set_state(stop);

  subscriber1.Stop();
  subscriber2.Stop();

  BOOST_CHECK_GT(subscriber_1_received_count, 0);
  BOOST_CHECK_GT(subscriber_2_received_count, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
