///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE camera_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_camera.hpp>
#include <i3ds/camera_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

typedef EmulatedMonoCamera<Camera::MonoFrame8MTopic> TestCamera;

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      prop( {mode_mono, 12, 2, 640, 480}),
  camera(TestCamera::Create(context, node, prop)),
  server(context),
  client(CameraClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");
    camera->Attach(server);
    server.Start();
    client->set_timeout(1000);
  }

  ~F()
  {
    BOOST_TEST_MESSAGE("teardown fixture");
    server.Stop();
  }

  const NodeID node;

  Context::Ptr context;

  FrameProperties prop;
  TestCamera::Ptr camera;
  Server server;
  CameraClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_creation)
{
  test_sensor_creation(camera, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_region)
{
  test_region<PlanarRegion, TestCamera, CameraClient>(camera, client);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_command)
{
  BOOST_CHECK_EQUAL(camera->state(), inactive);
  client->set_state(activate);
  BOOST_CHECK_EQUAL(camera->state(), standby);

  BOOST_CHECK_EQUAL(camera->shutter(), 0);
  BOOST_CHECK_CLOSE(camera->gain(), 0.0, 1e-6);
  client->set_exposure(1000, 1.0);
  BOOST_CHECK_EQUAL(camera->shutter(), 1000);
  BOOST_CHECK_CLOSE(camera->gain(), 1.0, 1e-6);

  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), false);
  BOOST_CHECK_EQUAL(camera->max_shutter(), 0);
  BOOST_CHECK_CLOSE(camera->max_gain(), 0.0, 1e-6);

  client->set_auto_exposure(true, 10000, 1.0);

  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), true);
  BOOST_CHECK_EQUAL(camera->max_shutter(), 10000);
  BOOST_CHECK_CLOSE(camera->max_gain(), 1.0, 1e-6);

  BOOST_CHECK_EQUAL(camera->flash_enabled(), false);
  BOOST_CHECK_EQUAL(camera->flash_strength(), 0);

  client->set_flash(true, 128);

  BOOST_CHECK_EQUAL(camera->flash_enabled(), true);
  BOOST_CHECK_EQUAL(camera->flash_strength(), 128);

  BOOST_CHECK_EQUAL(camera->pattern_enabled(), false);
  BOOST_CHECK_EQUAL(camera->pattern_sequence(), 0);

  client->set_pattern(true, 1);

  BOOST_CHECK_EQUAL(camera->pattern_enabled(), true);
  BOOST_CHECK_EQUAL(camera->pattern_sequence(), 1);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_configuration_query)
{
  ShutterTime shutter = 10000;
  SensorGain gain = 2.0;
  bool auto_exposure_enabled = true;
  ShutterTime max_shutter = 100000;
  SensorGain max_gain = 3.0;
  bool flash_enabled = true;
  FlashStrength flash_strength = 128;
  bool pattern_enabled = true;
  PatternSequence pattern_sequence = 10;

  client->set_exposure(shutter, gain);
  client->set_auto_exposure(auto_exposure_enabled, max_shutter, max_gain);
  client->set_flash(flash_enabled, flash_strength);
  client->set_pattern(pattern_enabled, pattern_sequence);

  client->load_config();

  BOOST_CHECK_EQUAL(shutter, client->shutter());
  BOOST_CHECK_CLOSE(gain, client->gain(), 1e-6);

  BOOST_CHECK_EQUAL(auto_exposure_enabled, client->auto_exposure_enabled());
  BOOST_CHECK_EQUAL(max_shutter, client->max_shutter());
  BOOST_CHECK_CLOSE(max_gain, client->max_gain(), 1e-6);

  BOOST_CHECK_EQUAL(flash_enabled, client->flash_enabled());
  BOOST_CHECK_EQUAL(flash_strength, client->flash_strength());

  BOOST_CHECK_EQUAL(pattern_enabled, client->pattern_enabled());
  BOOST_CHECK_EQUAL(pattern_sequence, client->pattern_sequence());
}

////////////////////////////////////////////////////////////////////////////////

int received;

void
handle_measurement(Camera::MonoFrame8MTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Recv: " << data.attributes.timestamp.microseconds);

  BOOST_CHECK_EQUAL(data.region.offset_x, 400);
  BOOST_CHECK_EQUAL(data.region.offset_y, 300);
  BOOST_CHECK_EQUAL(data.region.size_x, 150);
  BOOST_CHECK_EQUAL(data.region.size_y, 100);
  received++;
}

BOOST_AUTO_TEST_CASE(camera_sampling)
{
  received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<Camera::MonoFrame8MTopic>(client->node(), &handle_measurement);

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
