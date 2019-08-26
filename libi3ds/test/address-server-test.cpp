///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE address_server_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <i3ds/address_server.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////


std::string get_address(zmq::socket_t *socket, std::string input)
{
  zmq::message_t request (input.length());
  memcpy (request.data (), input.c_str(), input.length());
  socket->send (request);

  zmq::message_t reply;
  socket->recv (&reply);
  std::string reply_string = std::string(static_cast<char*>(reply.data()), reply.size());
  return reply_string;

}
////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(setup_address_server_nok)
{
  BOOST_REQUIRE_THROW(AddressServer server("test_addresses2.csv"),
		      std::runtime_error);
}

struct F
{
  F()
    : port(i3ds::AddressServer::DEFAULT_PORT),
      context(1),
      socket(context, ZMQ_REQ),
      srv("test_addresses.csv", port)
  {
    socket.connect ("tcp://localhost:" + std::to_string(port));
    srv.Start();
  }

  ~F()
  {
    srv.Stop();
  }

  const int port;
  zmq::context_t context;
  zmq::socket_t socket;
  AddressServer srv;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(check_addresses)
{
  BOOST_CHECK_EQUAL(get_address(&socket, "323,pub"), "b,tcp://127.0.0.1:7000");
  BOOST_CHECK_EQUAL(get_address(&socket, "323,sub"), "c,tcp://127.0.0.1:7000");
  BOOST_CHECK_EQUAL(get_address(&socket, "323,req"), "c,tcp://127.0.0.1:7001");
  BOOST_CHECK_EQUAL(get_address(&socket, "323,rep"), "b,tcp://127.0.0.1:7001");
  BOOST_CHECK_EQUAL(get_address(&socket, "324,pub"), "b,tcp://192.168.0.2:7000");
  BOOST_CHECK_EQUAL(get_address(&socket, "324,sub"), "c,tcp://192.168.0.2:7000");
  BOOST_CHECK_EQUAL(get_address(&socket, "324,req"), "c,tcp://192.168.0.2:7001");
  BOOST_CHECK_EQUAL(get_address(&socket, "324,rep"), "b,tcp://192.168.0.2:7001");
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
