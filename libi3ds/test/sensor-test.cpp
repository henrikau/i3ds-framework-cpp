///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE sensor_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "codec.hpp"
#include "sensor.hpp"

using namespace i3ds;

class TestSensor : public Sensor
{
public:

  TestSensor(SensorID id) : Sensor(id) {};

  void test_callback_and_clear(std::string callback);
  void test_no_callback();

  virtual double get_temperature() const {return 300.0;}

protected:

  std::vector<std::string> callbacks;

  void log(std::string callback) {callbacks.push_back(callback);}

  virtual void do_activate() {log("do_activate");}
  virtual void do_start() {log("do_start");}
  virtual void do_stop() {log("do_stop");}
  virtual void do_deactivate() {log("do_deactivate");}

  virtual bool support_rate(SensorRate rate);
};

class TestClient
{
public:

  TestClient(Sensor& sensor) : sensor_(sensor) {}

  CommandResult issue_state_command(StateCommand cmd);

  void test_legal_state_command(StateCommand cmd);
  void test_illegal_state_command(StateCommand cmd);

  CommandResult issue_rate_command(SensorRate rate);

  void test_legal_rate_command(SensorRate rate);
  void test_illegal_rate_command(SensorRate rate, CommandResult error);
  void test_unsupported_rate_command(SensorRate rate, CommandResult error);

private:

  CommandResult issue_command();

  Sensor& sensor_;

  Encoder<SensorCommandCodec> command_;
  Decoder<SensorCommandResponseCodec> response_;
};

void TestSensor::test_callback_and_clear(std::string callback)
{
  BOOST_CHECK_EQUAL(callbacks.size(), 1);
  BOOST_CHECK_EQUAL(callbacks[0], callback);

  callbacks.clear();
}

void TestSensor::test_no_callback()
{
  BOOST_CHECK_EQUAL(callbacks.size(), 0);
}

bool TestSensor::support_rate(SensorRate rate)
{
  log("support_rate");

  return (0.1 <= rate && rate <= 10.0);
}

CommandResult TestClient::issue_command()
{
  Message req;

  command_.Encode(req);

  req.sensor_id = sensor_.get_id();
  req.endpoint_id = Sensor::COMMAND;

  Message res = sensor_.handle_request(req);

  BOOST_CHECK_EQUAL(res.sensor_id, sensor_.get_id());
  BOOST_CHECK_EQUAL(res.endpoint_id, Sensor::COMMAND);

  BOOST_REQUIRE_GT(res.size, 0);
  BOOST_REQUIRE(res.data);

  response_.Decode(res);

  return response_.data.result;
}

CommandResult TestClient::issue_state_command(StateCommand sc)
{
  command_.data.kind = SensorCommand::command_PRESENT;
  command_.data.u.command = sc;

  return issue_command();
}

void TestClient::test_legal_state_command(StateCommand sc)
{
  CommandResult r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_state_command(StateCommand sc)
{
  CommandResult r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, error_illegal_state);
}

CommandResult TestClient::issue_rate_command(SensorRate rate)
{
  command_.data.kind = SensorCommand::set_rate_PRESENT;
  command_.data.u.set_rate = rate;

  return issue_command();
}

void TestClient::test_legal_rate_command(SensorRate rate)
{
  CommandResult r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_rate_command(SensorRate rate, CommandResult error)
{
  CommandResult r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, error);
}

void TestClient::test_unsupported_rate_command(SensorRate rate, CommandResult error)
{
  CommandResult r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, error);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_creation)
{
  SensorID id = 1;

  TestSensor s(id);

  BOOST_CHECK_EQUAL(s.get_id(), id);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  BOOST_CHECK_EQUAL(s.get_rate(), 0.0);
  BOOST_CHECK_CLOSE(s.get_temperature(), 300.0, 0.01);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_state_command)
{
  TestSensor s(1);
  TestClient c(s);

  // Test commands from INACTIVE.
  c.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  s.test_no_callback();

  c.test_illegal_state_command(stop);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  s.test_no_callback();

  c.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  s.test_no_callback();

  c.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  s.test_callback_and_clear("do_activate");

  // Test commands from STANDBY.
  c.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  s.test_no_callback();

  c.test_legal_state_command(deactivate);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  s.test_callback_and_clear("do_deactivate");

  c.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  s.test_callback_and_clear("do_activate");

  c.test_legal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), operational);
  s.test_callback_and_clear("do_start");

  // Test commands from OPERATIONAL.
  c.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), operational);
  s.test_no_callback();

  c.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(s.get_state(), operational);
  s.test_no_callback();

  c.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), operational);
  s.test_no_callback();

  c.test_legal_state_command(stop);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  s.test_callback_and_clear("do_stop");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_rate_command)
{
  TestSensor s(1);
  TestClient c(s);

  // Test from INACTIVE (illegal).
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  c.test_illegal_rate_command(1.0, error_illegal_state);
  s.test_no_callback();

  // Test from STANDBY (legal).
  c.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  c.test_legal_rate_command(1.0);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 1.0, 1e-9);

  c.test_unsupported_rate_command(100.0, error_unsupported_value);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 1.0, 1e-9);

  c.test_legal_rate_command(0.1);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 0.1, 1e-9);

  c.test_unsupported_rate_command(0.01, error_unsupported_value);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  c.test_unsupported_rate_command(100.0, error_unsupported_value);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  // Test from OPERATIONAL (illegal).
  c.test_legal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  c.test_illegal_rate_command(1.0, error_illegal_state);
}
