///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_RECEIVER_HPP
#define __I3DS_RECEIVER_HPP

#include "communication.hpp"
#include <thread>

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Worker class for receiving messages.
////////////////////////////////////////////////////////////////////////////////

class Receiver
{
public:

  Receiver(Context::Ptr context);
  virtual ~Receiver();

  // Stop the receiver if running.
  void Stop();

  // Run the receiver until stop is called.
  void Run();

  // Starts the Run-function in a separate thread.
  void Start();

protected:

  // Initialize the socket.
  virtual Socket::Ptr Create() = 0;

  // Handle message, may send using socket.
  virtual void Handle(Message& message, Socket& socket) = 0;
  
  // Context reference.
  Context::Ptr context_;

private:

  // Receiver socket.
  Socket::Ptr socket_;

  // Timeout for receive polling.
  const int timeout_ms_;

  // Running flag.
  volatile bool running_;

  // Worker thread.
  std::thread worker_;
};

} // namespace i3ds

#endif
