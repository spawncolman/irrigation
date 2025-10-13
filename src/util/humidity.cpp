#include "humidity.hpp"
#include <Arduino.h>

namespace util::sensor {

Humidity::Humidity(uint8_t pin) : pin(pin) {}

void Humidity::setup() {}

float Humidity::measure(uint16_t samplesNumber) {
    float measure = 0.0f;

    for (uint16_t ind = 0; ind < samplesNumber; ++ind) {
        int humedad = analogRead(pin);

        measure += (float)humedad;
    }

    measure /= (float)samplesNumber;
    
    Serial.printf("humidity measure %2.2f\n", measure);
    return measure;
}

} // namespace util::sensor
