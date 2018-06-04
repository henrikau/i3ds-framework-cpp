///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE tof_camera_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_tof_camera.hpp>
#include <i3ds/tof_camera_client.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      camera(EmulatedToFCamera::Create(context, node)),
      server(context),
      client(context, node)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    camera->Attach(server);
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
  EmulatedToFCamera::Ptr camera;
  Server server;
  ToFCameraClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(tof_camera_creation)
{
  BOOST_CHECK_EQUAL(camera->node(), node);
  BOOST_CHECK_EQUAL(camera->state(), inactive);
  BOOST_CHECK_EQUAL(camera->period(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(tof_camera_command)
{
  BOOST_CHECK_EQUAL(camera->state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(camera->state(), standby);

  PlanarRegion r1 = {300, 200, 150, 100};

  client.set_region(true, r1);

  BOOST_CHECK_EQUAL(camera->region_enabled(), true);

  PlanarRegion r2 = camera->region();

  BOOST_CHECK_EQUAL(r1.size_x, r2.size_x);
  BOOST_CHECK_EQUAL(r1.size_y, r2.size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, r2.offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, r2.offset_y);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(tof_camera_configuration_query)
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
handle_measurement(ToFCamera::Measurement2MTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recv: " << data.attributes.timestamp.microseconds);
  received++;
}

BOOST_AUTO_TEST_CASE(tof_camera_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<ToFCamera::Measurement2MTopic>(client.node(), &handle_measurement);


  SamplePeriod period = 100000;

  client.set_state(activate);
  client.set_period(period);
  client.set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 2));

  client.set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
