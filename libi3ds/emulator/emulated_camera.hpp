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

  EmulatedCamera(Context::Ptr context, NodeID id, int resx, int resy);
  virtual ~EmulatedCamera();

  // Getters.
  virtual int resolution_x() const {return resx_;}
  virtual int resolution_y() const {return resy_;}
  virtual ExposureTime exposure() const {return exposure_;}
  virtual SensorGain gain() const {return gain_;}
  virtual bool auto_exposure() const {return auto_exposure_;}
  virtual ExposureTime exposure_limit() const {return exposure_limit_;}
  virtual SensorGain gain_limit() const {return gain_limit_;}
  virtual PlanarRegion region() const {return region_;}
  virtual bool flash_enabled() const {return flash_enabled_;}
  virtual FlashStrength flash_strength() const {return flash_strength_;}
  virtual bool pattern_enabled() const {return pattern_enabled_;}
  virtual IlluminationPattern illumination_pattern() const {return pattern_;}

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
  virtual ResultCode set_flash_enabled(bool flash_enabled);
  virtual ResultCode set_flash_strength(FlashStrength flash_strength);
  virtual ResultCode set_pattern_enabled(bool pattern_enabled);
  virtual ResultCode set_illumination_pattern(IlluminationPattern pattern);

private:

  const int resx_;
  const int resy_;

  ExposureTime exposure_;
  SensorGain gain_;
  bool auto_exposure_;
  ExposureTime exposure_limit_;
  SensorGain gain_limit_;
  PlanarRegion region_;
  bool flash_enabled_;
  FlashStrength flash_strength_;
  bool pattern_enabled_;
  IlluminationPattern pattern_;
};

} // namespace i3ds

#endif
