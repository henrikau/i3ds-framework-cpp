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
#include "message.hpp"
#include "zmq.hpp"
#include <cstdint>


i3ds::ZmqSensorServer::ZmqSensorServer(i3ds::Sensor* sensor, std::string address)
{
    sensor_ = sensor;
    address_ = address;
}

i3ds::ZmqSensorServer::~ZmqSensorServer()
{
}

void i3ds::ZmqSensorServer::run()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    try {
        socket.bind(address_);
    } catch (const zmq::error_t e) {
        // TODO: handle properly
    }

    while(true) {
        zmq::message_t id_msg, data_msg;
        socket.recv(&id_msg);
        i3ds::Message request = create_i3ds_message(id_msg);
        if(!id_msg.more())
        {
            socket.recv(&data_msg);
            add_message_payload(&request, data_msg);
        }

        i3ds::Message response = sensor_->handle_request(request);

        zmq::message_t reply_id = create_id_message(response);
        if(response.size == 0) {
            socket.send(reply_id);
        } else {
            zmq::message_t reply_data = create_payload_message(response);
            socket.send(reply_id, ZMQ_SNDMORE);
            socket.send(reply_data);
        }
    }
}

zmq::message_t i3ds::create_payload_message(i3ds::Message i3ds_msg)
{
    zmq::message_t payload_msg((void*)i3ds_msg.data, i3ds_msg.size);
    return payload_msg;
}

zmq::message_t i3ds::create_id_message(i3ds::Message i3ds_msg)
{
    uint32_t combined_id = (i3ds_msg.sensor_id << 8) + i3ds_msg.endpoint_id;
    char id_buffer[9]; // Size is 9 to accomodate terminating \0
    snprintf(id_buffer, 9, "%08x", combined_id);
    zmq::message_t id_msg((void*)id_buffer,8);
    return id_msg;
}

i3ds::Message i3ds::create_i3ds_message(const zmq::message_t &id_msg)
{
    char id_buffer[9];
    snprintf(id_buffer, 9, "%s", (char*)id_msg.data());
    uint32_t combined_id = strtoul(id_buffer, NULL, 16);

    i3ds::Message i3ds_msg;
    i3ds_msg.sensor_id = (combined_id >> 8);
    i3ds_msg.endpoint_id = combined_id & 255;

    return i3ds_msg;
}

void i3ds::add_message_payload(i3ds::Message* i3ds_msg, const zmq::message_t& payload_msg)
{
    i3ds_msg->data = (byte*)payload_msg.data();
    i3ds_msg->size = payload_msg.size();
}
