/////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "zmq_msg_codec.hpp"
#include "message.hpp"
#include "zmq.hpp"
#include <cstdint>

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
    i3ds_msg.size = 0;

    return i3ds_msg;
}

void i3ds::add_message_payload(i3ds::Message* i3ds_msg, const zmq::message_t& payload_msg)
{
    //i3ds_msg->data = (byte*)payload_msg.data();
    i3ds_msg->data = (byte*)malloc(payload_msg.size());
    memcpy(i3ds_msg->data, payload_msg.data(), payload_msg.size());
    i3ds_msg->size = payload_msg.size();
}


void i3ds::sendI3dsMsgOverZmq(i3ds::Message i3ds_msg, zmq::socket_t &socket)
{
    zmq::message_t id_msg = create_id_message(i3ds_msg);
    if(i3ds_msg.size == 0) {
        socket.send(id_msg);
    } else {
        zmq::message_t data_msg = create_payload_message(i3ds_msg);
        socket.send(id_msg, ZMQ_SNDMORE);
        socket.send(data_msg);
    }
}

i3ds::Message i3ds::receiveI3dsMsgOverZmq(zmq::socket_t &socket)
{
    zmq::message_t id_msg, data_msg;
    socket.recv(&id_msg);
    i3ds::Message i3ds_msg = create_i3ds_message(id_msg);
    if(id_msg.more())
    {
        socket.recv(&data_msg);
        add_message_payload(&i3ds_msg, data_msg);
    }
    return i3ds_msg;
}

