#include "current_sensor.hpp"
#include "Arduino.h"

/* constants */

// Sensor sensitivity in V/A. Uncomment or change to match the sensor
// variant in use (5A, 20A, 30A, ...). Keep as constexpr to allow
// compile-time folding.
//static constexpr float SENSITIVITY_5A = 0.185F;
//static constexpr float SENSITIVITY_20A = 0.100F;
static constexpr float SENSITIVITY_30A = 0.066F;

// Select the sensitivity used by this board (adjust if you change
// the sensor model).
static constexpr float SENSOR_SENSIBILITY = SENSITIVITY_30A; // V/A

// ADC reference assumptions and arithmetic constants.
static constexpr float ADC_REF_VOLTAGE = 3.3F;    // ADC reference (V)
static constexpr float ADC_MAX_VALUE = 1023.0F;   // 10-bit ADC maximum

// Resistors used in the input divider (ohms). If your hardware
// changes, update these constants accordingly.
static constexpr float DIV_R1 = 6800.0F;
static constexpr float DIV_R2 = 12000.0F;

// Midpoint voltage of sensor (V). Many ACS sensors output ~Vcc/2 at
// zero current; adjust if your sensor differs.
// https://medium.com/@keirish15/acs-712-20a-current-sensor-interfacing-with-esp-32-5a3d240acfe9
static constexpr float SENSOR_ZERO_VOLTAGE = 2.5F;

// Maximum sensible samples for a single call to measure(). This
// prevents accidental extremely large loops.
static constexpr uint16_t MAX_SAMPLES = 10000U;

// Note: the original implementation included a remark about ESP32
// ADC differences; this implementation assumes ADC voltage range of
// ADC_REF_VOLTAGE. If targeting other MCUs adjust constants above.

namespace util::sensor {

CurrentSensor::CurrentSensor(uint8_t pin) : pin(pin) {}

void CurrentSensor::setup() {}

float CurrentSensor::measure(uint16_t samplesNumber) {
    uint16_t ind = 0;
    float currentAverage = 0.0f;
    
    // Validate input and cap to a reasonable maximum
    if (samplesNumber == 0U) {
        return 0.0F;
    }

    if (samplesNumber > MAX_SAMPLES) {
        samplesNumber = MAX_SAMPLES;
    }

    for (uint16_t i = 0U; i < samplesNumber; ++i) {
        int adc = analogRead(pin);

        // Convert ADC reading to voltage using assumed ADC reference
        float adc_voltage = static_cast<float>(adc) * (ADC_REF_VOLTAGE / ADC_MAX_VALUE);

        // Account for voltage divider: sensor voltage before divider
        float sensor_voltage = adc_voltage * ((DIV_R1 + DIV_R2) / DIV_R2);

        float current = (sensor_voltage - SENSOR_ZERO_VOLTAGE) / SENSOR_SENSIBILITY;

        currentAverage += current;
    }

    return (currentAverage / static_cast<float>(samplesNumber));
}

} // namespace util::sensor
