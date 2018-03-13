///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE zmq_sensor_server_test
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "zmq_sensor_server.hpp"
#include "message.hpp"
#include <zmq.hpp>

using namespace i3ds;

struct id_pair
{
    int sensor_id;
    int endpoint_id;
    const char* correct_hex;
};

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(create_id_message_test)
{
    std::vector<id_pair> id_pairs;
    id_pairs.push_back({0,0,"00000000"});
    id_pairs.push_back({32413,45,"007e9d2d"});
    id_pairs.push_back({16777215,255,"ffffffff"});
    id_pairs.push_back({1,1,"00000101"});
    id_pairs.push_back({10485760,45,"a000002d"});
    id_pairs.push_back({32413,16,"007e9d10"});
    id_pairs.push_back({12,4,"00000c04"});
    for(std::vector<id_pair>::iterator iter = id_pairs.begin(); iter != id_pairs.end(); ++iter) {
        Message i3ds_msg;
        i3ds_msg.sensor_id = iter->sensor_id;
        i3ds_msg.endpoint_id = iter->endpoint_id;
        zmq::message_t zmq_msg = create_id_message(i3ds_msg);
        byte* buffer = (byte*) zmq_msg.data();
        for (unsigned int i = 0; i < 8; ++i) {
            BOOST_CHECK_EQUAL(buffer[i], iter->correct_hex[i]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(create_payload_message_test)
{
    Message i3ds_msg;

    char data[32] = "Hello world123";
    i3ds_msg.data = (byte*)data;
    i3ds_msg.size = strlen(data)+1;
    zmq::message_t zmq_msg = create_payload_message(i3ds_msg);
    BOOST_CHECK_EQUAL(strcmp(data, (char*)zmq_msg.data()), 0);
}

////////////////////////////////////////////////////////////////////////////////

Message create_string_msg(int sensor_id, int endpoint_id, const char* data)
{
    Message i3ds_msg;
    i3ds_msg.data = (byte*)data;
    i3ds_msg.size = strlen(data)+1;
    i3ds_msg.sensor_id = sensor_id;
    i3ds_msg.endpoint_id = endpoint_id;
    return i3ds_msg;
}

Message create_random_data_msg(int sensor_id, int endpoint_id, int size)
{
    Message i3ds_msg;
    i3ds_msg.data = (byte*)malloc(size);
    i3ds_msg.size = size;
    i3ds_msg.sensor_id = sensor_id;
    i3ds_msg.endpoint_id = endpoint_id;
    return i3ds_msg;
}

void check_zmq_msg_decoding(Message original_msg)
{
    zmq::message_t id_msg = create_id_message(original_msg);
    zmq::message_t data_msg = create_payload_message(original_msg);
    Message decoded_msg = create_i3ds_message(id_msg);
    add_message_payload(&decoded_msg, data_msg);

    BOOST_CHECK_EQUAL(original_msg.size, decoded_msg.size);
    BOOST_CHECK_EQUAL(original_msg.sensor_id, decoded_msg.sensor_id);
    BOOST_CHECK_EQUAL(original_msg.endpoint_id, decoded_msg.endpoint_id);
    for (unsigned int i = 0; i < original_msg.size; i++) {
        BOOST_CHECK_EQUAL(original_msg.data[i], decoded_msg.data[i]);
    }
}

BOOST_AUTO_TEST_CASE(create_i3ds_msg_test)
{
    check_zmq_msg_decoding(create_string_msg(123,21,"Hello World123"));
    check_zmq_msg_decoding(create_string_msg(0,0,""));
    check_zmq_msg_decoding(create_string_msg(16777215,255,"adsklfjsdhalkgjfhawpeoiuhf qd"));
    check_zmq_msg_decoding(create_string_msg(1,1,"<some_tag>some content</some_tag>"));

    check_zmq_msg_decoding(create_random_data_msg(123, 21, 24));
    check_zmq_msg_decoding(create_random_data_msg(0, 0, 0));
    check_zmq_msg_decoding(create_random_data_msg(16777215, 255, 1000));
    check_zmq_msg_decoding(create_random_data_msg(1, 1, 1));

}
