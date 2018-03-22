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

#include "receiver.hpp"

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

  socket_ = Create(*context_);

  try
    {
      while (running_)
	{
	  if (socket_->Receive(message, timeout_ms_))
	    {
	      Handle(message, *socket_);
	    }
        }
    }
  catch(std::exception e)
    {
      std::cerr << "Receiver got error: " << e.what() << std::endl;
    }

  socket_.reset();
}
