///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE communication_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "communication.hpp"

#include <iostream>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

void Initialize(Message& msg, Address address, const std::string& data)
{
  msg.set_payload((byte*) data.c_str(), data.size());
  msg.set_address(address);
}

void Initialize(Message& msg, Address address, size_t size)
{
  byte* data = (byte*) malloc(size);

  for (unsigned int i = 0; i < size; i++)
    {
      data[i] = i % 0xFF;
    }
  
  msg.set_payload(data, size);
  msg.set_address(address);

  free(data);
}

void check_message(const Message& msg, const Address& address, const std::string data)
{
  Address a = msg.address();
  
  BOOST_CHECK_EQUAL(a.sensor, address.sensor);
  BOOST_CHECK_EQUAL(a.endpoint, address.endpoint);
  BOOST_CHECK_EQUAL(msg.size(), data.size());
  BOOST_CHECK_EQUAL(memcmp(msg.data(), data.c_str(), msg.size()), 0);
}

////////////////////////////////////////////////////////////////////////////////

struct F {
  F() : address(1, 1), zmq_address("tcp://127.0.0.1:5555") { BOOST_TEST_MESSAGE( "setup fixture" ); }
  ~F() { BOOST_TEST_MESSAGE( "teardown fixture" ); }

  const Address address;
  const std::string zmq_address;
  Context context;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(create_id_message_test)
{
  std::vector<Address> addresses;

  addresses.push_back(Address(0,0));
  addresses.push_back(Address(32413,45));
  addresses.push_back(Address(16777215,255));
  addresses.push_back(Address(1,1));
  addresses.push_back(Address(10485760,45));
  addresses.push_back(Address(32413,16));
  addresses.push_back(Address(12,4));

  for (auto a : addresses) {
    Message msg;
    msg.set_address(a);
    Address aa = msg.address();
    BOOST_CHECK_EQUAL(a.sensor, aa.sensor);
    BOOST_CHECK_EQUAL(a.endpoint, aa.endpoint);
  }
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(create_message)
{
  Message msg;

  std::string data = "Hello world!";
  
  Initialize(msg, address, data);
  
  BOOST_CHECK_EQUAL(data.size(), msg.size());
  BOOST_CHECK_EQUAL(memcmp(data.c_str(), msg.data(), data.size()), 0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(request_response_pattern)
{
  Socket::Ptr server = context.Server();
  Socket::Ptr client = context.Client();

  server->Bind(zmq_address);
  client->Connect(zmq_address);
  
  Message req, res, copy;

  Initialize(req, address, "Hello?");

  client->Send(req);
  server->Receive(copy);

  check_message(copy, address, "Hello?");

  Initialize(res, address, "World!");

  server->Send(res);
  client->Receive(copy);

  check_message(copy, address, "World!");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(publish_subscribe_pattern)
{
  Socket::Ptr publisher = context.Publisher();
  Socket::Ptr subscriber = context.Subscriber();

  publisher->Bind(zmq_address);

  subscriber->Connect(zmq_address);
  subscriber->Filter(address);

  // Needed to let filter take effect before sending.
  usleep(10000);
  
  Message msg, copy;

  Initialize(msg, address, "Hello world!");

  publisher->Send(msg);
  subscriber->Receive(copy);

  check_message(copy, address, "Hello world!");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
