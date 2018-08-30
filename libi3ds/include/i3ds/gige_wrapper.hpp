///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_GIGE_WRAPPER_HPP
#define __I3DS_GIGE_WRAPPER_HPP

#include <i3ds/gige_camera_sensor.hpp>

namespace i3ds
{

class GigEWrapper
{
public:

  virtual ~GigEWrapper() {};

  // Camera control
  virtual void Open(const GigECamera::Parameters& param) = 0;
  virtual void Close() = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;

  // Set internal trigger to the given period.
  virtual bool setInternalTrigger(int period_us) = 0;

  // Sensor width and height
  virtual int getSensorWidth() = 0;
  virtual int getSensorHeight() = 0;

  // Region of interest
  virtual bool isRegionSupported() = 0;

  virtual int getRegionWidth() = 0;
  virtual int getRegionHeight() = 0;
  virtual int getRegionOffsetX() = 0;
  virtual int getRegionOffsetY() = 0;

  virtual void setRegionWidth(int width) = 0;
  virtual void setRegionHeight(int height) = 0;
  virtual void setRegionOffsetX(int offset_x) = 0;
  virtual void setRegionOffsetY(int offset_y) = 0;

  // Shutter time in microseconds
  virtual int getShutter() = 0;
  virtual int getMaxShutter() = 0;
  virtual int getMinShutter() = 0;
  virtual void setShutter(int shutter_us) = 0;

  virtual bool isAutoShutterSupported() = 0;

  virtual bool getAutoShutterEnabled() = 0;
  virtual void setAutoShutterEnabled(bool enable) = 0;

  virtual int getAutoShutterLimit() = 0;
  virtual int getMaxAutoShutterLimit() = 0;
  virtual int getMinAutoShutterLimit() = 0;
  virtual void setAutoShutterLimit(int shutter_limit) = 0;

  // Gain in decibel
  virtual double getGain() = 0;
  virtual double getMaxGain() = 0;
  virtual double getMinGain() = 0;
  virtual void setGain(double gain) = 0;

  virtual bool isAutoGainSupported() = 0;

  virtual bool getAutoGainEnabled() = 0;
  virtual void setAutoGainEnabled(bool enable) = 0;

  virtual double getAutoGainLimit() = 0;
  virtual double getMaxAutoGainLimit() = 0;
  virtual double getMinAutoGainLimit() = 0;
  virtual void setAutoGainLimit(double gain_limit) = 0;
};

} // namespace i3ds

#endif
