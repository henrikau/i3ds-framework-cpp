///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE camera_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <thread>
#include <string>
#include <vector>
#include <iostream>

#include "emulated_camera.hpp"

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F {
  F()
    : id(1),
      context(Context::Create()),
      camera(context, id)
  {
    BOOST_TEST_MESSAGE("setup fixture");
    camera.Start();
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    camera.Stop();
  }

  const NodeID id;
  Context::Ptr context;
  EmulatedCamera camera;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(camera_creation)
{
  BOOST_CHECK_EQUAL(camera.node(), id);
  BOOST_CHECK_EQUAL(camera.state(), inactive);
  BOOST_CHECK_EQUAL(camera.rate(), 0.0);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
