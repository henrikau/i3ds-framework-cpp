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

#include <i3ds/flash_client.hpp>
#include <i3ds/configurator.hpp>

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
  int strength;
  int duration;


  i3ds::Configurator configurator;
  po::options_description desc("Allowed flash control options");
  configurator.add_common_options(desc);

  desc.add_options()
  ("node,n", po::value(&node_id)->default_value(25), "NodeID")
  ("strength,s", po::value(&strength), "Flash strength (0-100%)")
  ("duration,d", po::value(&duration), "Duration of flash (10-3000000  us)\n(Be aware, there are some limitations between strength and duration.)")
  ;
  po::variables_map vm = configurator.parse_common_options(desc, argc, argv);

  i3ds::Context::Ptr context;

  context = i3ds::Context::Create();
  BOOST_LOG_TRIVIAL(info) << "Connecting to Wide Angular Flash Server with node ID: " << node_id;
  i3ds::FlashClient flash_client(context, node_id);
  BOOST_LOG_TRIVIAL(trace) << "---> [OK]";

  flash_client.set_flash(duration, strength);

  return 0;
}
