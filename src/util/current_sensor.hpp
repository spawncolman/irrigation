#ifndef UTIL_SENSOR_CURRENTSENSOR_HPP
#define UTIL_SENSOR_CURRENTSENSOR_HPP

#include <cstdint>

/**
 * @file current_sensor.hpp
 * @brief Simple ADC-based current sensor helper.
 *
 * This small class wraps a single analog input connected to a current
 * sensor (for example ACS7xx series) and provides a blocking sampling
 * method that returns the averaged current in amperes.
 *
 * Important assumptions and notes:
 * - The implementation expects an ADC reference voltage and scaling
 *   compatible with the sensor. On many ESP32 boards the ADC range is
 *   0..3.3V; the code uses this assumption when converting ADC counts
 *   to voltage.
 * - The sensor sensitivity (V/A) must match the hardware variant and
 *   is configured in the implementation file. Verify the constant if
 *   you change the sensor model (e.g. 5A/20A/30A variants).
 * - The measurement method performs a simple average over a number of
 *   samples provided by the caller. This is intentionally simple and
 *   deterministic for embedded use.
 * - This helper keeps the API minimal and avoids dynamic allocation.
 */

namespace util::sensor {

/**
 * @brief Lightweight wrapper for a single ADC current sensor.
 *
 * Usage example:
 * @code
 * util::sensor::CurrentSensor cs(A0);
 * cs.setup();
 * float amps = cs.measure(100);
 * @endcode
 */
class CurrentSensor {
  public:
    /**
     * @brief Construct a sensor helper for the given analog pin.
     *
     * @param pin Analog pin (ADC channel) where the sensor output is
     *            connected.
     */
    explicit CurrentSensor(uint8_t pin);

    /**
     * @brief Initialize hardware if required.
     *
     * For many MCUs no explicit setup is required for analog inputs; the
     * method exists for API symmetry and future needs.
     */
    void setup();

    /**
     * @brief Measure the current by averaging a number of ADC samples.
     *
     * The function performs blocking analog reads and computes the sensor
     * output voltage and converts it to current using the sensitivity
     * constant present in the implementation file.
     *
     * @param samplesNumber Number of ADC samples to average (must be > 0).
     * @return float Averaged current in amperes.
     */
    float measure(uint16_t samplesNumber);

  private:
    uint8_t pin; /**< ADC pin / channel */
};

} // namespace util::sensor

#endif // UTIL_SENSOR_CURRENTSENSOR_HPP
