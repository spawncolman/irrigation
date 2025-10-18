#ifndef UTIL_SENSOR_HUMIDITY_HPP
#define UTIL_SENSOR_HUMIDITY_HPP

#include <cstdint>

/**
 * @file humidity.hpp
 * @brief Minimal ADC-based humidity input helper.
 *
 * This helper provides a very small API to sample an analog pin and
 * return the averaged ADC reading. It does not perform any conversion
 * to physical relative humidity because that requires a specific
 * sensor model and calibration curve.
 */

namespace util::sensor {

class Humidity {
  public:
    /**
     * @brief Construct the helper bound to an analog pin.
     * @param pin Analog pin (ADC channel) connected to the sensor.
     */
    explicit Humidity(uint8_t pin);

    /**
     * @brief Perform any hardware initialization required.
     *
     * Typically analog inputs require no explicit setup on many MCUs,
     * so this function is provided for API symmetry and future use.
     */
    void setup();

    /**
     * @brief Measure the sensor by averaging ADC samples.
     *
     * The returned value is the average ADC counts as a floating point
     * number. Caller is responsible for providing a sensible
     * `samplesNumber` (e.g., 10..500). The function is blocking and
     * intended for occasional sampling.
     *
     * @param samplesNumber Number of raw ADC samples to average.
     * @return float Averaged ADC reading (raw units). Convert to
     *         physical units using a sensor-specific formula.
     */
    float measure(uint16_t samplesNumber);

  private:
    uint8_t pin; /**< Analog input pin / ADC channel */
};

} // namespace util::sensor

#endif // UTIL_SENSOR_HUMIDITY_HPP
