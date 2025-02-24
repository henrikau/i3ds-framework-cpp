///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE analog_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_analog.hpp>
#include <i3ds/analog_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      analog(EmulatedAnalog::CreateTactile(context, node)),
      client(AnalogClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");

    analog->Attach(server);
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
  EmulatedAnalog::Ptr analog;
  AnalogClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(analog_creation)
{
  test_sensor_creation(analog, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(analog_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(Analog::MeasurementTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recived measurement with batch size: " << data.descriptor.batch_size);

  BOOST_CHECK_EQUAL(data.descriptor.series_count, 3);
  BOOST_CHECK_EQUAL(data.descriptor.batch_size, 10);

  BOOST_CHECK_EQUAL(data.samples.size(), data.descriptor.batch_size * data.descriptor.series_count);

  for (const float value : data.samples)
    {
      BOOST_CHECK(0.0 <= value && value <= 20.0);
    }

  received++;
}

BOOST_AUTO_TEST_CASE(analog_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<Analog::MeasurementTopic>(client->node(), &handle_measurement);

  SamplePeriod period = 100000;

  client->set_state(activate);
  client->set_sampling(period, 10);
  client->set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client->set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
