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
#include <set>
#include <vector>
#include <cstdlib>

#include <i3ds/power_client.hpp>

#include <boost/program_options.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace po = boost::program_options;
namespace logging = boost::log;

int main(int argc, char *argv[])
{
  //i3ds::SensorConfigurator configurator;
  NodeID node_id = 0;
  std::vector<PowerOutput> enable_ports;
  std::vector<PowerOutput> disable_ports;
  bool set_ports = false;
  std::string addr_server;

  po::options_description desc("Allowed trigger control options");

  desc.add_options()
  ("help", "Show help")
  ("node,n", po::value(&node_id)->default_value(299), "NodeID")
  ("enable,e", po::value(&enable_ports), "Enable power ports")
  ("disable,d", po::value(&disable_ports), "Disable power ports")
  ("set,s", po::bool_switch(&set_ports), "Turn on the enabled ports, turn off all others")
  ("addr-server,a", po::value(&addr_server)->default_value(""), "Address to the address-server")
  ("verbose,v", "Print verbose output") ("quite,q", "Quiet output");
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    BOOST_LOG_TRIVIAL(info) << desc;
    return -1;
  }
  if (vm.count ("quiet"))
    {
      logging::core::get ()->set_filter (
	  logging::trivial::severity >= logging::trivial::warning);
    }
  else if (!vm.count ("verbose"))
    {
      logging::core::get ()->set_filter (
	  logging::trivial::severity >= logging::trivial::info);
    }

  i3ds::Context::Ptr context;
  if (addr_server.empty()) {
    context = i3ds::Context::Create();
  } else {
    context = std::make_shared<i3ds::Context>(addr_server);
  }
  

  BOOST_LOG_TRIVIAL(info) << "Connecting to power server with node ID: " << node_id;
  i3ds::PowerClient power_client(context, node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  if (set_ports) {
    BOOST_LOG_TRIVIAL(info) << "Setting power switches:";
    for (size_t i=0; i<enable_ports.size(); i++) {
      BOOST_LOG_TRIVIAL(info) << "  " << enable_ports[i];
    }
    
    power_client.set_channels(i3ds::PowerOutputSet(enable_ports.begin(), enable_ports.end()));
    return 0;
  }

  if (enable_ports.size() > 0) {
    BOOST_LOG_TRIVIAL(info) << "Enabling power switches:";
    for (size_t i=0; i<enable_ports.size(); i++) {
      BOOST_LOG_TRIVIAL(info) << "  " << enable_ports[i];
    }
    power_client.enable_channels(i3ds::PowerOutputSet(enable_ports.begin(), enable_ports.end()));
  }
  if (disable_ports.size() > 0) {
    BOOST_LOG_TRIVIAL(info) << "Disabling power switches:";
    for (size_t i=0; i<disable_ports.size(); i++) {
      BOOST_LOG_TRIVIAL(info) << "  " << disable_ports[i];
    }
    power_client.disable_channels(i3ds::PowerOutputSet(disable_ports.begin(), disable_ports.end()));
  }
  

  return 0;
}
