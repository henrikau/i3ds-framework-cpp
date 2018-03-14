///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __ZMQ_MSG_CODEC_HPP
#define __ZMQ_MSG_CODEC_HPP

#include "message.hpp"
#include <zmq.hpp>

namespace i3ds
{

zmq::message_t create_payload_message(Message i3ds_msg);
zmq::message_t create_id_message(Message i3ds_msg);

Message create_i3ds_message(const zmq::message_t &id_msg);
void add_message_payload(Message* i3ds_msg, const zmq::message_t& payload_msg);

void sendI3dsMsgOverZmq(Message i3ds_msg, zmq::socket_t &socket);
Message receiveI3dsMsgOverZmq(zmq::socket_t &socket);

} // namespace i3ds

#endif
