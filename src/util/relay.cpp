#include "relay.hpp"

#include <Arduino.h>

namespace util::relay {

Relay::Relay(uint8_t pin) : relayPin(pin) {}

void Relay::setup() {
    /* pin setup */
    pinMode(relayPin, OUTPUT);
}

void Relay::enable() { digitalWrite(relayPin, HIGH); }

void Relay::disable() {digitalWrite(relayPin, LOW);}

} // namespace util::relay
