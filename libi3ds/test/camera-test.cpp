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

#include "client.hpp"
#include "emulated_camera.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

class CameraClient : public Client
{
public:

  CameraClient(Context::Ptr context, NodeID sensor);

  ResultCode set_state(StateCommand state);
  ResultCode set_rate(SensorRate rate);
  ResultCode set_exposure(ExposureTime exposure_time);
  ResultCode set_gain(SensorGain gain);
  ResultCode set_auto_exposure(bool auto_exposure);
  ResultCode set_exposure_limit(ExposureTime exposure_limit);
  ResultCode set_gain_limit(SensorGain gain_limit);
  ResultCode set_region(PlanarRegion region);
  ResultCode set_flash_enabled(bool flash_enabled);
  ResultCode set_flash_strength(FlashStrength flash_strength);
  ResultCode set_pattern_enabled(bool pattern_enabled);
  ResultCode set_illumination_pattern(IlluminationPattern pattern);

  ExposureTime exposure() const {return config_.response.config_exposure;}
  SensorGain gain() const {return config_.response.config_gain;}
  bool auto_exposure() const {return config_.response.config_auto_exposure;}
  ExposureTime exposure_limit() const {return config_.response.config_exposure_limit;}
  SensorGain gain_limit() const {return config_.response.config_gain_limit;}
  PlanarRegion region() const {return config_.response.config_region;}
  bool flash_enabled() const {return config_.response.config_flash_illumination;}
  FlashStrength flash_strength() const {return config_.response.config_flash_strength;}
  bool pattern_enabled() const {return config_.response.config_pattern_illumination;}
  IlluminationPattern illumination_pattern() const {return config_.response.config_illumination_pattern;}

  void load_configuration();

private:

  ResultCode issue_command();

  Camera::CommandService::Data command_;
  Camera::ConfigurationService::Data config_;
};

CameraClient::CameraClient(Context::Ptr context, NodeID sensor)
  : Client(context, sensor)
{
  Camera::ConfigurationService::Initialize(config_);
}

void
CameraClient::load_configuration()
{
  bool result = Call<Camera::ConfigurationService>(Sensor::CONFIGURATION, config_, 1000);

  BOOST_CHECK(result);
}

ResultCode
CameraClient::issue_command()
{
  bool result = Call<Camera::CommandService>(Sensor::COMMAND, command_, 1000);

  BOOST_CHECK(result);

  return command_.response.result;
}

ResultCode
CameraClient::set_state(StateCommand state)
{
  command_.request.kind = CameraCommand::CameraCommand_sensor_command_PRESENT;
  command_.request.u.sensor_command.kind = SensorCommand::set_state_PRESENT;
  command_.request.u.sensor_command.u.set_state = state;

  return issue_command();
}

ResultCode
CameraClient::set_rate(SensorRate rate)
{
  command_.request.kind = CameraCommand::CameraCommand_sensor_command_PRESENT;
  command_.request.u.sensor_command.kind = SensorCommand::set_rate_PRESENT;
  command_.request.u.sensor_command.u.set_rate = rate;

  return issue_command();
}

ResultCode
CameraClient::set_exposure(ExposureTime exposure_time)
{
  command_.request.kind = CameraCommand::set_exposure_PRESENT;
  command_.request.u.set_exposure = exposure_time;

  return issue_command();
}

ResultCode
CameraClient::set_gain(SensorGain gain)
{
  command_.request.kind = CameraCommand::set_gain_PRESENT;
  command_.request.u.set_gain = gain;

  return issue_command();
}

ResultCode
CameraClient::set_auto_exposure(bool auto_exposure)
{
  command_.request.kind = CameraCommand::set_auto_exposure_PRESENT;
  command_.request.u.set_auto_exposure = auto_exposure;

  return issue_command();
}

ResultCode
CameraClient::set_exposure_limit(SensorGain exposure_limit)
{
  command_.request.kind = CameraCommand::set_exposure_limit_PRESENT;
  command_.request.u.set_exposure_limit = exposure_limit;

  return issue_command();
}

ResultCode
CameraClient::set_gain_limit(SensorGain gain_limit)
{
  command_.request.kind = CameraCommand::set_gain_limit_PRESENT;
  command_.request.u.set_gain_limit = gain_limit;

  return issue_command();
}

ResultCode
CameraClient::set_region(PlanarRegion region)
{
  command_.request.kind = CameraCommand::CameraCommand_set_region_PRESENT;
  command_.request.u.set_region = region;

  return issue_command();
}

ResultCode
CameraClient::set_flash_enabled(bool flash_enabled)
{
  command_.request.kind = CameraCommand::set_flash_illumination_PRESENT;
  command_.request.u.set_flash_illumination = flash_enabled;

  return issue_command();
}

ResultCode
CameraClient::set_flash_strength(FlashStrength flash_strength)
{
  command_.request.kind = CameraCommand::set_flash_strength_PRESENT;
  command_.request.u.set_flash_strength = flash_strength;

  return issue_command();
}

ResultCode
CameraClient::set_pattern_enabled(bool pattern_enabled)
{
  command_.request.kind = CameraCommand::set_pattern_illumination_PRESENT;
  command_.request.u.set_pattern_illumination = pattern_enabled;

  return issue_command();
}

ResultCode
CameraClient::set_illumination_pattern(IlluminationPattern pattern)
{
  command_.request.kind = CameraCommand::set_illumination_pattern_PRESENT;
  command_.request.u.set_illumination_pattern = pattern;

  return issue_command();
}

////////////////////////////////////////////////////////////////////////////////

