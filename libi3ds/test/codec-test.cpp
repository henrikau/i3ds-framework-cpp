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
#include "message.hpp"
#include "codec.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_null)
{
  Encoder<NullCodec> encoder;
  Decoder<NullCodec> decoder;

  Message msg;

  encoder.Encode(msg);

  BOOST_CHECK_EQUAL(msg.size, 0);

  decoder.Decode(msg);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_status)
{
  Encoder<SensorStatusCodec> encoder;
  Decoder<SensorStatusCodec> decoder;

  encoder.data.timestamp.microseconds = 123456789;
  encoder.data.timestamp.usecPerSec = 1000000;

  encoder.data.sensor_id = 1;
  encoder.data.sensor_state = inactive;
  encoder.data.sensor_temperature.kelvin = 250.0;

  set_string(encoder.data.status_message, "Hello world!");

  Message msg;

  encoder.Encode(msg);
  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.timestamp.microseconds, encoder.data.timestamp.microseconds);
  BOOST_CHECK_EQUAL(decoder.data.sensor_id, encoder.data.sensor_id);
  BOOST_CHECK_EQUAL(decoder.data.sensor_state, encoder.data.sensor_state);
  BOOST_CHECK_CLOSE(decoder.data.sensor_temperature.kelvin, encoder.data.sensor_temperature.kelvin, 0.01);
  BOOST_CHECK_EQUAL(to_string(decoder.data.status_message), to_string(encoder.data.status_message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command)
{
  Encoder<SensorCommandCodec> encoder;
  Decoder<SensorCommandCodec> decoder;

  encoder.data.kind = SensorCommand::command_PRESENT;
  encoder.data.u.command = activate;

  Message msg;

  encoder.Encode(msg);
  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.kind, SensorCommand::command_PRESENT);
  BOOST_CHECK_EQUAL(decoder.data.u.command, activate);

  encoder.data.kind = SensorCommand::set_rate_PRESENT;
  encoder.data.u.set_rate = 0.01;

  encoder.Encode(msg);
  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.kind, SensorCommand::set_rate_PRESENT);
  BOOST_CHECK_CLOSE(decoder.data.u.set_rate, 0.01, 1.0e-12);

  encoder.data.kind = SensorCommand::set_count_PRESENT;
  encoder.data.u.set_count = 3;

  encoder.Encode(msg);
  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.kind, SensorCommand::set_count_PRESENT);
  BOOST_CHECK_EQUAL(decoder.data.u.set_count, 3);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_command_response)
{
  Encoder<SensorCommandResponseCodec> encoder;

  encoder.data.result = success;
  set_string(encoder.data.message, "OK");

  Message msg;

  encoder.Encode(msg);

  Decoder<SensorCommandResponseCodec> decoder;

  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.result, encoder.data.result);
  BOOST_CHECK_EQUAL(to_string(decoder.data.message), to_string(encoder.data.message));
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(codec_sensor_configuration)
{
  Encoder<SensorConfigurationCodec> encoder;

  encoder.data.sensor_state = inactive;
  encoder.data.config_rate = 0.1;
  encoder.data.config_count = 3;

  Message msg;

  encoder.Encode(msg);

  Decoder<SensorConfigurationCodec> decoder;

  decoder.Decode(msg);

  BOOST_CHECK_EQUAL(decoder.data.sensor_state, encoder.data.sensor_state);
  BOOST_CHECK_CLOSE(decoder.data.config_rate, encoder.data.config_rate, 0.01);
  BOOST_CHECK_EQUAL(decoder.data.config_count, encoder.data.config_count);
}
