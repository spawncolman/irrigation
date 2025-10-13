#ifndef UTIL_SENSOR_HUMIDITY_HPP
#define UTIL_SENSOR_HUMIDITY_HPP

#include <cstdint>

namespace util::sensor {

class Humidity {
  public:
    Humidity(uint8_t pin);

    void setup();

    float measure(uint16_t samplesNumber);

  private:
    uint8_t pin;
};

} // namespace util::sensor

#endif // UTIL_SENSOR_HUMIDITY_HPP
