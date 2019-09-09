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

#include <i3ds/codec.hpp>
#include <i3ds/sensor.hpp>
#include <i3ds/sensor_client.hpp>

using namespace i3ds;

class TestSensor : public Sensor
{
public:

  typedef std::shared_ptr<TestSensor> Ptr;

  TestSensor(NodeID node);

  void test_callback_and_clear(std::string callback);
  void test_no_callback();

  virtual double temperature() const {return 300.0;}

  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  std::vector<std::string> callbacks;

  void log(std::string callback) {callbacks.push_back(callback);}

  virtual void do_activate() {log("do_activate");}
  virtual void do_start() {log("do_start");}
  virtual void do_stop() {log("do_stop");}
  virtual void do_deactivate() {log("do_deactivate");}
};

class TestClient : public SensorClient
{
public:

  typedef std::shared_ptr<TestClient> Ptr;

  TestClient(Context::Ptr context, NodeID sensor);

  ResultCode issue_state_command(StateCommand cmd);

  void test_legal_state_command(StateCommand cmd);
  void test_illegal_state_command(StateCommand cmd);

  ResultCode issue_sample_command(SamplePeriod period, BatchSize batch_size);

  void test_legal_sample_command(SamplePeriod period, BatchSize batch_size);
  void test_illegal_sample_command(SamplePeriod period, BatchSize batch_size, ResultCode error);
  void test_unsupported_sample_command(SamplePeriod period, BatchSize batch_size, ResultCode error);
};

