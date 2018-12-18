///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <i3ds/receiver.hpp>
#include <i3ds/exception.hpp>

i3ds::Receiver::Receiver(Context::Ptr context)
  : context_(context), timeout_ms_(100), running_(false)
{
}

i3ds::Receiver::~Receiver()
{
  Stop();
}

void
i3ds::Receiver::Start()
{
  if (running_)
    {
      throw CommunicationError("Receiver is already running");
    }

  running_ = true;
  worker_ = std::thread(&i3ds::Receiver::Run, this);
}

void
i3ds::Receiver::Stop()
{
  running_ = false;

  if (worker_.joinable())
    {
      worker_.join();
    }
}

void
i3ds::Receiver::Run()
{
  Message message;

  socket_ = Create();

  while (running_)
    {
      try
        {
          socket_->Receive(message, timeout_ms_);
          Handle(message, *socket_);
        }
      catch(Timeout& e)
        {
          // Carry on...
        }
      catch(CodecError& e)
        {
          std::cerr << "Codec error: " << e.what() << std::endl;
        }
      catch(CommunicationError& e)
        {
          std::cerr << "Communication error: " << e.what() << std::endl;
          running_ = false;
        }
      catch(std::exception& e)
        {
          std::cerr << "Exception: " << e.what() << std::endl;
          running_ = false;
        }
    }

  socket_.reset();
}
