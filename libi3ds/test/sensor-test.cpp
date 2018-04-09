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

  TestSensor(Context::Ptr context, NodeID id);

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

  virtual bool support_rate(SampleRate rate);
};

class TestClient : public Client
{
public:

  TestClient(Context::Ptr context, NodeID sensor);

  ResultCode issue_state_command(StateCommand cmd);

  void test_legal_state_command(StateCommand cmd);
  void test_illegal_state_command(StateCommand cmd);

  ResultCode issue_rate_command(SampleRate rate);

  void test_legal_rate_command(SampleRate rate);
  void test_illegal_rate_command(SampleRate rate, ResultCode error);
  void test_unsupported_rate_command(SampleRate rate, ResultCode error);
};

TestSensor::TestSensor(Context::Ptr context, NodeID id) : Sensor(context, id)
{
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

bool TestSensor::support_rate(SampleRate rate)
{
  log("support_rate");

  return (1000 <= rate && rate <= 1000000);
}

TestClient::TestClient(Context::Ptr context, NodeID sensor)
  : Client(context, sensor)
{
}

ResultCode TestClient::issue_state_command(StateCommand sc)
{
  Sensor::StateService::Data command;
  Sensor::StateService::Initialize(command);

  command.request = sc;

  bool result = Call<Sensor::StateService>(command, 1000);

  BOOST_CHECK(result);

  return command.response.result;
}

void TestClient::test_legal_state_command(StateCommand sc)
{
  ResultCode r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_state_command(StateCommand sc)
{
  ResultCode r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, error_state);
}

ResultCode TestClient::issue_rate_command(SampleRate rate)
{
  Sensor::SampleService::Data command;
  Sensor::SampleService::Initialize(command);

  command.request.rate = rate;
  command.request.count = 0;

  bool result = Call<Sensor::SampleService>(command, 1000);

  BOOST_CHECK(result);

  return command.response.result;
}

void TestClient::test_legal_rate_command(SampleRate rate)
{
  ResultCode r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_rate_command(SampleRate rate, ResultCode error)
{
  ResultCode r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, error);
}

void TestClient::test_unsupported_rate_command(SampleRate rate, ResultCode error)
{
  ResultCode r = issue_rate_command(rate);

  BOOST_CHECK_EQUAL(r, error);
}

////////////////////////////////////////////////////////////////////////////////

struct F {
  F()
    : id(1),
      context(Context::Create()),
      sensor(context, id),
      client(context, id)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    sensor.Start();
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    sensor.Stop();
    client.Stop();
  }

  const NodeID id;
  Context::Ptr context;
  TestSensor sensor;
  TestClient client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_creation)
{
  BOOST_CHECK_EQUAL(sensor.node(), id);
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

  client.test_illegal_rate_command(10000, error_state);
  sensor.test_no_callback();

  // Test from STANDBY (legal).
  client.test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  client.test_legal_rate_command(1000);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_EQUAL(sensor.rate(), 1000);

  client.test_unsupported_rate_command(2000000, error_unsupported);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_EQUAL(sensor.rate(), 1000);

  client.test_legal_rate_command(2000);
  BOOST_CHECK_EQUAL(sensor.state(), standby);
  BOOST_CHECK_EQUAL(sensor.rate(), 2000);

  client.test_unsupported_rate_command(10, error_unsupported);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  client.test_unsupported_rate_command(2000000, error_unsupported);
  BOOST_CHECK_EQUAL(sensor.state(), standby);

  // Test from OPERATIONAL (illegal).
  client.test_legal_state_command(start);
  BOOST_CHECK_EQUAL(sensor.state(), operational);

  client.test_illegal_rate_command(10000, error_state);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
