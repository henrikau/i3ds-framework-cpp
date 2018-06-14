///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE radar_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_radar.hpp>
#include <i3ds/radar_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      radar(EmulatedRadar::Create(context, node)),
      server(context),
      client(RadarClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");
    radar->Attach(server);
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
  EmulatedRadar::Ptr radar;
  Server server;
  RadarClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_creation)
{
  test_sensor_creation(radar, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_region)
{
  test_region<PlanarRegion, EmulatedRadar, RadarClient>(radar, client);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(Radar::Measurement400KTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recv: " << data.attributes.timestamp.microseconds);

  BOOST_CHECK_EQUAL(data.region.offset_x, 400);
  BOOST_CHECK_EQUAL(data.region.offset_y, 300);
  BOOST_CHECK_EQUAL(data.region.size_x, 150);
  BOOST_CHECK_EQUAL(data.region.size_y, 100);
  received++;
}

BOOST_AUTO_TEST_CASE(radar_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<Radar::Measurement400KTopic>(client->node(), &handle_measurement);

  SamplePeriod period = 100000;
  PlanarRegion region = {400, 300, 150, 100};

  client->set_state(activate);
  client->set_sampling(period);
  client->set_region(true, region);
  client->set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client->set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
