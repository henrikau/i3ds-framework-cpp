///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __ADDRESS_SERVER_HPP
#define __ADDRESS_SERVER_HPP

#include <zmq.hpp>
#include <string>
#include <map>
#include <thread>
#include <atomic>

namespace i3ds
{

class AddressServer
{
public:

  static const int DEFAULT_PORT;

  AddressServer(std::string filename, int port = DEFAULT_PORT);
  virtual ~AddressServer();

  void Start();
  void Stop();
  void Run();

private:
  std::map<std::string, std::string> address_book_;
  zmq::context_t context_;
  zmq::socket_t socket_;

  std::atomic<bool> running_;
  std::thread worker_;

  void handle_query();
  void load_csv_file(std::string filename);
};

} // namespace i3ds

#endif
