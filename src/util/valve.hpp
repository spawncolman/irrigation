#ifndef UTIL_MOTOR_MOTOR_HPP
#define UTIL_MOTOR_MOTOR_HPP

#include <cstdint>

/**
 * @file valve.hpp
 * @brief Small valve driver abstraction used by the irrigation controller.
 *
 * This header defines a minimal Valve class that supports two wiring types:
 * - RELAY: simple on/off control
 * - SOLENOID: bidirectional control using two direction pins and a PWM enable
 *
 * The implementation intentionally avoids use of the C++ standard library to
 * remain suitable for constrained embedded environments.
 */

/**
 * @brief Enumerates valve commands.
 */
enum ValveCommand : uint8_t {
    COMMAND_UNKNOWN = 0,
    VALVE_OPEN = 1,
    VALVE_CLOSE = 2
};

/**
 * @brief Enumerates valve hardware wiring types.
 */
enum ValveType : uint8_t { NOT_COFIGURED = 0, RELAY = 1, SOLENOID = 2 };

namespace util::valve {

/**
 * @class Valve
 * @brief Represents a single valve and provides open/close operations.
 *
 * The class stores pin assignments but does not allocate dynamic memory.
 * The consumer is responsible for calling `setup()` once before using open/close.
 */
class Valve {
  public:
    /**
     * @brief Construct a Valve instance.
     * @param type Wiring type (RELAY or SOLENOID)
     * @param enable Pin used to enable motor/PWM (or relay control)
     * @param in1 Direction/control pin 1
     * @param in2 Direction/control pin 2
     */
    Valve(ValveType type, uint8_t enable, uint8_t in1, uint8_t in2);

    /**
     * @brief Configure GPIOs. Must be called once before using the valve.
     */
    void setup();

    /**
     * @brief Open the valve.
     */
    void open();

    /**
     * @brief Close the valve.
     */
    void close();

  private:
    ValveType type;
    uint8_t enable;
    uint8_t in1;
    uint8_t in2;
    // Note: implementation internal helpers are defined in the .cpp file.
};

} // namespace util::valve

#endif // UTIL_MOTOR_MOTOR_HPP