TestSensor::TestSensor(NodeID node) : Sensor(node)
{
  set_device_name("Test sensor");
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

bool TestSensor::is_sampling_supported(SampleCommand sample)
{
  log("is_sampling_supported");

  return (1 <= sample.batch_size && sample.batch_size <= 10) &&
         (1000 <= sample.period && sample.period <= 1000000) &&
         (sample.batch_count == 0);
}

TestClient::TestClient(Context::Ptr context, NodeID sensor)
  : SensorClient(context, sensor)
{
}

ResultCode TestClient::issue_state_command(StateCommand sc)
{
  Sensor::StateService::Data command;
  Sensor::StateService::Initialize(command);

  command.request = sc;

  Call<Sensor::StateService>(command);

  return command.response.result;
}

void TestClient::test_legal_state_command(StateCommand sc)
{
  ResultCode r = issue_state_command(sc);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_state_command(StateCommand sc)
{
  try
    {
      issue_state_command(sc);
      BOOST_CHECK(false);
    }
  catch (CommandError& e)
    {
      BOOST_CHECK_EQUAL(e.result(), error_state);
    }
}

ResultCode TestClient::issue_sample_command(SamplePeriod period, BatchSize batch_size)
{
  Sensor::SampleService::Data command;
  Sensor::SampleService::Initialize(command);

  command.request.period = period;
  command.request.batch_size = batch_size;
  command.request.batch_count = 0;

  Call<Sensor::SampleService>(command);

  return command.response.result;
}

void TestClient::test_legal_sample_command(SamplePeriod period, BatchSize batch_size)
{
  ResultCode r = issue_sample_command(period, batch_size);

  BOOST_CHECK_EQUAL(r, success);
}

void TestClient::test_illegal_sample_command(SamplePeriod period, BatchSize batch_size, ResultCode error)
{
  try
    {
      issue_sample_command(period, batch_size);
      BOOST_CHECK(false);
    }
  catch (CommandError& e)
    {
      BOOST_CHECK_EQUAL(e.result(), error);
    }
}

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : id(1),
      context(Context::Create()),
      server(context)
  {
    BOOST_TEST_MESSAGE("setup fixture");

    sensor = std::make_shared<TestSensor>(id);
    client = std::make_shared<TestClient>(context, id);

    sensor->Attach(server);
    client->set_timeout(1000);

    server.Start();
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );

    server.Stop();
  }

  const NodeID id;
  Context::Ptr context;
  Server server;

  TestSensor::Ptr sensor;
  TestClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_creation)
{
  BOOST_CHECK_EQUAL(sensor->node(), id);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  BOOST_CHECK_EQUAL(sensor->period(), 1000000);
  BOOST_CHECK_EQUAL(sensor->batch_size(), 1);
  BOOST_CHECK_EQUAL(sensor->batch_count(), 0);
  BOOST_CHECK_CLOSE(sensor->temperature(), 300.0, 0.01);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_state_command)
{
  // Test commands from INACTIVE.
  client->test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  sensor->test_no_callback();

  client->test_illegal_state_command(stop);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  sensor->test_no_callback();

  client->test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  sensor->test_no_callback();

  client->test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  sensor->test_callback_and_clear("do_activate");

  // Test commands from STANDBY.
  client->test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  sensor->test_no_callback();

  client->test_legal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  sensor->test_callback_and_clear("do_deactivate");

  client->test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  sensor->test_callback_and_clear("do_activate");

  client->test_legal_state_command(start);
  BOOST_CHECK_EQUAL(sensor->state(), operational);
  sensor->test_callback_and_clear("do_start");

  // Test commands from OPERATIONAL.
  client->test_illegal_state_command(start);
  BOOST_CHECK_EQUAL(sensor->state(), operational);
  sensor->test_no_callback();

  client->test_illegal_state_command(deactivate);
  BOOST_CHECK_EQUAL(sensor->state(), operational);
  sensor->test_no_callback();

  client->test_illegal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor->state(), operational);
  sensor->test_no_callback();

  client->test_legal_state_command(stop);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  sensor->test_callback_and_clear("do_stop");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(sensor_sample_command)
{
  // Test from INACTIVE (illegal).
  BOOST_CHECK_EQUAL(sensor->state(), inactive);

  client->test_illegal_sample_command(10000, 1, error_state);
  sensor->test_no_callback();

  // Test from STANDBY (legal).
  client->test_legal_state_command(activate);
  BOOST_CHECK_EQUAL(sensor->state(), standby);

  client->test_legal_sample_command(1000, 2);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  BOOST_CHECK_EQUAL(sensor->period(), 1000);
  BOOST_CHECK_EQUAL(sensor->batch_size(), 2);

  client->load_all();
  BOOST_CHECK_EQUAL(client->period(), 1000);
  BOOST_CHECK_EQUAL(client->batch_size(), 2);

  client->test_illegal_sample_command(2000000, 1, error_value);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  BOOST_CHECK_EQUAL(sensor->period(), 1000);
  BOOST_CHECK_EQUAL(sensor->batch_size(), 2);

  client->test_legal_sample_command(2000, 3);
  BOOST_CHECK_EQUAL(sensor->state(), standby);
  BOOST_CHECK_EQUAL(sensor->period(), 2000);
  BOOST_CHECK_EQUAL(sensor->batch_size(), 3);

  client->test_illegal_sample_command(10, 1, error_value);
  BOOST_CHECK_EQUAL(sensor->state(), standby);

  client->test_illegal_sample_command(2000000, 1, error_value);
  BOOST_CHECK_EQUAL(sensor->state(), standby);

  client->test_illegal_sample_command(1000, 0, error_value);
  BOOST_CHECK_EQUAL(sensor->state(), standby);

  client->test_illegal_sample_command(1000, 100, error_value);
  BOOST_CHECK_EQUAL(sensor->state(), standby);

  // Test from OPERATIONAL (illegal).
  client->test_legal_state_command(start);
  BOOST_CHECK_EQUAL(sensor->state(), operational);

  client->test_illegal_sample_command(10000, 1, error_state);

}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_device_name)
{
  client->test_legal_state_command(activate);
  client->load_all();
  BOOST_CHECK_EQUAL(client->device_name(), "Test sensor");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
