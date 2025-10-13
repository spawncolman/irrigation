#include "valve.hpp"
#include "Arduino.h"
#include <cstdint>

/* constants */
static constexpr uint32_t MOTOR_VALVE_DELAY_US = 2005U;
static constexpr uint8_t MOTOR_PWM_VALUE = 250U;

namespace util::valve {

Valve::Valve(ValveType type, uint8_t enable, uint8_t in1, uint8_t in2)
    : type(type), enable(enable), in1(in1), in2(in2) {}

void Valve::setup() {
    /* pin setup */
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enable, OUTPUT);
}

void Valve::open() {
    Serial.printf("open delay %u us pwm %u\n", MOTOR_VALVE_DELAY_US,
                  MOTOR_PWM_VALUE);

    if (type == SOLENOID) {
        /* corta salida */
        analogWrite(enable, 0);
        /* cambia voltaje */
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        /* activa salida */
        for (uint16_t value = 0; value < MOTOR_PWM_VALUE; ++value) {
            /* activa salida */
            analogWrite(enable, value);
            delayMicroseconds(MOTOR_VALVE_DELAY_US);
        }
        /* corta salida */
        analogWrite(enable, 0);
    } else {
        /* corta salida */
        analogWrite(enable, 0);
        /* activa salida */
        for (uint16_t value = 0; value < MOTOR_PWM_VALUE; ++value) {
            /* activa salida */
            analogWrite(enable, value);
            delayMicroseconds(MOTOR_VALVE_DELAY_US);
        }
    }
}

void Valve::close() {
    Serial.printf("close delay %u us pwm %u\n", MOTOR_VALVE_DELAY_US,
                  MOTOR_PWM_VALUE);
    if (type == SOLENOID) {
        /* corta salida */
        digitalWrite(enable, LOW);
        /* cambia voltaje */
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        /* activa salida */
        for (uint16_t value = 0; value < MOTOR_PWM_VALUE; ++value) {
            /* activa salida */
            analogWrite(enable, value);
            delayMicroseconds(MOTOR_VALVE_DELAY_US);
        }
        /* corta salida */
        analogWrite(enable, 0);
    } else {
        /* corta salida */
        analogWrite(enable, 0);
    }
}

} // namespace util::valve
