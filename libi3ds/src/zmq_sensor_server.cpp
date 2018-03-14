/////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "zmq_sensor_server.hpp"
#include "zmq_msg_codec.hpp"
#include "message.hpp"
#include "zmq.hpp"
#include <stdexcept>


i3ds::ZmqSensorServer::ZmqSensorServer(i3ds::Sensor* sensor, std::string address)
try : context_(1), socket_(context_, ZMQ_REP)
{
    sensor_ = sensor;
    address_ = address;
    socket_.bind(address_);
} catch (const zmq::error_t e) {
    throw std::runtime_error(e.what());
}

i3ds::ZmqSensorServer::~ZmqSensorServer()
{
}

void i3ds::ZmqSensorServer::run()
{
    while(true) {
        try {
            zmq::message_t id_msg, data_msg;
            socket_.recv(&id_msg);
            i3ds::Message request = create_i3ds_message(id_msg);
            if(id_msg.more())
            {
                socket_.recv(&data_msg);
                add_message_payload(&request, data_msg);
            }

            i3ds::Message response = sensor_->handle_request(request);

            zmq::message_t reply_id = create_id_message(response);
            if(response.size == 0) {
                socket_.send(reply_id);
            } else {
                zmq::message_t reply_data = create_payload_message(response);
                socket_.send(reply_id, ZMQ_SNDMORE);
                socket_.send(reply_data);
            }
        } catch (const zmq::error_t e) {
            throw std::runtime_error(e.what());
        }
    }
}

