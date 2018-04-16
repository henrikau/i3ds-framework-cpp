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

#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "client.hpp"
#include "subscriber.hpp"
#include "emulated_camera.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

class CameraClient : public Client
{
public:

  CameraClient(Context::Ptr context, NodeID sensor);

  void set_state(StateCommand state);
  void set_rate(SampleRate rate);

  void set_exposure(ExposureTime exposure, SensorGain gain);
  void set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain);
  void set_region(bool enable, PlanarRegion region);
  void set_flash(bool enable, FlashStrength strength);
  void set_pattern(bool enable, PatternSequence sequence);

  ExposureTime exposure() const {return config_.response.exposure;}
  SensorGain gain() const {return config_.response.gain;}
  bool auto_exposure_enabled() const {return config_.response.auto_exposure_enabled;}
  ExposureTime max_exposure() const {return config_.response.max_exposure;}
  SensorGain max_gain() const {return config_.response.max_gain;}
  bool region_enabled() const {return config_.response.region_enabled;}
  PlanarRegion region() const {return config_.response.region;}
  bool flash_enabled() const {return config_.response.flash_enabled;}
  FlashStrength flash_strength() const {return config_.response.flash_strength;}
  bool pattern_enabled() const {return config_.response.pattern_enabled;}
  PatternSequence pattern_sequence() const {return config_.response.pattern_sequence;}

  void Attach(Subscriber& subscriber);

  void load_configuration();

  void handle_measurement(EmulatedCamera::ImageMeasurement::Data& data);

  int received() const {return received_;}

private:

  Camera::ConfigurationService::Data config_;

  int received_;
};

CameraClient::CameraClient(Context::Ptr context, NodeID sensor)
  : Client(context, sensor)
{
  Camera::ConfigurationService::Initialize(config_);

  received_ = 0;
}

