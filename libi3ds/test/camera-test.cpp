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

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      client(CameraClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");

    // Standard parameters.
    param.camera_name = "test";
    param.external_trigger = false;
    param.support_flash = false;
    param.support_pattern = false;
    param.image_count = 1;
    param.frame_mode = mode_mono;
    param.data_depth = 16;
    param.pixel_size = 2;

    // Width and height for easy testing.
    param.width = 1000;
    param.height = 1000;

    // TODO: These does not matter for test.
    param.packet_size = 8000;
    param.packet_delay = 100;

    // Configuration for external trigger.
    param.trigger_source = 0;
    param.camera_output = 0;
    param.camera_offset = 0;
    param.flash_output = 0;
    param.flash_offset = 0;
    param.pattern_output = 0;
    param.pattern_offset = 0;

    // Node ID for trigger and flash services if supported.
    param.trigger_node = 2;
    param.flash_node = 3;

    param.sample_dir = "";

    camera = EmulatedCamera::Create(context, node, param);

    camera->Attach(server);
    server.Start();
    client->set_timeout(1000);

    received = 0;
  }

  ~F()
  {
    BOOST_TEST_MESSAGE("teardown fixture");
    server.Stop();
  }

  void handle_measurement(Camera::FrameTopic::Data& data);

  void check_region(PlanarRegion ra, PlanarRegion rb);
  void set_legal_region(PlanarRegion r, bool enabled = true);
  void set_illegal_region(PlanarRegion r);

  const NodeID node;

  Context::Ptr context;
  Server server;

  CameraClient::Ptr client;

  EmulatedCamera::Parameters param;
  EmulatedCamera::Ptr camera;

  int received;
};

void F::check_region(PlanarRegion ra, PlanarRegion rb)
{
  BOOST_CHECK_EQUAL(ra.size_x, rb.size_x);
  BOOST_CHECK_EQUAL(ra.size_y, rb.size_y);
  BOOST_CHECK_EQUAL(ra.offset_x, rb.offset_x);
  BOOST_CHECK_EQUAL(ra.offset_y, rb.offset_y);
}

void F::set_legal_region(PlanarRegion r, bool enabled)
{
  client->set_region(enabled, r);

  BOOST_CHECK_EQUAL(camera->region_enabled(), enabled);

  if (enabled)
    {
      check_region(r, camera->region());
    }
}

void F::set_illegal_region(PlanarRegion r)
{
  const PlanarRegion old = camera->region();

  BOOST_CHECK_THROW(client->set_region(true, r), CommandError);

  check_region(old, camera->region());
}

