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

// Forward declaration of GigE Vision wrapper.
class GigEWrapper;

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
  GigECamera(Context::Ptr context, NodeID node, Parameters param, GigEWrapper& wrapper);

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

  // Send image sample, intented to be called by GigE wrapper.
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

  // Constant parameters for GigE camera.
  const Parameters param_;

  // Wrapper for communicating with specific GigE camera bus/device.
  GigEWrapper& wrapper_;

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
