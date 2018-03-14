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

i3ds::ZmqSensorServer::ZmqSensorServer(Handler* handler, std::string address)
try : context_(1), socket_(context_, ZMQ_REP)
{
    handler_ = handler;
    address_ = address;
    socket_.bind(address_);
} catch (const zmq::error_t e) {
    throw std::runtime_error(e.what());
}

void i3ds::ZmqSensorServer::run()
{
    while(true) {
        try {
            Message request = receiveI3dsMsgOverZmq(socket_);
            Message response;

	    handler_->handle(request, response);

            sendI3dsMsgOverZmq(response, socket_);
        } catch (const zmq::error_t e) {
            throw std::runtime_error(e.what());
        }
    }
}
