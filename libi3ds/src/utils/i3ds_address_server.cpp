///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#include "i3ds/utils/address_server.hpp"

#include <csignal>

volatile bool running;

void signal_handler(int signum)
{
    running = false;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <port> <configfile>" << std::endl;
        return -1;
    }
    running = true;
    signal(SIGINT, signal_handler);

    i3ds::AddressServer srv(argv[1], argv[2]);

    srv.Start();
    while(running)
    {
        sleep(1);
    }
    srv.Stop();
    
    return 0;
}
