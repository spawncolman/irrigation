#include "valve.hpp"
#include "Arduino.h"

/*
 * @file valve.cpp
 * @brief Minimal valve driver implementation.
 *
 * The implementation favors clarity and MISRA-friendly code: no dynamic
 * allocation, no exceptions, and no usage of the C++ standard library.
 */

/* constants */
static const uint32_t MOTOR_VALVE_DELAY_US = 2005U;
static const uint8_t MOTOR_PWM_VALUE = 250U;

namespace util::valve {

static void pwm_ramp(uint8_t enable_pin, uint8_t pwm_max, uint32_t delay_us) {
    /* Gradually increase PWM from 0 to pwm_max to softly drive motor */
    uint16_t value = 0U;
    while (value < pwm_max) {
        analogWrite(enable_pin, value);
        delayMicroseconds(delay_us);
        ++value;
    }
    /* ensure final value is 0 (stop) */
    analogWrite(enable_pin, 0);
}

Valve::Valve(ValveType type_arg, uint8_t enable_arg, uint8_t in1_arg, uint8_t in2_arg)
    : type(type_arg), enable(enable_arg), in1(in1_arg), in2(in2_arg) {
    /* basic validation: pins should not be identical */
    if ((enable == in1) || (enable == in2) || (in1 == in2)) {
        Serial.println("Valve pins invalid: duplicates detected");
        /* keep going; user must fix wiring */
    }
}

void Valve::setup() {
    /* configure pins as outputs */
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(enable, OUTPUT);
    /* ensure outputs are in a safe state */
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enable, 0);
}

void Valve::open() {
    Serial.printf("Valve::open delay=%u pwm=%u\n", (unsigned)MOTOR_VALVE_DELAY_US, (unsigned)MOTOR_PWM_VALUE);

    if (type == SOLENOID) {
        /* corta salida */
        analogWrite(enable, 0);
        /* cambia voltaje */
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        /* ramp PWM */
        pwm_ramp(enable, MOTOR_PWM_VALUE, MOTOR_VALVE_DELAY_US);
    } else {
        /* Relay/simple valve: use enable pin high for a short pulse */
        digitalWrite(enable, HIGH);
        delayMicroseconds(MOTOR_VALVE_DELAY_US);
        digitalWrite(enable, LOW);
    }
}

void Valve::close() {
    Serial.printf("Valve::close delay=%u pwm=%u\n", (unsigned)MOTOR_VALVE_DELAY_US, (unsigned)MOTOR_PWM_VALUE);

    if (type == SOLENOID) {
        /* corta salida */
        digitalWrite(enable, LOW);
        /* cambia voltaje */
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        pwm_ramp(enable, MOTOR_PWM_VALUE, MOTOR_VALVE_DELAY_US);
    } else {
        /* ensure relay is deasserted */
        digitalWrite(enable, LOW);
    }
}

} // namespace util::valve
