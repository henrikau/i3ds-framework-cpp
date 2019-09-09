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
#include <vector>
#include <cstdlib>

#include <i3ds/communication.hpp>
#include <i3ds/exception.hpp>
#include <i3ds/address_server.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

void Append(Message& msg, std::string data)
{
  msg.append_payload((const byte*) data.c_str(), data.size());
}

void Append(Message& msg, const std::vector<std::string>& data)
{
  for (unsigned int i = 0; i < data.size(); i++)
    {
      Append(msg, data[i]);
    }
}

void Append(Message& msg, size_t size)
{
  byte* data = (byte*) malloc(size);

  for (unsigned int i = 0; i < size; i++)
    {
      data[i] = i % 0xFF;
    }

  msg.append_payload(data, size, false);
}

void Initialize(Message& msg, Address address)
{
  msg.set_address(address);
}

void check_message(const Message& msg, const Address& address, const std::string data)
{
  Address a = msg.address();

  BOOST_CHECK_EQUAL(a.node, address.node);
  BOOST_CHECK_EQUAL(a.endpoint, address.endpoint);
  BOOST_CHECK_EQUAL(msg.payloads(), 1);
  BOOST_CHECK_EQUAL(msg.size(0), data.size());
  BOOST_CHECK_EQUAL(memcmp(msg.data(0), data.c_str(), msg.size(0)), 0);
}

void check_message(const Message& msg, const Address& address, const std::vector<std::string>& data)
{
  Address a = msg.address();

  BOOST_CHECK_EQUAL(a.node, address.node);
  BOOST_CHECK_EQUAL(a.endpoint, address.endpoint);
  BOOST_CHECK_EQUAL(msg.payloads(), data.size());

  for (unsigned int i = 0; i < data.size(); i++)
    {
      std::cout << "Data " << std::string((const char*) msg.data(i), msg.size(i)) << std::endl;
      BOOST_CHECK_EQUAL(msg.size(i), data[i].size());
      BOOST_CHECK_EQUAL(memcmp(msg.data(i), data[i].c_str(), msg.size(i)), 0);
    }
}

////////////////////////////////////////////////////////////////////////////////

struct F
{
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

BOOST_AUTO_TEST_CASE(addr_server_url_test)
{
    // Test url from env var
    setenv("I3DS_ADDR_SRV_URL", "tcp://192.68.0.1:5000", 1);
    Context::Ptr c1 = Context::Create();
    BOOST_CHECK(c1->get_addr_srv_url() == "tcp://192.68.0.1:5000");

    // Test url from string
    Context::Ptr c2 = Context::Create("tcp://192.168.0.1:4000");
    BOOST_CHECK(c2->get_addr_srv_url() == "tcp://192.168.0.1:4000");

    // Test default url
    unsetenv("I3DS_ADDR_SRV_URL");
    Context::Ptr c3 = Context::Create();
    BOOST_CHECK(c3->get_addr_srv_url() == Context::DEFAULT_ADDR_SRV_URL);

}

////////////////////////////////////////////////////////////////////////////////

struct addr_pair
{
  Address a;
  std::string hex;
};

BOOST_AUTO_TEST_CASE(create_id_message_test)
{
  std::vector<addr_pair> addr_pairs;

  addr_pairs.push_back({Address(), "00000000"});
  addr_pairs.push_back({Address(0,0), "00000000"});
  addr_pairs.push_back({Address(32413,45),"007E9D2D"});
  addr_pairs.push_back({Address(16777215,255), "FFFFFFFF"});
  addr_pairs.push_back({Address(1,1), "00000101"});
  addr_pairs.push_back({Address(10485760,45), "A000002D"});
  addr_pairs.push_back({Address(32413,16), "007E9D10"});
  addr_pairs.push_back({Address(12,4), "00000C04"});

  for (auto p : addr_pairs)
    {
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

  Initialize(msg, address);
  Append(msg, data);

  BOOST_CHECK_EQUAL(msg.payloads(), 1);
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

  Initialize(req, address);
  Append(req, "Hello?");

  client->Send(req);
  server->Receive(copy);

  check_message(copy, address, "Hello?");

  Initialize(res, address);
  Append(res, "World!");

  server->Send(res);
  client->Receive(copy);

  check_message(copy, address, "World!");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(publish_subscribe_pattern)
{
  Socket::Ptr publisher = Socket::Publisher(context);
  Socket::Ptr subscriber = Socket::Subscriber(context);

  std::vector<std::string> text;

  text.push_back("Hello");
  text.push_back("brave");
  text.push_back("new");
  text.push_back("world!");

  publisher->Attach(address.node);

  subscriber->Attach(address.node);
  subscriber->Filter(address);

  // Needed to let filter take effect before sending.
  usleep(10000);

  Message msg, copy;

  Initialize(msg, address);
  Append(msg, text);

  publisher->Send(msg);
  subscriber->Receive(copy);

  check_message(copy, address, text);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(connect_to_address_from_config_file)
{
  AddressServer address_server("test_addresses.csv"); // Default port assumed by Context

  address_server.Start();

  int node = 323; // Known node_id from test-file

  Socket::Ptr publisher = Socket::Publisher(context);
  Socket::Ptr subscriber = Socket::Subscriber(context);
  Socket::Ptr server = Socket::Server(context);
  Socket::Ptr client = Socket::Client(context);

  publisher->Attach(node);
  subscriber->Attach(node);
  server->Attach(node);
  client->Attach(node);

  // Attempt to attach to node id, not found in file
  BOOST_CHECK_THROW(publisher->Attach(5432), i3ds::CommunicationError);

  address_server.Stop();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(nonexisiting_address_server)
{
  i3ds::Context::Ptr c = i3ds::Context::Create("tcp://nonexisting_hostname:123456");

  Socket::Ptr publisher = Socket::Publisher(c);

  BOOST_CHECK_THROW(publisher->Attach(323), i3ds::CommunicationError);

}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
