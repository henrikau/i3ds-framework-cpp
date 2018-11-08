///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __MEASUREMENT_PROXY_HPP
#define __MEASUREMENT_PROXY_HPP

#include <zmq.hpp>
#include <thread>

namespace i3ds
{

class MeasurementProxy
{
public:

  static const int DEFAULT_SUBSCRIBE_PORT;
  static const int DEFAULT_PUBLISH_PORT;
  static const int DEFAULT_CONTROL_PORT;

  MeasurementProxy(int subscribe_port = DEFAULT_SUBSCRIBE_PORT,
                   int publish_port = DEFAULT_PUBLISH_PORT,
                   int control_port = DEFAULT_CONTROL_PORT);
  virtual ~MeasurementProxy();

  void Start();
  void Stop();

private:
  zmq::context_t context_;
  zmq::socket_t subscribe_socket_;
  zmq::socket_t publish_socket_;
  zmq::socket_t control_recv_socket_;
  zmq::socket_t control_send_socket_;

  std::thread worker_;

  void Run();
};

} // namespace i3ds

#endif
