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

#include <memory>

#include <i3ds/periodic.hpp>

#include <i3ds/gige_camera_sensor.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace logging = boost::log;

namespace i3ds
{

class EmulatedCamera : public GigECamera
{
public:

  struct Parameters : public GigECamera::Parameters
  {
    int64_t height;
    int64_t width;
    std::string sample_dir;
  };

  typedef std::shared_ptr<EmulatedCamera> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id, Parameters param)
  {
    return std::make_shared<EmulatedCamera>(context, id, param);
  }

  EmulatedCamera(Context::Ptr context, NodeID id, Parameters param);
  virtual ~EmulatedCamera();

protected:

  // Camera control
  virtual void Open();
  virtual void Close();
  virtual void Start();
  virtual void Stop();

  // Set internal trigger to the given period.
  virtual bool setInternalTrigger(int64_t period_us);

  // Sensor width and height
  virtual int64_t getSensorWidth() const;
  virtual int64_t getSensorHeight() const;

  // Region of interest
  virtual bool isRegionSupported() const;

  virtual int64_t getRegionWidth() const;
  virtual int64_t getRegionHeight() const;
  virtual int64_t getRegionOffsetX() const;
  virtual int64_t getRegionOffsetY() const;

  virtual void setRegionWidth(int64_t width);
  virtual void setRegionHeight(int64_t height);
  virtual void setRegionOffsetX(int64_t offset_x);
  virtual void setRegionOffsetY(int64_t offset_y);

  // Shutter time in microseconds
  virtual int64_t getShutter() const;
  virtual int64_t getMaxShutter() const;
  virtual int64_t getMinShutter() const;
  virtual void setShutter(int64_t shutter_us);

  virtual bool isAutoShutterSupported() const;

  virtual bool getAutoShutterEnabled() const;
  virtual void setAutoShutterEnabled(bool enable);

  virtual int64_t getAutoShutterLimit() const;
  virtual int64_t getMaxAutoShutterLimit() const;
  virtual int64_t getMinAutoShutterLimit() const;
  virtual void setAutoShutterLimit(int64_t shutter_limit);

  // Gain in decibel
  virtual double getGain() const;
  virtual double getMaxGain() const;
  virtual double getMinGain() const;
  virtual void setGain(double gain);

  virtual bool isAutoGainSupported() const;

  virtual bool getAutoGainEnabled() const;
  virtual void setAutoGainEnabled(bool enable);

  virtual double getAutoGainLimit() const;
  virtual double getMaxAutoGainLimit() const;
  virtual double getMinAutoGainLimit() const;
  virtual void setAutoGainLimit(double gain_limit);

  bool check_region() const;

  void load_images(std::string sample_dir);
  cv::Mat next_image();

  bool generate_sample();

  int64_t shutter_;
  double gain_;

  bool auto_shutter_enabled_;
  int64_t auto_shutter_limit_;

  bool auto_gain_enabled_;
  double auto_gain_limit_;

  bool region_enabled_;
  cv::Rect region_, sensor_;

  int64_t period_;

  Sampler sampler_;

  std::vector<cv::Mat> sample_images_;
  unsigned int current_image_index_;
};

} // namespace i3ds

#endif
