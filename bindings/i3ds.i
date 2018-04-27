// -*- Mode: c++ -*-
%{

#include "i3ds/communication.hpp"

#include "i3ds/client_factory.hpp"
#include "i3ds/sensor_client.hpp"
#include "i3ds/camera_client.hpp"

using namespace i3ds;
%}

%include "stdint.i"
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_shared_ptr.i"

%shared_ptr(i3ds::Context)
%shared_ptr(i3ds::ClientFactory)
%shared_ptr(i3ds::Client)
%shared_ptr(i3ds::SensorClient)
%shared_ptr(i3ds::CameraClient)

typedef long long asn1SccSint;
typedef unsigned long long asn1SccUint;

typedef asn1SccSint T_Int8;
typedef asn1SccUint T_UInt8;
typedef asn1SccSint T_Int16;
typedef asn1SccUint T_UInt16;
typedef asn1SccSint T_Int32;
typedef asn1SccUint T_UInt32;
typedef asn1SccSint T_Int64;
typedef asn1SccUint T_UInt64;

typedef bool T_Boolean;
typedef double T_Double;
typedef double T_Float;

typedef struct {
  int nCount;   
  byte arr[256];
} T_String;

typedef asn1SccUint NodeId;
typedef asn1SccUint EndpointId;

typedef enum {
    success = 0,
    error_node_id = 1,
    error_endpoint_id = 2,
    error_unsupported = 3,
    error_state = 4,
    error_value = 5,
    error_other = 6
} ResultCode;

typedef struct {
    ResultCode result;
    T_String message;
} CommandResponse;

typedef enum {
    activate = 0,
    start = 1,
    stop = 2,
    deactivate = 3
} StateCommand;

typedef enum {
    inactive = 0,
    standby = 1,
    operational = 2,
    failure = 3
} SensorState;

typedef struct {
    T_UInt16 offset_x;
    T_UInt16 offset_y;
    T_UInt16 size_x;
    T_UInt16 size_y;
} PlanarRegion;

namespace i3ds {
  
class Context
{
public:
  typedef std::shared_ptr<Context> Ptr;

  Context(std::string addr_srv_addr = "tcp://localhost:6000");
  virtual ~Context() {};
  static Ptr Create();
};

class Client
{
public:
  typedef std::shared_ptr<Client> Ptr;

  Client(Context::Ptr context, NodeID node);
  virtual ~Client();

  NodeID node() const;
  void Stop();
  int timeout() const;
  void set_timeout(int t);
};

class SensorClient : public Client
{
public:
  typedef std::shared_ptr<SensorClient> Ptr;

  SensorClient(Context::Ptr context, NodeID sensor);

  void set_state(StateCommand state);
  void set_rate(SampleRate rate);

  SensorState state() const;
  Temperature temperature() const;

  void load_status();
  virtual void load_config();
  virtual void load_all();
};

class CameraClient : public SensorClient
{
public:
  typedef std::shared_ptr<CameraClient> Ptr;

  CameraClient(Context::Ptr context, NodeID sensor);

  void set_exposure(ExposureTime exposure, SensorGain gain);
  void set_auto_exposure(bool enable, ExposureTime max_exposure, SensorGain max_gain);
  void set_region(bool enable, PlanarRegion region);
  void set_flash(bool enable, FlashStrength strength);
  void set_pattern(bool enable, PatternSequence sequence);

  ExposureTime exposure() const;
  SensorGain gain() const;
  bool auto_exposure_enabled() const;
  ExposureTime max_exposure() const;
  SensorGain max_gain() const;
  bool region_enabled() const;
  PlanarRegion region() const;
  bool flash_enabled() const;
  FlashStrength flash_strength() const;
  bool pattern_enabled() const;
  PatternSequence pattern_sequence() const;

  virtual void load_config();
};

class ClientFactory
{
public:

  typedef std::shared_ptr<ClientFactory> Ptr;

  static Ptr Create(Context::Ptr context);

  ClientFactory(Context::Ptr context);
  virtual ~ClientFactory();

  ToFCameraClient::Ptr CreateToFCamera(NodeID node);
  CameraClient::Ptr CreateCamera(NodeID node);

private:

  Context::Ptr context_;
  NodeID next_id_;
};
  
%clearnodefaultctor;
} // namespace i3ds
