///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE pose_estimator_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <chrono>

#include <i3ds/subscriber.hpp>
#include <i3ds/emulated_pose_estimator.hpp>
#include <i3ds/pose_estimator_client.hpp>
#include <i3ds/common_tests.hpp>

using namespace i3ds;

////////////////////////////////////////////////////////////////////////////////

struct F
{
  F()
    : node(1),
      context(Context::Create()),
      server(context),
      pe(EmulatedPoseEstimator::Create(context, node)),
      client(PoseEstimatorClient::Create(context, node))
  {
    BOOST_TEST_MESSAGE("setup fixture");
    pe->Attach(server);
    server.Start();
    client->set_timeout(1000);
  }

  ~F()
  {
    BOOST_TEST_MESSAGE( "teardown fixture" );
    server.Stop();
  }

  const NodeID node;

  Context::Ptr context;
  Server server;
  EmulatedPoseEstimator::Ptr pe;
  PoseEstimatorClient::Ptr client;
};

BOOST_FIXTURE_TEST_SUITE(s, F)

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(pose_estimator_creation)
{
  test_sensor_creation(pe, node);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(pose_estimator_sample_settings)
{
  test_sample_settings(client);
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(pose_estimator_imaging_mode)
{
  bool imaging_mode = true;

  client->set_state(activate);
  client->set_imaging_mode(imaging_mode);

  BOOST_CHECK_EQUAL(pe->imaging_mode(), imaging_mode);


  client->load_config();

  BOOST_CHECK_EQUAL(imaging_mode, client->imaging_mode());
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(pose_estimator_camera_select)
{
  uint8_t camera = 4;

  client->set_state(activate);
  client->set_selected_camera(camera);

  BOOST_CHECK_EQUAL(pe->selected_camera(), camera);


  client->load_config();

  BOOST_CHECK_EQUAL(camera, client->selected_camera());
}

////////////////////////////////////////////////////////////////////////////////

int poses_received;
int frames_received;

void
handle_pose_measurement(PoseEstimator::MeasurementTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Received pose at: " << data.attributes.timestamp);

  BOOST_CHECK_EQUAL(data.estimated_pose.position.data.nCount, 3);
  BOOST_CHECK_EQUAL(data.estimated_pose.position.data.arr[0], 1.0);
  BOOST_CHECK_EQUAL(data.estimated_pose.position.data.arr[1], 2.0);
  BOOST_CHECK_EQUAL(data.estimated_pose.position.data.arr[2], 3.0);

  BOOST_CHECK_EQUAL(data.estimated_pose.orientation.im.nCount,  3);
  BOOST_CHECK_EQUAL(data.estimated_pose.orientation.im.arr[0], 1.0);
  BOOST_CHECK_EQUAL(data.estimated_pose.orientation.im.arr[1], 2.0);
  BOOST_CHECK_EQUAL(data.estimated_pose.orientation.im.arr[2], 3.0);
  BOOST_CHECK_EQUAL(data.estimated_pose.orientation.re, 4.0);

  poses_received++;
}

void
handle_frame_measurement(PoseEstimator::FrameTopic::Data& data)
{
  BOOST_TEST_MESSAGE("Received image at: " << data.descriptor.attributes.timestamp);

  BOOST_CHECK_EQUAL(data.descriptor.region.size_x, 640);
  BOOST_CHECK_EQUAL(data.descriptor.region.size_y, 480);
  BOOST_CHECK_EQUAL(data.descriptor.region.offset_x, 0);
  BOOST_CHECK_EQUAL(data.descriptor.region.offset_y, 0);

  BOOST_CHECK_EQUAL(data.descriptor.frame_mode, mode_mono);
  BOOST_CHECK_EQUAL(data.descriptor.data_depth, 8);
  BOOST_CHECK_EQUAL(data.descriptor.pixel_size, 1);
  frames_received++;
}

BOOST_AUTO_TEST_CASE(pose_estimator_sampling)
{
  poses_received = 0;
  frames_received = 0;
  Subscriber subscriber(context);

  subscriber.Attach<PoseEstimator::MeasurementTopic>(client->node(), &handle_pose_measurement);
  subscriber.Attach<PoseEstimator::FrameTopic>(client->node(), &handle_frame_measurement);


  SamplePeriod period = 100000;

  client->set_state(activate);
  client->set_sampling(period);
  client->set_state(start);

  subscriber.Start();
  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));
  client->set_state(stop);
  BOOST_CHECK_GT(poses_received, 0);

  client->set_imaging_mode(true);
  client->set_state(start);
  std::this_thread::sleep_for(std::chrono::microseconds(period * 5));
  client->set_state(stop);
  BOOST_CHECK_GT(frames_received, 0);

  subscriber.Stop();
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE_END()
