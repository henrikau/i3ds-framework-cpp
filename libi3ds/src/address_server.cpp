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

#include <i3ds/address_server.hpp>

const int i3ds::AddressServer::DEFAULT_PORT = 6000;

i3ds::AddressServer::AddressServer(std::string filename, int port) : context_(1), socket_(context_, ZMQ_REP)
{
  int timeout_ms = 100;
  socket_.bind("tcp://*:" + std::to_string(port));
  socket_.setsockopt(ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));
  load_csv_file(filename);
}

i3ds::AddressServer::~AddressServer()
{
  Stop();
}

void
i3ds::AddressServer::load_csv_file(std::string filename)
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
i3ds::AddressServer::handle_query()
{
  zmq::message_t request;

  if (socket_.recv(&request) > 0)
    {
      std::string query = std::string(static_cast<char*>(request.data()), request.size());
      std::string result;
      if (address_book_.find(query) == address_book_.end())
        {
          result = "ADDRESS_NOT_FOUND";
        }
      else
        {
          result = address_book_[query];
        }

      zmq::message_t reply(result.length());
      memcpy(reply.data(), result.c_str(), result.length());
      socket_.send(reply);
    }
}

void
i3ds::AddressServer::Start()
{
  running_ = true;
  worker_ = std::thread(&i3ds::AddressServer::Run, this);
}

void
i3ds::AddressServer::Stop()
{
  running_ = false;
  if(worker_.joinable())
    {
      worker_.join();
    }
}

void
i3ds::AddressServer::Run()
{
  while (running_)
    {
      handle_query();
    }
}
