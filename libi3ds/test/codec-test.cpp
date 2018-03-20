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
  Encoder<NullCodec> encoder;
  Decoder<NullCodec> decoder;

  NullCodec::Data a, b;

  Message msg;

  encoder.Encode(msg, a);

  BOOST_CHECK_EQUAL(msg.size(), 0);

  decoder.Decode(msg, b);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_status)
{
  Encoder<SensorStatusCodec> encoder;
  Decoder<SensorStatusCodec> decoder;

  SensorStatusCodec::Data a, b;
  SensorStatusCodec::Initialize(a);

  a.timestamp.microseconds = 123456789;
  a.timestamp.usecPerSec = 1000000;

  a.sensor_id = 1;
  a.sensor_state = inactive;
  a.sensor_temperature.kelvin = 250.0;

  set_string(a.status_message, "Hello world!");

  Message msg;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.timestamp.microseconds, a.timestamp.microseconds);
  BOOST_CHECK_EQUAL(b.sensor_id, a.sensor_id);
  BOOST_CHECK_EQUAL(b.sensor_state, a.sensor_state);
  BOOST_CHECK_CLOSE(b.sensor_temperature.kelvin, a.sensor_temperature.kelvin, 0.01);
  BOOST_CHECK_EQUAL(to_string(b.status_message), to_string(a.status_message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command)
{
  Encoder<SensorCommandCodec> encoder;
  Decoder<SensorCommandCodec> decoder;

  SensorCommandCodec::Data a, b;
  SensorCommandCodec::Initialize(a);

  a.kind = SensorCommand::command_PRESENT;
  a.u.command = activate;

  Message msg;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::command_PRESENT);
  BOOST_CHECK_EQUAL(b.u.command, activate);

  a.kind = SensorCommand::set_rate_PRESENT;
  a.u.set_rate = 0.01;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::set_rate_PRESENT);
  BOOST_CHECK_CLOSE(b.u.set_rate, 0.01, 1.0e-12);

  a.kind = SensorCommand::set_count_PRESENT;
  a.u.set_count = 3;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.kind, SensorCommand::set_count_PRESENT);
  BOOST_CHECK_EQUAL(b.u.set_count, 3);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command_response)
{
  Encoder<SensorCommandResponseCodec> encoder;
  Decoder<SensorCommandResponseCodec> decoder;

  SensorCommandResponseCodec::Data a, b;
  SensorCommandResponseCodec::Initialize(a);

  a.result = success;
  set_string(a.message, "OK");

  Message msg;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.result, a.result);
  BOOST_CHECK_EQUAL(to_string(b.message), to_string(a.message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_configuration)
{
  Encoder<SensorConfigurationCodec> encoder;
  Decoder<SensorConfigurationCodec> decoder;

  SensorConfigurationCodec::Data a, b;
  SensorConfigurationCodec::Initialize(a);

  a.sensor_state = inactive;
  a.config_rate = 0.1;
  a.config_count = 3;

  Message msg;

  encoder.Encode(msg, a);
  decoder.Decode(msg, b);

  BOOST_CHECK_EQUAL(b.sensor_state, a.sensor_state);
  BOOST_CHECK_CLOSE(b.config_rate, a.config_rate, 0.01);
  BOOST_CHECK_EQUAL(b.config_count, a.config_count);
}