void
F::handle_measurement(Camera::FrameTopic::Data& data)
{
  BOOST_CHECK_EQUAL(data.descriptor.frame_mode, param.frame_mode);
  BOOST_CHECK_EQUAL(data.descriptor.data_depth, param.data_depth);
  BOOST_CHECK_EQUAL(data.descriptor.pixel_size, param.pixel_size);
  BOOST_CHECK_EQUAL(data.descriptor.image_count, param.image_count);
  BOOST_CHECK_EQUAL(data.descriptor.image_count, data.images());

  check_region(data.descriptor.region, camera->region());

  const int size = image_size(data.descriptor);

  for (int i = 0; i < data.images(); i++)
    {
      BOOST_CHECK(data.image_data(i));
      BOOST_CHECK_EQUAL(data.image_size(i), size);
    }

  received++;
}

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
  client->set_state(activate);

  BOOST_CHECK_EQUAL(camera->region_enabled(), false);

  PlanarRegion r;

  r.size_x = param.width / 2;
  r.size_y = param.height / 2;
  r.offset_x = 0;
  r.offset_y = 0;

  set_legal_region(r);

  r.size_x = param.width / 2;
  r.size_y = param.height / 2;
  r.offset_x = r.size_x;
  r.offset_y = r.size_y;

  set_legal_region(r);

  r.size_x = param.width;
  r.size_y = param.height / 2;
  r.offset_x = r.size_x / 2;
  r.offset_y = r.size_y / 2;

  set_illegal_region(r);

  r.size_x = param.width / 2;
  r.size_y = param.height;
  r.offset_x = r.size_x / 2;
  r.offset_y = r.size_y / 2;

  set_illegal_region(r);

  r.size_x = param.width / 2;
  r.size_y = param.height / 2;
  r.offset_x = r.size_x / 2;
  r.offset_y = r.size_y / 2;

  set_legal_region(r);

  r.size_x = param.width;
  r.size_y = param.height / 2;
  r.offset_x = 0;
  r.offset_y = r.size_y;

  set_legal_region(r);

  r.size_x = param.width;
  r.size_y = param.height;
  r.offset_x = 0;
  r.offset_y = 0;

  set_legal_region(r, false);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_exposure)
{
  client->set_state(activate);

  // Check manual exposure settings.
  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), false);

  client->set_exposure(1000, 1.0);

  BOOST_CHECK_EQUAL(camera->shutter(), 1000);
  BOOST_CHECK_CLOSE(camera->gain(), 1.0, 1e-6);

  // Check auto exposure settings.
  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), false);

  client->set_auto_exposure(true, 10000, 1.0);

  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), true);
  BOOST_CHECK_EQUAL(camera->max_shutter(), 10000);
  BOOST_CHECK_CLOSE(camera->max_gain(), 1.0, 1e-6);

  // Disable auto exposure settings, should not change max shutter/gain.
  client->set_auto_exposure(false, 0, 0.0);

  BOOST_CHECK_EQUAL(camera->auto_exposure_enabled(), false);
  BOOST_CHECK_EQUAL(camera->max_shutter(), 10000);
  BOOST_CHECK_CLOSE(camera->max_gain(), 1.0, 1e-6);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_configuration_query)
{
  ShutterTime shutter = 10000;
  SensorGain gain = 2.0;
  bool auto_exposure_enabled = true;
  ShutterTime max_shutter = 100000;
  SensorGain max_gain = 3.0;
  // bool flash_enabled = true;
  // FlashStrength flash_strength = 128;
  // bool pattern_enabled = true;
  // PatternSequence pattern_sequence = 10;

  client->set_state(activate);

  client->set_exposure(shutter, gain);
  client->set_auto_exposure(auto_exposure_enabled, max_shutter, max_gain);

  // client->set_flash(flash_enabled, flash_strength);
  // client->set_pattern(pattern_enabled, pattern_sequence);

  client->load_config();

  BOOST_CHECK_EQUAL("Emulated camera", client->device_name());

  BOOST_CHECK_EQUAL(shutter, client->shutter());
  BOOST_CHECK_CLOSE(gain, client->gain(), 1e-6);

  BOOST_CHECK_EQUAL(auto_exposure_enabled, client->auto_exposure_enabled());
  BOOST_CHECK_EQUAL(max_shutter, client->max_shutter());
  BOOST_CHECK_CLOSE(max_gain, client->max_gain(), 1e-6);

  // BOOST_CHECK_EQUAL(flash_enabled, client->flash_enabled());
  // BOOST_CHECK_EQUAL(flash_strength, client->flash_strength());

  // BOOST_CHECK_EQUAL(pattern_enabled, client->pattern_enabled());
  // BOOST_CHECK_EQUAL(pattern_sequence, client->pattern_sequence());
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_sampling)
{
  using namespace std::placeholders;

  Subscriber subscriber(context);

  subscriber.Attach<Camera::FrameTopic>(node, std::bind(&F::handle_measurement, this, _1));
  subscriber.Start();

  SamplePeriod period = 100000;

  client->set_state(activate);
  client->set_sampling(period);
  client->set_state(start);

  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));

  client->set_state(stop);

  subscriber.Stop();

  BOOST_CHECK_GT(received, 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
