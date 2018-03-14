/////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "zmq_sensor_client.hpp"
#include "zmq_msg_codec.hpp"
#include "message.hpp"
#include "zmq.hpp"
#include <stdexcept>


i3ds::ZmqSensorClient::ZmqSensorClient(std::string address)
try : context_(1), socket_(context_, ZMQ_REQ)
{
    address_ = address;
    socket_.bind(address_);
} catch (const zmq::error_t e) {
    throw std::runtime_error(e.what());
}

i3ds::ZmqSensorClient::~ZmqSensorClient()
{
}

i3ds::Message i3ds::ZmqSensorClient::sendRequest(i3ds::Message i3ds_msg)
{
    try {
        sendI3dsMsgOverZmq(i3ds_msg, socket_);
        return receiveI3dsMsgOverZmq(socket_);
    } catch (const zmq::error_t e) {
        throw std::runtime_error(e.what());
    }
}

