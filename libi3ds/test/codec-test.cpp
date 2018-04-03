///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE codec_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include "sensor.hpp"
#include "communication.hpp"
#include "codec.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_null)
{
  NullCodec::Data a, b;

  Message msg;

  Encode<NullCodec>(msg, a);

  BOOST_CHECK_EQUAL(msg.size(), 0);

  Decode<NullCodec>(msg, b);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_status)
{
  SensorStatusCodec::Data a, b;
  SensorStatusCodec::Initialize(a);

  a.state = inactive;
  a.temperature.kelvin = 250.0;

  set_string(a.message, "Hello world!");

  Message msg;

  Encode<SensorStatusCodec>(msg, a);
  Decode<SensorStatusCodec>(msg, b);

  BOOST_CHECK_EQUAL(b.state, a.state);
  BOOST_CHECK_CLOSE(b.temperature.kelvin, a.temperature.kelvin, 0.01);
  BOOST_CHECK_EQUAL(to_string(b.message), to_string(a.message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command)
{
  SensorCommandCodec::Data a, b;
  SensorCommandCodec::Initialize(a);

  a.kind = SensorCommand::set_state_PRESENT;
  a.u.set_state = activate;

  Message msg;

  Encode<SensorCommandCodec>(msg, a);
  Decode<SensorCommandCodec>(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::set_state_PRESENT);
  BOOST_CHECK_EQUAL(b.u.set_state, activate);

  a.kind = SensorCommand::set_rate_PRESENT;
  a.u.set_rate = 0.01;

  Encode<SensorCommandCodec>(msg, a);
  Decode<SensorCommandCodec>(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::set_rate_PRESENT);
  BOOST_CHECK_CLOSE(b.u.set_rate, 0.01, 1.0e-12);

  a.kind = SensorCommand::set_count_PRESENT;
  a.u.set_count = 3;

  Encode<SensorCommandCodec>(msg, a);
  Decode<SensorCommandCodec>(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::set_count_PRESENT);
  BOOST_CHECK_EQUAL(b.u.set_count, 3);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command_response)
{
  SensorResponseCodec::Data a, b;
  SensorResponseCodec::Initialize(a);

  a.result = success;
  set_string(a.message, "OK");

  Message msg;

  Encode<SensorResponseCodec>(msg, a);
  Decode<SensorResponseCodec>(msg, b);

  BOOST_CHECK_EQUAL(b.result, a.result);
  BOOST_CHECK_EQUAL(to_string(b.message), to_string(a.message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_configuration)
{
  SensorConfigurationCodec::Data a, b;
  SensorConfigurationCodec::Initialize(a);

  a.config_rate = 0.1;
  a.config_count = 3;

  Message msg;

  Encode<SensorConfigurationCodec>(msg, a);
  Decode<SensorConfigurationCodec>(msg, b);

  BOOST_CHECK_CLOSE(b.config_rate, a.config_rate, 0.01);
  BOOST_CHECK_EQUAL(b.config_count, a.config_count);
}
