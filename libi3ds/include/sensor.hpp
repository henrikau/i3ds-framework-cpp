#include "SensorSuite.h"

namespace i3ds
{
  class Sensor
  {
  public:

    Sensor(SensorID id);
    virtual ~Sensor();

    // Get sensor ID.
    SensorID get_id() const {return _id;}

    // Get sensor state.
    SensorState get_state() const {return _state;}

    // Get sensor rate.
    SensorRate get_rate() const {return _rate;}

  protected:

    CommandResult handle_sensor_command(SensorCommand& command);

    virtual void do_activate() = 0;
    virtual void do_start() = 0;
    virtual void do_stop() = 0;
    virtual void do_deactivate() = 0;

    virtual bool support_rate(SensorRate rate) = 0;

  private:

    CommandResult handle_state_command(StateCommand command);
    CommandResult handle_rate_command(SensorRate rate);

    SensorID _id;
    SensorState _state;
    SensorRate _rate;
  };

} // namespace i3ds
