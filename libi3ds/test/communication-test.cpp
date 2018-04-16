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

#include <iostream>
#include <map>

#include "i3ds/core/communication.hpp"

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
  
  BOOST_CHECK_EQUAL(a.node, address.node);
  BOOST_CHECK_EQUAL(a.endpoint, address.endpoint);
  BOOST_CHECK_EQUAL(msg.size(), data.size());
  BOOST_CHECK_EQUAL(memcmp(msg.data(), data.c_str(), msg.size()), 0);
}

////////////////////////////////////////////////////////////////////////////////

struct F {
  F()
    : address(1, 1),
      context(Context::Create())
  {
    BOOST_TEST_MESSAGE( "setup fixture" );
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
  }

  const Address address;
  Context::Ptr context;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

struct addr_pair
{
  Address a;
  std::string hex;
};

BOOST_AUTO_TEST_CASE(create_id_message_test)
{
  std::vector<addr_pair> addr_pairs;

  addr_pairs.push_back({Address(0,0), "00000000"});
  addr_pairs.push_back({Address(32413,45),"007E9D2D"});
  addr_pairs.push_back({Address(16777215,255), "FFFFFFFF"});
  addr_pairs.push_back({Address(1,1), "00000101"});
  addr_pairs.push_back({Address(10485760,45), "A000002D"});
  addr_pairs.push_back({Address(32413,16), "007E9D10"});
  addr_pairs.push_back({Address(12,4), "00000C04"});

  for (auto p : addr_pairs) {
    Message msg;

    msg.set_address(p.a);

    BOOST_CHECK_EQUAL(p.a.node, msg.node());
    BOOST_CHECK_EQUAL(p.a.endpoint, msg.endpoint());
    BOOST_CHECK_EQUAL(p.hex, msg.address().to_string());
  }
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(create_message_payload)
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
  Socket::Ptr server = Socket::Server(context);
  Socket::Ptr client = Socket::Client(context);

  server->Attach(address.node);
  client->Attach(address.node);
  
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
  Socket::Ptr publisher = Socket::Publisher(context);
  Socket::Ptr subscriber = Socket::Subscriber(context);

  publisher->Attach(address.node);

  subscriber->Attach(address.node);
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
