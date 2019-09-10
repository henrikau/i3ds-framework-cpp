///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include <i3ds/sensor_client.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace i3ds
{

class Configurator
{
public:
  void add_common_options(po::options_description& desc);
  po::variables_map parse_common_options(po::options_description desc, int argc, char *argv[]);
};

class SensorConfigurator : public Configurator
{
private:

public:
  NodeID node_id;
  SamplePeriod period;

  SensorConfigurator() {}
  virtual ~SensorConfigurator() {}

  virtual void add_common_options(po::options_description& desc);

  void handle_sensor_commands(po::variables_map& vm, SensorClient& client);

  void print_sensor_status(SensorClient& client);

};

} // namespace i3ds

#endif
