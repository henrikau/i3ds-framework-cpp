///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __ZMQ_SENSOR_SERVER_HPP
#define __ZMQ_SENSOR_SERVER_HPP

#include "message.hpp"
#include "sensor.hpp"
#include <zmq.hpp>
#include <string>

namespace i3ds
{

class ZmqSensorServer
{
private:
    Sensor* sensor_;
    std::string address_;
    zmq::context_t context_;
    zmq::socket_t socket_;

public:
    ZmqSensorServer(i3ds::Sensor* sensor, std::string address);
    
    virtual ~ZmqSensorServer();
    void run();
};

zmq::message_t create_payload_message(Message i3ds_msg);
zmq::message_t create_id_message(Message i3ds_msg);

Message create_i3ds_message(const zmq::message_t &id_msg);
void add_message_payload(Message* i3ds_msg, const zmq::message_t& payload_msg);

} // namespace i3ds

#endif
