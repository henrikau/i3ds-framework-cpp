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
#include <cstdlib>

#include "i3ds/sensors/camera_client.hpp"

void
print_usage_and_exit(char *command, int retval)
{
    std::cout << "Usage: " << command << " <node_id> <command> <args...>" << std::endl;
    std::cout << "  Available commands:" << std::endl;
    std::cout << "    get                                                          - Print current settings" << std::endl;
    std::cout << "    sta <activate|start|stop|deactivate>                         - Send state command" << std::endl;
    std::cout << "    exp <exposure> <gain>                                        - Set exposure" << std::endl;
    std::cout << "    aex <auto-exposure enabled> <max_exposure> <max_gain>        - Set auto exposure" << std::endl;
    std::cout << "    reg <region enabled> <x-offset> <y-offset> <x-size> <y-size> - Set region" << std::endl;
    std::cout << "    fla <flash enabled> <flash strength>                         - Set flash" << std::endl;
    std::cout << "    pat <pattern enabled> <pattern sequence>                     - Set pattern" << std::endl;
    std::cout << "    rat <rate>                                                   - Set rate" << std::endl;
    exit(retval);
}

void
print_camera_settings(i3ds::CameraClient *camera)
{
    camera->load_configuration();
    camera->load_status();

    int state = camera->state();
    std::string state_string;
    switch (state)
    {
        case inactive:
            state_string = "inactive";
            break;
        case standby:
            state_string = "standby";
            break;
        case operational:
            state_string = "operational";
            break;
        case failure:
            state_string = "failure";
            break;
        default:
            state_string = "UNKNOWN STATE";
    }
  
    std::cout << "State: " << state_string << std::endl;
    std::cout << "Temperature [deg K]: " << camera->temperature().kelvin << std::endl;

    std::cout << "Exposure: " << camera->exposure() << std::endl;
    std::cout << "Gain: " << camera->gain() << std::endl;

    std::cout << "Auto-exposure enabled: " << camera->auto_exposure_enabled() << std::endl;
    std::cout << "Max exposure: " << camera->max_exposure() << std::endl;
    std::cout << "Max gain: " << camera->max_gain() << std::endl;

    std::cout << "Region enabled: " << camera->region_enabled() << std::endl;
    std::cout << "Region: (" << camera->region().size_x;
    std::cout << ", " << camera->region().size_y;
    std::cout << ", " << camera->region().offset_x;
    std::cout << ", " << camera->region().offset_y << ")" << std::endl;

    std::cout << "Flash enabled: " << camera->flash_enabled() << std::endl;
    std::cout << "Flash strength: " << camera->flash_strength() << std::endl;

    std::cout << "Pattern enabled: " << camera->pattern_enabled() << std::endl;
    std::cout << "Pattern sequence: " << camera->pattern_sequence() << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_usage_and_exit(argv[0],-1);
    }

    int node_id = atoi(argv[1]);
    i3ds::Context::Ptr context(i3ds::Context::Create());
    i3ds::CameraClient camera(context, node_id);

    std::string command = argv[2];

    if (command == "get")
    {
        print_camera_settings(&camera);
    }
    else if (command == "sta")
    {
        if (argc < 4) {print_usage_and_exit(argv[0],-1);}
        std::string state = argv[3];
        if (state == "activate") {camera.set_state(activate);}
        else if (state == "start") {camera.set_state(start);}
        else if (state == "stop") {camera.set_state(stop);}
    }
    else if (command == "ex")
    {
        if (argc < 5) {print_usage_and_exit(argv[0],-1);}
        ExposureTime exposure = atof(argv[3]);
        SensorGain gain = atof(argv[4]);
        camera.set_exposure(exposure, gain);
    }
    else if (command == "aex")
    {
        if (argc < 6) {print_usage_and_exit(argv[0],-1);}
        bool auto_exposure_enabled = atoi(argv[3]);
        ExposureTime max_exposure = atof(argv[4]);
        SensorGain max_gain = atof(argv[5]);
        camera.set_auto_exposure(auto_exposure_enabled, max_exposure, max_gain);
    }
    else if (command == "req")
    {
        if (argc < 8) {print_usage_and_exit(argv[0],-1);}
        bool region_enabled = atoi(argv[3]);
        PlanarRegion region = {static_cast<uint16_t>(atoi(argv[4])),
                               static_cast<uint16_t>(atoi(argv[5])),
                               static_cast<uint16_t>(atoi(argv[6])),
                               static_cast<uint16_t>(atoi(argv[7]))};
        camera.set_region(region_enabled, region);
    }
    else if (command == "fla")
    {
        if (argc < 5) {print_usage_and_exit(argv[0],-1);}
        bool flash_enabled = atoi(argv[3]);
        FlashStrength flash_strength = atof(argv[4]);
        camera.set_flash(flash_enabled, flash_strength);
    }
    else if (command == "pat")
    {
        if (argc < 5) {print_usage_and_exit(argv[0],-1);}
        bool pattern_enabled = atoi(argv[3]);
        PatternSequence pattern_sequence = atof(argv[4]);
        camera.set_pattern(pattern_enabled, pattern_sequence);
    }
    else if (command == "rat")
    {
        if (argc < 4) {print_usage_and_exit(argv[0],-1);}
        SampleRate rate = atoi(argv[3]);
        camera.set_rate(rate);
    }
    else 
    {
        print_usage_and_exit(argv[0],0);
    }

    return 0;
}
