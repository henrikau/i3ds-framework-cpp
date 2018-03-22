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

#include <thread>
#include <string>
#include <vector>
#include <iostream>

#include "codec.hpp"
#include "sensor.hpp"
#include "client.hpp"

using namespace i3ds;

class TestSensor : public Sensor
{
public:

  TestSensor(Context& context, SensorID id);

  void test_callback_and_clear(std::string callback);
  void test_no_callback();

  virtual double temperature() const {return 300.0;}

protected:

  std::vector<std::string> callbacks;

  void log(std::string callback) {callbacks.push_back(callback);}

  virtual void do_activate() {log("do_activate");}
  virtual void do_start() {log("do_start");}
  virtual void do_stop() {log("do_stop");}
  virtual void do_deactivate() {log("do_deactivate");}

  virtual bool support_rate(SensorRate rate);
};

class TestClient : public Client
{
public:

  TestClient(Context& context, SensorID sensor);

  CommandResult issue_state_command(StateCommand cmd);

  void test_legal_state_command(StateCommand cmd);
  void test_illegal_state_command(StateCommand cmd);

  CommandResult issue_rate_command(SensorRate rate);

  void test_legal_rate_command(SensorRate rate);
  void test_illegal_rate_command(SensorRate rate, CommandResult error);
  void test_unsupported_rate_command(SensorRate rate, CommandResult error);

private:

  CommandResult issue_command();

  Sensor::CommandService::Data command_;
};

TestSensor::TestSensor(Context& context, SensorID id) : Sensor(context, id)
{
  default_command_handler();
  default_status_handler();
  default_configuration_handler();
}

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

TestClient::TestClient(Context& context, SensorID sensor)
  : Client(context, sensor)
{
}

CommandResult TestClient::issue_command()
{
  bool result = Call<Sensor::CommandService>(Sensor::COMMAND, command_, 100);

  BOOST_CHECK(result);

  return command_.response.result;
}

CommandResult TestClient::issue_state_command(StateCommand sc)
{
  command_.request.kind = SensorCommand::command_PRESENT;
  command_.request.u.command = sc;

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
  command_.request.kind = SensorCommand::set_rate_PRESENT;
  command_.request.u.set_rate = rate;

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

struct F {
  F()
    : id(1),
      context(),
      sensor(context, id),
      client(context, id),
      worker(&TestSensor::Run, &sensor)
  {
    BOOST_TEST_MESSAGE("setup fixture");
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );

    context.Close();
    worker.join();
  }

  const SensorID id;
  Context context;
  TestSensor sensor;
  TestClient client;
  std::thread worker;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_creation)
{
  BOOST_CHECK_EQUAL(sensor.sensor(), id);
  BOOST_CHECK_EQUAL(sensor.state(), inactive);
  BOOST_CHECK_EQUAL(sensor.rate(), 0.0);
  BOOST_CHECK_CLOSE(sensor.temperature(), 300.0, 0.01);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_state_command)
{
  // Test commands from INACTIVE.
  client.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(sensor.state(), inactive);
  sensor.test_no_callback();

  client.test_illegal_state_command(stop);
  BOOST_CHECK_EQUAL(sensor.state(), inactive);
  sensor.test_no_callback();

  client.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor.state(), inactive);
  sensor.test_no_callback();

  client.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  sensor.test_callback_and_clear("do_activate");

  // Test commands from STANDBY.
  client.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  sensor.test_no_callback();

  client.test_legal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor.state(), inactive);
  sensor.test_callback_and_clear("do_deactivate");

  client.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  sensor.test_callback_and_clear("do_activate");

  client.test_legal_state_command(start);
  BOOST_CHECK_EQUAL(sensor.state(), operational);
  sensor.test_callback_and_clear("do_start");

  // Test commands from OPERATIONAL.
  client.test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(sensor.state(), operational);
  sensor.test_no_callback();

  client.test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor.state(), operational);
  sensor.test_no_callback();

  client.test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), operational);
  sensor.test_no_callback();

  client.test_legal_state_command(stop);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  sensor.test_callback_and_clear("do_stop");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_rate_command)
{
  // Test from INACTIVE (illegal).
  BOOST_CHECK_EQUAL(sensor.state(), inactive);

  client.test_illegal_rate_command(1.0, error_illegal_state);
  sensor.test_no_callback();

  // Test from STANDBY (legal).
  client.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  client.test_legal_rate_command(1.0);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_CLOSE(sensor.rate(), 1.0, 1e-9);

  client.test_unsupported_rate_command(100.0, error_unsupported_value);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_CLOSE(sensor.rate(), 1.0, 1e-9);

  client.test_legal_rate_command(0.1);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_CLOSE(sensor.rate(), 0.1, 1e-9);

  client.test_unsupported_rate_command(0.01, error_unsupported_value);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  client.test_unsupported_rate_command(100.0, error_unsupported_value);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  // Test from OPERATIONAL (illegal).
  client.test_legal_state_command(start);
  BOOST_CHECK_EQUAL(sensor.state(), operational);

  client.test_illegal_rate_command(1.0, error_illegal_state);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
