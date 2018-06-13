///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <zmq.hpp>
#include <thread>

#include <i3ds/measurement_proxy.hpp>

const int i3ds::MeasurementProxy::DEFAULT_SUBSCRIBE_PORT = 9001;
const int i3ds::MeasurementProxy::DEFAULT_PUBLISH_PORT = 9002;
const int i3ds::MeasurementProxy::DEFAULT_CONTROL_PORT = 9003;

i3ds::MeasurementProxy::MeasurementProxy(int subscribe_port, int publish_port, int control_port) :
  context_(1),
  subscribe_socket_(context_, ZMQ_XSUB),
  publish_socket_(context_, ZMQ_XPUB),
  control_recv_socket_(context_, ZMQ_SUB),
  control_send_socket_(context_, ZMQ_PUB)
{
  subscribe_socket_.bind("tcp://*:" + std::to_string(subscribe_port));
  publish_socket_.bind("tcp://*:" + std::to_string(publish_port));
  control_send_socket_.bind("tcp://127.0.0.1:" + std::to_string(control_port));
  control_recv_socket_.connect("tcp://127.0.0.1:" + std::to_string(control_port));
  control_recv_socket_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

i3ds::MeasurementProxy::~MeasurementProxy()
{
  Stop();
}

void
i3ds::MeasurementProxy::Start()
{
  worker_ = std::thread(&i3ds::MeasurementProxy::Run, this);
}

void
i3ds::MeasurementProxy::Stop()
{
  control_send_socket_.send("TERMINATE", 9, 0);
  if(worker_.joinable())
    {
      worker_.join();
    }
}

void
i3ds::MeasurementProxy::Run()
{
  zmq::proxy_steerable(static_cast<void *>(subscribe_socket_),
                       static_cast<void *>(publish_socket_),
                       nullptr,
                       static_cast<void *>(control_recv_socket_));
}
