///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_EMULATED_CAMERA_HPP
#define __I3DS_EMULATED_CAMERA_HPP

#include "camera.hpp"

namespace i3ds
{

class EmulatedCamera : public Camera
{
public:

  EmulatedCamera(Context::Ptr context, NodeID id);
  virtual ~EmulatedCamera();

  virtual double temperature() const;

protected:

  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

  virtual bool support_rate(SensorRate rate);

  // Setters.
  virtual ResultCode set_exposure(ExposureTime exposure);
  virtual ResultCode set_gain(SensorGain gain);
  virtual ResultCode set_auto_exposure(bool auto_exposure);
  virtual ResultCode set_exposure_limit(ExposureTime exposure_limit);
  virtual ResultCode set_gain_limit(SensorGain gain_limit);
  virtual ResultCode set_region(PlanarRegion region);
  virtual ResultCode set_flash_illumination(bool flash_enable);
  virtual ResultCode set_flash_strength(FlashStrength flash_strength);
  virtual ResultCode set_pattern_illumination(bool pattern_enabled);
  virtual ResultCode set_illumination_pattern(IlluminationPattern pattern);

  // Getters.
  virtual void get_exposure(ExposureTime& exposure) const {exposure =  exposure_;}
  virtual void get_gain(SensorGain& gain) const {gain = gain_;}
  virtual void get_auto_exposure(bool& auto_exposure) const {auto_exposure = auto_exposure_;}
  virtual void get_exposure_limit(ExposureTime& exposure_limit) const {exposure_limit = exposure_limit_;}
  virtual void get_gain_limit(SensorGain& gain_limit) const {gain_limit = gain_limit_;}
  virtual void get_region(PlanarRegion& region) const {region = region_;}
  virtual void get_flash_illumination(bool& flash_enable) const {flash_enable = flash_enable_;}
  virtual void get_flash_strength(FlashStrength& flash_strength) const {flash_strength = flash_strength_;}
  virtual void get_pattern_illumination(bool& pattern_enabled) const {pattern_enabled = pattern_enabled_;}
  virtual void get_illumination_pattern(IlluminationPattern& pattern) const {pattern = pattern_;}

private:

  ExposureTime exposure_;
  SensorGain gain_;
  bool auto_exposure_;
  ExposureTime exposure_limit_;
  SensorGain gain_limit_;
  PlanarRegion region_;
  bool flash_enable_;
  FlashStrength flash_strength_;
  bool pattern_enabled_;
  IlluminationPattern pattern_;
};

} // namespace i3ds

#endif