void
CameraClient::load_configuration()
{
  bool result = Call<Camera::ConfigurationService>(config_, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_state(StateCommand state)
{
  Sensor::StateService::Data command;

  command.request = state;

  bool result = Call<Sensor::StateService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_rate(SampleRate rate)
{
  Sensor::SampleService::Data command;

  command.request.rate = rate;
  command.request.count = 0;

  bool result = Call<Sensor::SampleService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_exposure(ExposureTime exposure, SensorGain gain)
{
  Camera::ExposureService::Data command;

  command.request.exposure = exposure;
  command.request.gain = gain;

  bool result = Call<Camera::ExposureService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain)
{
  Camera::AutoExposureService::Data command;

  command.request.enable = enable;
  command.request.max_exposure = max_exposure;
  command.request.max_gain = max_gain;

  bool result = Call<Camera::AutoExposureService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_region(bool enable, PlanarRegion region)
{
  Camera::RegionService::Data command;

  command.request.enable = enable;
  command.request.region = region;

  bool result = Call<Camera::RegionService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_flash(bool enable, FlashStrength strength)
{
  Camera::FlashService::Data command;

  command.request.enable = enable;
  command.request.strength = strength;

  bool result = Call<Camera::FlashService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::set_pattern(bool enable, PatternSequence sequence)
{
  Camera::PatternService::Data command;

  command.request.enable = enable;
  command.request.sequence = sequence;

  bool result = Call<Camera::PatternService>(command, 1000);

  BOOST_CHECK(result);
}

void
CameraClient::Attach(Subscriber& subscriber)
{
  using std::placeholders::_1;

  subscriber.Attach<EmulatedCamera::ImageMeasurement>(node(), std::bind(&CameraClient::handle_measurement, this, _1));
}

void
CameraClient::handle_measurement(EmulatedCamera::ImageMeasurement::Data& data)
{
  std::cout << "Recv: " << data.attributes.timestamp.microseconds << std::endl;
  received_++;
}

////////////////////////////////////////////////////////////////////////////////

struct F {
  F()
    : node(1),
      resx(800),
      resy(600),
      context(Context::Create()),
      server(context),
      camera(context, node, resx, resy),
      client(context, node)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    camera.Attach(server);
    server.Start();
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    server.Stop();
  }

  const NodeID node;
  const int resx, resy;

  Context::Ptr context;
  Server server;
  EmulatedCamera camera;
  CameraClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_creation)
{
  BOOST_CHECK_EQUAL(camera.node(), node);
  BOOST_CHECK_EQUAL(camera.state(), inactive);
  BOOST_CHECK_EQUAL(camera.rate(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_command)
{
  BOOST_CHECK_EQUAL(camera.state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(camera.state(), standby);

  BOOST_CHECK_EQUAL(camera.exposure(), 0);
  BOOST_CHECK_CLOSE(camera.gain(), 0.0, 1e-6);
  client.set_exposure(1000, 1.0);
  BOOST_CHECK_EQUAL(camera.exposure(), 1000);
  BOOST_CHECK_CLOSE(camera.gain(), 1.0, 1e-6);

  BOOST_CHECK_EQUAL(camera.auto_exposure_enabled(), false);
  BOOST_CHECK_EQUAL(camera.max_exposure(), 0);
  BOOST_CHECK_CLOSE(camera.max_gain(), 0.0, 1e-6);

  client.set_auto_exposure(true, 10000, 1.0);

  BOOST_CHECK_EQUAL(camera.auto_exposure_enabled(), true);
  BOOST_CHECK_EQUAL(camera.max_exposure(), 10000);
  BOOST_CHECK_CLOSE(camera.max_gain(), 1.0, 1e-6);

  PlanarRegion r1 = {300, 200, 150, 100};

  client.set_region(true, r1);

  BOOST_CHECK_EQUAL(camera.region_enabled(), true);

  PlanarRegion r2 = camera.region();

  BOOST_CHECK_EQUAL(r1.size_x, r2.size_x);
  BOOST_CHECK_EQUAL(r1.size_y, r2.size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, r2.offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, r2.offset_y);

  BOOST_CHECK_EQUAL(camera.flash_enabled(), false);
  BOOST_CHECK_EQUAL(camera.flash_strength(), 0);

  client.set_flash(true, 128);

  BOOST_CHECK_EQUAL(camera.flash_enabled(), true);
  BOOST_CHECK_EQUAL(camera.flash_strength(), 128);

  BOOST_CHECK_EQUAL(camera.pattern_enabled(), false);
  BOOST_CHECK_EQUAL(camera.pattern_sequence(), 0);

  client.set_pattern(true, 1);

  BOOST_CHECK_EQUAL(camera.pattern_enabled(), true);
  BOOST_CHECK_EQUAL(camera.pattern_sequence(), 1);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_configuration_query)
{
  ExposureTime exposure = 0.01;
  SensorGain gain = 2.0;
  bool auto_exposure_enabled = true;
  ExposureTime max_exposure = 0.1;
  SensorGain max_gain = 3.0;
  bool region_enabled = true;
  PlanarRegion region = {300, 200, 150, 100};
  bool flash_enabled = true;
  FlashStrength flash_strength = 128;
  bool pattern_enabled = true;
  PatternSequence pattern_sequence = 10;

  client.set_state(activate);

  client.set_exposure(exposure, gain);
  client.set_auto_exposure(auto_exposure_enabled, max_exposure, max_gain);
  client.set_region(region_enabled, region);
  client.set_flash(flash_enabled, flash_strength);
  client.set_pattern(pattern_enabled, pattern_sequence);

  client.load_configuration();

  BOOST_CHECK_EQUAL(exposure, client.exposure());
  BOOST_CHECK_CLOSE(gain, client.gain(), 1e-6);

  BOOST_CHECK_EQUAL(auto_exposure_enabled, client.auto_exposure_enabled());
  BOOST_CHECK_EQUAL(max_exposure, client.max_exposure());
  BOOST_CHECK_CLOSE(max_gain, client.max_gain(), 1e-6);

  BOOST_CHECK_EQUAL(region_enabled, client.region_enabled());
  BOOST_CHECK_EQUAL(region.size_x, client.region().size_x);
  BOOST_CHECK_EQUAL(region.size_y, client.region().size_y);
  BOOST_CHECK_EQUAL(region.offset_x, client.region().offset_x);
  BOOST_CHECK_EQUAL(region.offset_y, client.region().offset_y);

  BOOST_CHECK_EQUAL(flash_enabled, client.flash_enabled());
  BOOST_CHECK_EQUAL(flash_strength, client.flash_strength());

  BOOST_CHECK_EQUAL(pattern_enabled, client.pattern_enabled());
  BOOST_CHECK_EQUAL(pattern_sequence, client.pattern_sequence());
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_sampling)
{
  Subscriber subscriber(context);

  client.Attach(subscriber);

  SampleRate rate = 100000;

  client.set_state(activate);
  client.set_rate(rate);
  client.set_state(start);

  subscriber.Start();

  std::this_thread::sleep_for(std::chrono::microseconds(rate * 2));

  client.set_state(stop);

  std::chrono::milliseconds(100);
  subscriber.Stop();

  BOOST_CHECK_GT(client.received(), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
