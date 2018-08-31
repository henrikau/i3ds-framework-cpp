///////////////////////////////////////////////////////////////////////////\file
///
///   Copyright 2018 SINTEF AS
///
///   This Source Code Form is subject to the terms of the Mozilla
///   Public License, v. 2.0. If a copy of the MPL was not distributed
///   with this file, You can obtain one at https://mozilla.org/MPL/2.0/
///
////////////////////////////////////////////////////////////////////////////////

#ifndef __I3DS_GIGE_CAMERA_HPP
#define __I3DS_GIGE_CAMERA_HPP

#include <i3ds/camera_sensor.hpp>
#include <i3ds/trigger_client.hpp>
#include <i3ds/flash_client.hpp>
#include <i3ds/publisher.hpp>

namespace i3ds
{

class GigECamera : public Camera
{
public:

  struct Parameters
  {
    // GigE vision name of camera.
    std::string camera_name;

    // True if external trigger is to be used.
    bool external_trigger;

    // True if flash is supported.
    bool support_flash;

    // True if pattern is supported.
    bool support_pattern;

    // Image count (e.g. 1 = mono, 2 = stereo)
    int image_count;

    // Frame mode (defaults to mono)
    Frame_mode_t frame_mode;

    // Data bit-depth and bytes per pixel
    int data_depth;
    int pixel_size;

    // GigE vision tuning of packet size (jumbo packets)
    int packet_size;

    // GigE vision tuning of inter-packet delay
    int packet_delay;

    // Configuration for external trigger.
    int trigger_source;
    int camera_output;
    int camera_offset;
    int flash_output;
    int flash_offset;
    int pattern_output;
    int pattern_offset;

    // Node ID for trigger and flash services if supported.
    NodeID trigger_node;
    NodeID flash_node;
  };

  // Shared pointer for GigE camera interfaces.
  typedef std::shared_ptr<GigECamera> Ptr;

  // Constructor for GigE camera.
  GigECamera(Context::Ptr context, NodeID node, Parameters param);

  // Destructor for GigE camera.
  virtual ~GigECamera();

  // Get shutter time for camera in microseconds.
  virtual ShutterTime shutter() const;

  // Get gain for camera in decibel.
  virtual SensorGain gain() const;

  // Get auto exposure for camera.
  virtual bool auto_exposure_enabled() const;

  // Get shutter time limit for camera with auto exposure in microseconds.
  virtual ShutterTime max_shutter() const;

  // Get gain limit for camera with auto exposure in decibel.
  virtual SensorGain max_gain() const;

  // Get the region of interest enabled for camera.
  virtual bool region_enabled() const;

  // Get the region of interest for the camera.
  virtual PlanarRegion region() const;

  // Get the flash illumination for the camera.
  virtual bool flash_enabled() const {return flash_enabled_;}

  // Get the flash strength for the camera.
  virtual FlashStrength flash_strength() {return flash_strength_;}

  // Get the pattern illumination for the camera.
  virtual bool pattern_enabled() const {return pattern_enabled_;}

  // Get the pattern sequence for the camera.
  virtual PatternSequence pattern_sequence() const {return pattern_sequence_;}

  // Returns true if sample configuration is supported.
  virtual bool is_sampling_supported(SampleCommand sample);

  // Sensor action when activated.
  virtual void do_activate();

  // Sensor action when started.
  virtual void do_start();

  // Sensor action when activated.
  virtual void do_stop();

  // Sensor action when activated.
  virtual void do_deactivate();

protected:

  // Constant parameters for GigE camera.
  const Parameters param_;

  // Camera control
  virtual void Open() = 0;
  virtual void Close() = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;

  // Set internal trigger to the given period.
  virtual bool setInternalTrigger(int64_t period_us) = 0;

  // Sensor width and height
  virtual int64_t getSensorWidth() const = 0;
  virtual int64_t getSensorHeight() const = 0;

  // Region of interest
  virtual bool isRegionSupported() const = 0;

  virtual int64_t getRegionWidth() const = 0;
  virtual int64_t getRegionHeight() const = 0;
  virtual int64_t getRegionOffsetX() const = 0;
  virtual int64_t getRegionOffsetY() const = 0;

  virtual void setRegionWidth(int64_t width) = 0;
  virtual void setRegionHeight(int64_t height) = 0;
  virtual void setRegionOffsetX(int64_t offset_x) = 0;
  virtual void setRegionOffsetY(int64_t offset_y) = 0;

  // Shutter time in microseconds
  virtual int64_t getShutter() const = 0;
  virtual int64_t getMaxShutter() const = 0;
  virtual int64_t getMinShutter() const = 0;
  virtual void setShutter(int64_t shutter_us) = 0;

  virtual bool isAutoShutterSupported() const = 0;

  virtual bool getAutoShutterEnabled() const = 0;
  virtual void setAutoShutterEnabled(bool enable) = 0;

  virtual int64_t getAutoShutterLimit() const = 0;
  virtual int64_t getMaxAutoShutterLimit() const = 0;
  virtual int64_t getMinAutoShutterLimit() const = 0;
  virtual void setAutoShutterLimit(int64_t shutter_limit) = 0;

  // Gain in decibel
  virtual double getGain() const = 0;
  virtual double getMaxGain() const = 0;
  virtual double getMinGain() const = 0;
  virtual void setGain(double gain) = 0;

  virtual bool isAutoGainSupported() const = 0;

  virtual bool getAutoGainEnabled() const = 0;
  virtual void setAutoGainEnabled(bool enable) = 0;

  virtual double getAutoGainLimit() const = 0;
  virtual double getMaxAutoGainLimit() const = 0;
  virtual double getMinAutoGainLimit() const = 0;
  virtual void setAutoGainLimit(double gain_limit) = 0;

  // Send image sample where multiple images are stacked in height.
  virtual bool send_sample(const byte* image, int width, int height);

  // Handler for camera exposure command, must be overloaded.
  virtual void handle_exposure(ExposureService::Data& command);

  // Handler for camera auto exposure command.
  virtual void handle_auto_exposure(AutoExposureService::Data& command);

  // Handler for camera region command.
  virtual void handle_region(RegionService::Data& command);

  // Handler for camera flash command.
  virtual void handle_flash(FlashService::Data& command);

  // Handler for camera pattern command.
  virtual void handle_pattern(PatternService::Data& command);

private:

  // Client for external trigger configuration, if supported.
  TriggerClient::Ptr trigger_;

  // Set of external triggers enabled.
  TriggerOutputSet trigger_outputs_;

  // Client for flash configuration, if supported.
  FlashClient::Ptr flash_;

  // True if flash is enabled.
  bool flash_enabled_;

  // Configured flash strength.
  FlashStrength flash_strength_;

  // True if pattern illumination is enabled.
  bool pattern_enabled_;

  // Chosen pattern sequence.
  PatternSequence pattern_sequence_;

  // Image stream publisher.
  Publisher publisher_;

  // Helper methods for setting and clearing triggers.
  void set_trigger(TriggerOutput channel, TriggerOffset offset);
  void clear_trigger(TriggerOutput channel);
};

} // namespace i3ds

#endif