struct F {
  F()
    : id(1),
      resx(800),
      resy(600),
      context(Context::Create()),
      camera(context, id, resx, resy),
      client(context, id)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    camera.Start();
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    camera.Stop();
  }

  const NodeID id;
  const int resx, resy;

  Context::Ptr context;
  EmulatedCamera camera;
  CameraClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_creation)
{
  BOOST_CHECK_EQUAL(camera.node(), id);
  BOOST_CHECK_EQUAL(camera.state(), inactive);
  BOOST_CHECK_EQUAL(camera.rate(), 0.0);
  BOOST_CHECK_EQUAL(camera.resolution_x(), resx);
  BOOST_CHECK_EQUAL(camera.resolution_y(), resy);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_command)
{
  BOOST_CHECK_EQUAL(camera.state(), inactive);
  client.set_state(activate);
  BOOST_CHECK_EQUAL(camera.state(), standby);

  BOOST_CHECK_EQUAL(camera.rate(), 0.0);
  client.set_rate(0.1);
  BOOST_CHECK_EQUAL(camera.rate(), 0.1);

  BOOST_CHECK_CLOSE(camera.exposure(), 0.0, 1e-6);
  client.set_exposure(0.01);
  BOOST_CHECK_CLOSE(camera.exposure(), 0.01, 1e-6);

  BOOST_CHECK_CLOSE(camera.gain(), 1.0, 1e-6);
  client.set_gain(2.0);
  BOOST_CHECK_CLOSE(camera.gain(), 2.0, 1e-6);

  BOOST_CHECK_EQUAL(camera.auto_exposure(), false);
  client.set_auto_exposure(true);
  BOOST_CHECK_EQUAL(camera.auto_exposure(), true);

  BOOST_CHECK_CLOSE(camera.exposure_limit(), 0.0, 1e-6);
  client.set_exposure_limit(0.1);
  BOOST_CHECK_CLOSE(camera.exposure_limit(), 0.1, 1e-6);

  BOOST_CHECK_CLOSE(camera.gain_limit(), 1.0, 1e-6);
  client.set_gain_limit(3.0);
  BOOST_CHECK_CLOSE(camera.gain_limit(), 3.0, 1e-6);

  PlanarRegion r1 = {300, 200, 150, 100};

  client.set_region(r1);

  PlanarRegion r2 = camera.region();

  BOOST_CHECK_EQUAL(r1.size_x, r2.size_x);
  BOOST_CHECK_EQUAL(r1.size_y, r2.size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, r2.offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, r2.offset_y);

  BOOST_CHECK_EQUAL(camera.flash_enabled(), false);
  client.set_flash_enabled(true);
  BOOST_CHECK_EQUAL(camera.flash_enabled(), true);

  BOOST_CHECK_CLOSE(camera.flash_strength(), 0.0, 1e-6);
  client.set_flash_strength(0.75);
  BOOST_CHECK_CLOSE(camera.flash_strength(), 0.75, 1e-6);

  BOOST_CHECK_EQUAL(camera.pattern_enabled(), false);
  client.set_pattern_enabled(true);
  BOOST_CHECK_EQUAL(camera.pattern_enabled(), true);

  BOOST_CHECK_EQUAL(camera.illumination_pattern(), 0);
  client.set_illumination_pattern(10);
  BOOST_CHECK_EQUAL(camera.illumination_pattern(), 10);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_configuration_query)
{
  SensorRate rate = 0.1;
  ExposureTime exposure = 0.01;
  SensorGain gain = 2.0;
  bool auto_exposure = true;
  ExposureTime exposure_limit = 0.1;
  SensorGain gain_limit = 3.0;
  PlanarRegion region = {300, 200, 150, 100};
  bool flash_enabled = true;
  FlashStrength flash_strength = 0.75;
  bool pattern_enabled = true;
  IlluminationPattern illumination_pattern = 10;

  client.set_state(activate);
  client.set_rate(rate);
  client.set_exposure(exposure);
  client.set_gain(gain);
  client.set_auto_exposure(auto_exposure);
  client.set_exposure_limit(exposure_limit);
  client.set_gain_limit(gain_limit);
  client.set_region(region);
  client.set_flash_enabled(flash_enabled);
  client.set_flash_strength(flash_strength);
  client.set_pattern_enabled(pattern_enabled);
  client.set_illumination_pattern(illumination_pattern);

  client.load_configuration();

  BOOST_CHECK_CLOSE(exposure, client.exposure(), 1e-6);
  BOOST_CHECK_CLOSE(gain, client.gain(), 1e-6);
  BOOST_CHECK_EQUAL(auto_exposure, client.auto_exposure());
  BOOST_CHECK_CLOSE(exposure_limit, client.exposure_limit(), 1e-6);
  BOOST_CHECK_CLOSE(gain_limit, client.gain_limit(), 1e-6);
  BOOST_CHECK_EQUAL(region.size_x, client.region().size_x);
  BOOST_CHECK_EQUAL(region.size_y, client.region().size_y);
  BOOST_CHECK_EQUAL(region.offset_x, client.region().offset_x);
  BOOST_CHECK_EQUAL(region.offset_y, client.region().offset_y);
  BOOST_CHECK_EQUAL(flash_enabled, client.flash_enabled());
  BOOST_CHECK_CLOSE(flash_strength, client.flash_strength(), 1e-6);
  BOOST_CHECK_EQUAL(pattern_enabled, client.pattern_enabled());
  BOOST_CHECK_EQUAL(illumination_pattern, client.illumination_pattern());
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
