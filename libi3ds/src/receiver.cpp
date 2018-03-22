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

i3ds::Receiver::Receiver(Context& context)
  : context_(context), running_(false)
{
}

i3ds::Receiver::~Receiver()
{
}

void
i3ds::Receiver::Stop()
{
  running_ = false;
}
  
void
i3ds::Receiver::Run()
{
  running_ = true;

  try
    {
      while (running_)
	{
	  ReceiveOne(10);
	}
    }
  catch(std::exception e)
    {
      std::cerr << "Receiver got error: " << e.what() << std::endl;
      socket_.reset();
    }
}
