#ifndef UTIL_RELAY_RELAY_HPP
#define UTIL_RELAY_RELAY_HPP

#include <cstdint>

/**
 * @file relay.hpp
 * @brief Small helper for controlling a single relay GPIO.
 *
 * The Relay class provides a minimal, C++-friendly wrapper around a
 * single GPIO pin used to drive a relay. It is intentionally tiny to be
 * suitable for embedded code and to avoid dynamic allocation or use of the
 * C++ standard library.
 *
 * Usage:
 * @code
 * util::relay::Relay pumpRelay(RELAY_PIN);
 * pumpRelay.setup();
 * pumpRelay.enable(); // energize relay
 * // ... later
 * pumpRelay.disable(); // de-energize relay
 * @endcode
 */

namespace util::relay {

/**
 * @class Relay
 * @brief Encapsulates a single relay GPIO.
 *
 * Responsibilities:
 * - Stores the configured pin number.
 * - Exposes setup() to configure the pin as output.
 * - Exposes enable()/disable() to set the output level.
 *
 * Notes:
 * - The class does not claim ownership of hardware resources other than
 *   the pin number and does not perform any concurrency protection.
 * - Callers should avoid calling enable/disable from interrupt context if
 *   the underlying platform API is not interrupt-safe.
 */
class Relay {
  public:
    /**
     * @brief Construct a Relay instance.
     * @param pin GPIO number used to drive the relay coil or transistor.
     */
    explicit Relay(uint8_t pin);

    /**
     * @brief Configure the GPIO pin as an output and set a safe default state.
     *
     * This must be called once before invoking enable() or disable(). The
     * implementation will set the pin low by default to avoid unintentionally
     * energizing the relay after setup.
     */
    void setup();

    /**
     * @brief Energize the relay (set the pin to logical HIGH).
     */
    void enable();

    /**
     * @brief De-energize the relay (set the pin to logical LOW).
     */
    void disable();

  private:
    uint8_t relayPin; /**< GPIO pin number assigned to this relay */
};

} // namespace util::relay

#endif // UTIL_RELAY_RELAY_HPP
