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
  virtual ExposureTime exposure() const {return exposure_;}
  virtual SensorGain gain() const {return gain_;}
  virtual bool auto_exposure_enabled() const {return auto_exposure_enabled_;}
  virtual ExposureTime max_exposure() const {return max_exposure_;}
  virtual SensorGain max_gain() const {return max_gain_;}
  virtual bool region_enabled() const {return region_enabled_;}
  virtual PlanarRegion region() const {return region_;}
  virtual bool flash_enabled() const {return flash_enabled_;}
  virtual FlashStrength flash_strength() const {return flash_strength_;}
  virtual bool pattern_enabled() const {return pattern_enabled_;}
  virtual PatternSequence pattern_sequence() const {return pattern_sequence_;}

  // Setter commands.
  virtual void set_exposure(ExposureTime exposure, SensorGain gain);
  virtual void set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain);
  virtual void set_region(bool enable, PlanarRegion region);
  virtual void set_flash(bool enable, FlashStrength strength);
  virtual void set_pattern(bool enable, PatternSequence sequence);

  // Supported rate.
  virtual bool support_rate(SampleRate rate);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

private:

  const int resx_;
  const int resy_;

  ExposureTime exposure_;
  SensorGain gain_;
  bool auto_exposure_enabled_;
  ExposureTime max_exposure_;
  SensorGain max_gain_;
  bool region_enabled_;
  PlanarRegion region_;
  bool flash_enabled_;
  FlashStrength flash_strength_;
  bool pattern_enabled_;
  PatternSequence pattern_sequence_;
};

} // namespace i3ds

#endif
