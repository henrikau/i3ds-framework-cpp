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

namespace i3ds
{

////////////////////////////////////////////////////////////////////////////////
/// Worker class for receiving messages.
////////////////////////////////////////////////////////////////////////////////

class Receiver
{
public:

  Receiver(Context& context);
  virtual ~Receiver();

  // Stop the receiver if running.
  void Stop();
  
  // Run the receiver until stop is called.
  void Run();

protected:

  // Receive one message, return false on timeout.
  virtual bool ReceiveOne(int timeout_ms = -1) = 0;

  // Initialize the socket.
  virtual void Reset() = 0;

  // Context reference.
  Context& context_;

  // Receiver socket.
  Socket::Ptr socket_;

private:

  volatile bool running_;
};

} // namespace i3ds

#endif
