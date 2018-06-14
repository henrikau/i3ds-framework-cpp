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

#include <i3ds/topic.hpp>
#include <i3ds/publisher.hpp>
#include <i3ds/periodic.hpp>

#include <i3ds/camera_sensor.hpp>

#define BOOST_LOG_DYN_LINK

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace logging = boost::log;

namespace i3ds
{

struct CameraProperties
{
  Frame_mode_t mode;
  int data_depth;
  int pixel_size;
  int width;
  int height;
  int image_count;
  std::string sample_dir;
};

class EmulatedCamera : public Camera
{
public:

  typedef std::shared_ptr<EmulatedCamera> Ptr;

  static Ptr Create(Context::Ptr context, NodeID id, CameraProperties prop)
  {
    return std::make_shared<EmulatedCamera>(context, id, prop);
  }

  EmulatedCamera(Context::Ptr context, NodeID id, CameraProperties prop);
  virtual ~EmulatedCamera();

  // Getters.
  virtual ShutterTime shutter() const {return shutter_;}
  virtual SensorGain gain() const {return gain_;}
  virtual bool auto_exposure_enabled() const {return auto_exposure_enabled_;}
  virtual ShutterTime max_shutter() const {return max_shutter_;}
  virtual SensorGain max_gain() const {return max_gain_;}
  virtual bool region_enabled() const {return region_enabled_;}
  virtual PlanarRegion region() const {return region_;}
  virtual bool flash_enabled() const {return flash_enabled_;}
  virtual FlashStrength flash_strength() const {return flash_strength_;}
  virtual bool pattern_enabled() const {return pattern_enabled_;}
  virtual PatternSequence pattern_sequence() const {return pattern_sequence_;}

  // Supported period.
  virtual bool is_sampling_supported(SampleCommand sample);

protected:

  // Actions.
  virtual void do_activate();
  virtual void do_start();
  virtual void do_stop();
  virtual void do_deactivate();

  // Handlers.
  virtual void handle_exposure(ExposureService::Data& command);
  virtual void handle_region(RegionService::Data& command);
  virtual void handle_auto_exposure(AutoExposureService::Data& command);
  virtual void handle_flash(FlashService::Data& command);
  virtual void handle_pattern(PatternService::Data& command);

  void load_images();
  cv::Mat next_image();
  bool send_sample(unsigned long timestamp_us);

  ShutterTime shutter_;
  SensorGain gain_;
  bool auto_exposure_enabled_;
  ShutterTime max_shutter_;
  SensorGain max_gain_;
  bool region_enabled_;
  PlanarRegion region_;
  bool flash_enabled_;
  FlashStrength flash_strength_;
  bool pattern_enabled_;
  PatternSequence pattern_sequence_;

  CameraProperties prop_;

  Sampler sampler_;
  Publisher publisher_;

  std::vector<cv::Mat> sample_images_;
  unsigned int current_image_index_;
};

} // namespace i3ds

#endif
