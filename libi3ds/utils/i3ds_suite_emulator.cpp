///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include <csignal>
#include <iostream>
#include <unistd.h>

#include "i3ds/core/communication.hpp"
#include "i3ds/emulators/emulated_camera.hpp"

volatile bool running;

void signal_handler(int signum)
{
  running = false;
}

int main(int argc, char** argv)
{
  i3ds::Context::Ptr context = i3ds::Context::Create();;

  i3ds::Server server(context);
  i3ds::EmulatedCamera camera(context, 10, 800, 600);

  camera.Attach(server);

  running = true;
  signal(SIGINT, signal_handler);

  server.Start();

  while(running)
    {
      sleep(1);
    }

  server.Stop();

  return 0;
}
