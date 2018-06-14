///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_COMMON_TESTS_HPP
#define __I3DS_COMMON_TESTS_HPP

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/sensor.hpp>
#include <i3ds/sensor_client.hpp>

namespace i3ds
{

void 
test_sensor_creation(Sensor::Ptr sensor, NodeID node)
{
  BOOST_CHECK_EQUAL(sensor->node(), node);
  BOOST_CHECK_EQUAL(sensor->state(), inactive);
  BOOST_CHECK_EQUAL(sensor->period(), 0);
}

void
test_sample_settings(SensorClient::Ptr client)
{
  SamplePeriod period = 100000;
  BatchSize batch_size = 1;
  BatchCount batch_count = 1;

  client->set_state(activate);

  client->set_sampling(period, batch_size, batch_count);
  client->load_config();

  BOOST_CHECK_EQUAL(period, client->period());
  BOOST_CHECK_EQUAL(batch_size, client->batch_size());
  BOOST_CHECK_EQUAL(batch_count, client->batch_count());
}

template<typename RegionType, typename SensorType, typename ClientType>
void
test_region(typename SensorType::Ptr sensor, typename ClientType::Ptr client)
{
  RegionType r1 = {300, 200, 150, 100};
  bool region_enabled = true;

  client->set_state(activate);
  client->set_region(region_enabled, r1);

  BOOST_CHECK_EQUAL(sensor->region_enabled(), region_enabled);

  RegionType r2 = sensor->region();

  BOOST_CHECK_EQUAL(r1.size_x, r2.size_x);
  BOOST_CHECK_EQUAL(r1.size_y, r2.size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, r2.offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, r2.offset_y);

  client->load_config();

  BOOST_CHECK_EQUAL(region_enabled, client->region_enabled());
  BOOST_CHECK_EQUAL(r1.size_x, client->region().size_x);
  BOOST_CHECK_EQUAL(r1.size_y, client->region().size_y);
  BOOST_CHECK_EQUAL(r1.offset_x, client->region().offset_x);
  BOOST_CHECK_EQUAL(r1.offset_y, client->region().offset_y);
}

} // namespace i3ds

#endif
