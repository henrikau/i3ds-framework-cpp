///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_CAMERA_CLIENT_HPP
#define __I3DS_CAMERA_CLIENT_HPP

#include "i3ds/core/client.hpp"
#include "i3ds/core/subscriber.hpp"
#include "i3ds/sensors/camera.hpp"

namespace i3ds
{

class CameraClient : public Client
{
public:

  CameraClient(Context::Ptr context, NodeID sensor);

  bool set_state(StateCommand state);
  bool set_rate(SampleRate rate);

  bool set_exposure(ExposureTime exposure, SensorGain gain);
  bool set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain);
  bool set_region(bool enable, PlanarRegion region);
  bool set_flash(bool enable, FlashStrength strength);
  bool set_pattern(bool enable, PatternSequence sequence);

  ExposureTime exposure() const {return config_.response.exposure;}
  SensorGain gain() const {return config_.response.gain;}
  bool auto_exposure_enabled() const {return config_.response.auto_exposure_enabled;}
  ExposureTime max_exposure() const {return config_.response.max_exposure;}
  SensorGain max_gain() const {return config_.response.max_gain;}
  bool region_enabled() const {return config_.response.region_enabled;}
  PlanarRegion region() const {return config_.response.region;}
  bool flash_enabled() const {return config_.response.flash_enabled;}
  FlashStrength flash_strength() const {return config_.response.flash_strength;}
  bool pattern_enabled() const {return config_.response.pattern_enabled;}
  PatternSequence pattern_sequence() const {return config_.response.pattern_sequence;}

  bool load_configuration();

private:

  Camera::ConfigurationService::Data config_;

};

} // namespace i3ds

#endif
