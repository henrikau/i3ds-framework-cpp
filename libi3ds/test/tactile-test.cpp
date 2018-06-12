///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE tactile_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_tactile.hpp>
#include <i3ds/analog_client.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      tactile(EmulatedTactile::Create(context, node)),
      client(context, node)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    tactile->Attach(server);
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
  EmulatedTactile::Ptr tactile;
  AnalogClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(tactile_creation)
{
  BOOST_CHECK_EQUAL(tactile->node(), node);
  BOOST_CHECK_EQUAL(tactile->state(), inactive);
  BOOST_CHECK_EQUAL(tactile->period(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(tactile_state_command)
{
  BOOST_CHECK_EQUAL(tactile->state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(tactile->state(), standby);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(Analog::Measurement10KTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recived measurement with batch size: " << data.batch_size);

  BOOST_CHECK_EQUAL(data.series, 3);
  BOOST_CHECK(data.batch_size < 103 && data.batch_size > 98);

  BOOST_CHECK_EQUAL(data.samples.nCount, data.batch_size * data.series);
  BOOST_CHECK_EQUAL(data.samples.arr[0], 0.0);
  BOOST_CHECK_CLOSE(data.samples.arr[1], 6.666, 0.1);
  BOOST_CHECK_CLOSE(data.samples.arr[2], 13.333, 0.1);

  received++;
}

BOOST_AUTO_TEST_CASE(tactile_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<Analog::Measurement10KTopic>(client.node(), &handle_measurement);

  SamplePeriod period = 100000;

  client.set_state(activate);
  client.set_sampling(period);
  client.set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client.set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
