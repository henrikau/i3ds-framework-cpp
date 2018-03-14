///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __ZMQ_SENSOR_CLIENT_HPP
#define __ZMQ_SENSOR_CLIENT_HPP

#include "message.hpp"
#include <string>
#include <zmq.hpp>

namespace i3ds
{

class ZmqSensorClient
{
private:
    std::string address_;
    zmq::context_t context_;
    zmq::socket_t socket_;

public:
    ZmqSensorClient(std::string address);
    
    virtual ~ZmqSensorClient();
    Message sendRequest(Message message);
};

} // namespace i3ds

#endif
