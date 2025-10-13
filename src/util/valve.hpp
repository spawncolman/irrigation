#ifndef UTIL_MOTOR_MOTOR_HPP
#define UTIL_MOTOR_MOTOR_HPP

#include <cstdint>

/**
 * @brief enumerate of valve commands
 *
 */
enum ValveCommand : uint8_t {
    COMMAND_UNKNOWN = 0,
    VALVE_OPEN = 1,
    VALVE_CLOSE = 2
};

/**
 * @brief enumerate of valve types.
 *
 */
enum ValveType : uint8_t { NOT_COFIGURED, RELAY, SOLENOID };

namespace util::valve {

class Valve {
  public:
    Valve(ValveType type, uint8_t enable, uint8_t in1, uint8_t in2);

    void setup();

    void open();

    void close();

  private:
    ValveType type;
    uint8_t enable;
    uint8_t in1;
    uint8_t in2;
};

} // namespace util::valve

#endif // UTIL_MOTOR_MOTOR_HPP
