///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE lidar_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_lidar.hpp>
#include <i3ds/lidar_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      lidar(EmulatedLIDAR::Create(context, node)),
      server(context),
      client(LIDARClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");
    lidar->Attach(server);
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
  EmulatedLIDAR::Ptr lidar;
  Server server;
  LIDARClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(lidar_creation)
{
  test_sensor_creation(lidar, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(lidar_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(lidar_region)
{
  test_region<PolarRegion, EmulatedLIDAR, LIDARClient>(lidar, client);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(LIDAR::Measurement400KTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recv: " << data.attributes.timestamp.microseconds);

  BOOST_CHECK_EQUAL(data.region.offset_x, -300.0);
  BOOST_CHECK_EQUAL(data.region.offset_y, -200.0);
  BOOST_CHECK_EQUAL(data.region.size_x, 600.0);
  BOOST_CHECK_EQUAL(data.region.size_y, 400.0);
  received++;
}

BOOST_AUTO_TEST_CASE(lidar_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<LIDAR::Measurement400KTopic>(client->node(), &handle_measurement);


  SamplePeriod period = 100000;
  PolarRegion r1 = {-300.0, -200.0, 600, 400};

  client->set_state(activate);
  client->set_sampling(period);
  client->set_region(true, r1);
  client->set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client->set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
