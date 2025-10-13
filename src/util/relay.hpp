#ifndef UTIL_RELAY_RELAY_HPP
#define UTIL_RELAY_RELAY_HPP

#include <cstdint>

namespace util::relay {

class Relay {
  public:
    Relay(uint8_t pin);

    void setup();

    void enable();

    void disable();

  private:
    uint8_t relayPin;
};

} // namespace util::relay

#endif // UTIL_RELAY_RELAY_HPP
