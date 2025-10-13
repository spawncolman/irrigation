#include "current_sensor.hpp"
#include "Arduino.h"

/* constants */

// Sensibilidad del sensor en V/A
// static float SENSIBILITY = 0.185; // Modelo 5A
// static float SENSIBILITY = 0.100; // Modelo 20A
static float SENSIBILITY = 0.066; // Modelo 30A

static float VOLTS_5V = 5.0;
static float ADC_RESOLUTION = 1023.0;

float R1 = 6800.0;
float R2 = 12000.0;

// este codigo no es directamente compatible con ESP32
// ver:
// https://medium.com/@keirish15/acs-712-20a-current-sensor-interfacing-with-esp-32-5a3d240acfe9

namespace util::sensor {

CurrentSensor::CurrentSensor(uint8_t pin) : pin(pin) {}

void CurrentSensor::setup() {}

float CurrentSensor::measure(uint16_t samplesNumber) {
    uint16_t ind = 0;
    float currentAverage = 0.0f;

    for (uint16_t ind = 0; ind < samplesNumber; ++ind) {
        int adc = analogRead(pin);
        float adc_voltage = adc * (3.3 / ADC_RESOLUTION);
        float current_voltage = (adc_voltage * (R1 + R2) / R2);
        float current = (current_voltage - 2.5) / SENSIBILITY;

        currentAverage += current;
    }

    return (currentAverage / samplesNumber);
}

} // namespace util::sensor
