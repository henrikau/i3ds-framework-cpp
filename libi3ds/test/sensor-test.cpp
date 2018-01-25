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

#include "sensor.hpp"

using namespace i3ds;

class TestSensor : public Sensor
{
 public:

  TestSensor(SensorID id) : Sensor(id) {};

  CommandResult issue_state_command(StateCommand cmd);

  void test_legal_state_command(StateCommand cmd, std::string callback);
  void test_illegal_state_command(StateCommand cmd);

  CommandResult issue_rate_command(SensorRate rate);

  void test_legal_rate_command(SensorRate rate);
  void test_illegal_rate_command(SensorRate rate, CommandResult error, int n);

  std::vector<std::string> callbacks;

 protected:

  void log(std::string callback) {callbacks.push_back(callback);}
  void clear() {callbacks.clear();}

  virtual void do_activate() {log("do_activate");}
  virtual void do_start() {log("do_start");}
  virtual void do_stop() {log("do_stop");}
  virtual void do_deactivate() {log("do_deactivate");}

  virtual bool update_rate(SensorRate rate);
};

bool TestSensor::update_rate(SensorRate rate)
{
  log("update_rate");

  return (0.1 <= rate && rate <= 10.0);
}

CommandResult TestSensor::issue_state_command(StateCommand sc)
{
  SensorCommand cmd;

  SensorCommand_Initialize(&cmd);

  cmd.kind = SensorCommand::command_PRESENT;
  cmd.u.command = sc;

  return handle_sensor_command(cmd);
}

void TestSensor::test_legal_state_command(StateCommand sc, std::string callback)
{
  CommandResult r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, success);
  BOOST_CHECK_EQUAL(callbacks.size(), 1);
  BOOST_CHECK_EQUAL(callbacks[0], callback);

  clear();
}

void TestSensor::test_illegal_state_command(StateCommand sc)
{
  CommandResult r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, error_illegal_state);
  BOOST_CHECK_EQUAL(callbacks.size(), 0);

  clear();
}

CommandResult TestSensor::issue_rate_command(SensorRate rate)
{
  SensorCommand cmd;

  SensorCommand_Initialize(&cmd);

  cmd.kind = SensorCommand::set_rate_PRESENT;
  cmd.u.set_rate = rate;

  return handle_sensor_command(cmd);
}

void TestSensor::test_legal_rate_command(SensorRate rate)
{
  CommandResult r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, success);
  BOOST_CHECK_EQUAL(callbacks.size(), 1);
  BOOST_CHECK_EQUAL(callbacks[0], "update_rate");

  clear();
}

void TestSensor::test_illegal_rate_command(SensorRate rate, CommandResult error, int n)
{
  CommandResult r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, error);
  BOOST_CHECK_EQUAL(callbacks.size(), n);

  if (n > 0)
    {
      BOOST_CHECK_EQUAL(callbacks[0], "update_rate");
    }

  clear();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_creation)
{
  SensorID id = 1;

  TestSensor s(id);

  BOOST_CHECK_EQUAL(s.get_id(), id);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);
  BOOST_CHECK_EQUAL(s.get_rate(), 0.0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_state_command)
{
  TestSensor s(1);

  // Test commands from INACTIVE.
  s.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  s.test_illegal_state_command(stop);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  s.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  s.test_legal_state_command(activate, "do_activate");
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  // Test commands from STANDBY.
  s.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  s.test_legal_state_command(deactivate, "do_deactivate");
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  s.test_legal_state_command(activate, "do_activate");
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  s.test_legal_state_command(start, "do_start");
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  // Test commands from OPERATIONAL.
  s.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  s.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  s.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  s.test_legal_state_command(stop, "do_stop");
  BOOST_CHECK_EQUAL(s.get_state(), standby);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_rate_command)
{
  TestSensor s(1);

  // Test from INACTIVE (illegal).
  BOOST_CHECK_EQUAL(s.get_state(), inactive);

  s.test_illegal_rate_command(1.0, error_illegal_state, 0);
  s.test_illegal_rate_command(-1.0, error_illegal_state, 0);

  // Test from STANDBY (legal).
  s.test_legal_state_command(activate, "do_activate");
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  s.test_legal_rate_command(1.0);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 1.0, 1e-9);

  s.test_illegal_rate_command(-1.0, error_unsupported_value, 0);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 1.0, 1e-9);

  s.test_legal_rate_command(0.1);
  BOOST_CHECK_EQUAL(s.get_state(), standby);
  BOOST_CHECK_CLOSE(s.get_rate(), 0.1, 1e-9);

  s.test_illegal_rate_command(0.01, error_unsupported_value, 1);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  s.test_illegal_rate_command(100.0, error_unsupported_value, 1);
  BOOST_CHECK_EQUAL(s.get_state(), standby);

  // Test from OPERATIONAL (illegal).
  s.test_legal_state_command(start, "do_start");
  BOOST_CHECK_EQUAL(s.get_state(), operational);

  s.test_illegal_rate_command(1.0, error_illegal_state, 0);
  s.test_illegal_rate_command(-1.0, error_illegal_state, 0);
}
