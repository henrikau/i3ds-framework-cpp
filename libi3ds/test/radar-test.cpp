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
#include <iostream>

#include "i3ds/subscriber.hpp"
#include "i3ds/emulated_radar.hpp"
#include "i3ds/radar_client.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      radar(context, node),
      server(context),
      client(context, node)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    radar.Attach(server);
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
  EmulatedRadar radar;
  Server server;
  RadarClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_creation)
{
  BOOST_CHECK_EQUAL(radar.node(), node);
  BOOST_CHECK_EQUAL(radar.state(), inactive);
  BOOST_CHECK_EQUAL(radar.rate(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_command)
{
  BOOST_CHECK_EQUAL(radar.state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(radar.state(), standby);

  PlanarRegion r1 = {300, 200, 150, 100};

  client.set_region(true, r1);

  BOOST_CHECK_EQUAL(radar.region_enabled(), true);

  PlanarRegion r2 = radar.region();

  BOOST_CHECK_EQUAL(r1.size_x, r2.size_x);
  BOOST_CHECK_EQUAL(r1.size_y, r2.size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, r2.offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, r2.offset_y);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(radar_configuration_query)
{
  bool region_enabled = true;
  PlanarRegion region = {300, 200, 150, 100};

  client.set_state(activate);
  client.set_region(region_enabled, region);
  client.load_config();

  BOOST_CHECK_EQUAL(region_enabled, client.region_enabled());
  BOOST_CHECK_EQUAL(region.size_x, client.region().size_x);
  BOOST_CHECK_EQUAL(region.size_y, client.region().size_y);
  BOOST_CHECK_EQUAL(region.offset_x, client.region().offset_x);
  BOOST_CHECK_EQUAL(region.offset_y, client.region().offset_y);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(EmulatedRadar::RadarMeasurement::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
  received++;
}

BOOST_AUTO_TEST_CASE(radar_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<EmulatedRadar::RadarMeasurement>(client.node(), &handle_measurement);


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
