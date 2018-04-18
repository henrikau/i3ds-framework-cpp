///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <zmq.hpp>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <thread>

namespace i3ds
{
    
class AddressServer
{
public:
    AddressServer(std::string port, std::string filename);
    virtual ~AddressServer();

    void Start();
    void Stop();
    void Run();

private:
    std::map<std::string, std::string> address_book_;
    zmq::context_t context_;
    zmq::socket_t socket_;

    volatile bool running_;
    std::thread worker_;

    void handle_query();
    void load_csv_file(std::string filename);
};

AddressServer::AddressServer(std::string port, std::string filename) : context_(1), socket_(context_, ZMQ_REP) 
{
    int timeout_ms = 100;
    socket_.bind("tcp://*:" + port);
    socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));
    load_csv_file(filename);
}

AddressServer::~AddressServer()
{
    Stop();
}

void
AddressServer::load_csv_file(std::string filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Address-file could not be opened");
    }
    std::map<std::string, std::string> result;
    std::string line, node, type, operation, address;

    while(std::getline(file, line))
    {
        std::stringstream line_stream(line);
        std::getline(line_stream, node, ',');
        std::getline(line_stream, type, ',');
        std::getline(line_stream, operation, ',');
        std::getline(line_stream, address);
        address_book_.insert(std::pair<std::string, std::string>(node+","+type, operation+","+address));
    }
    file.close();
}

void
AddressServer::handle_query()
{
    zmq::message_t request;

    if (socket_.recv(&request)>0)
    {
        std::string query = std::string(static_cast<char*>(request.data()), request.size());
        std::string result = address_book_[query];

        zmq::message_t reply(result.length());
        memcpy(reply.data(), result.c_str(), result.length());
        socket_.send(reply);
    }
}

void
AddressServer::Start()
{
    running_ = true;
    worker_ = std::thread(&i3ds::AddressServer::Run, this);
}

void
AddressServer::Stop()
{
    running_ = false;
    if(worker_.joinable())
    {
        worker_.join();
    }
}

void
AddressServer::Run()
{
    while (running_)
    {
        handle_query();
    }
}

} // namespace i3ds
