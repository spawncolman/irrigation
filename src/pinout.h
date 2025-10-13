#ifndef SRC_PINOUT
#define SRC_PINOUT

#include <cstdint>

static constexpr uint8_t MOTOR_IN1_PIN = 16U;
static constexpr uint8_t MOTOR_IN2_PIN = 17U;
static constexpr uint8_t MOTOR_ENABLE_A_PIN = 18U;
static constexpr uint8_t MOTOR_IN3_PIN = 8U;
static constexpr uint8_t MOTOR_IN4_PIN = 3U;
static constexpr uint8_t MOTOR_ENABLE_B_PIN = 46U;

static constexpr uint8_t CURRENT_SENSOR_ESP32_PIN = 19U;
static constexpr uint8_t CURRENT_SENSOR_H_BRIDGE_PIN = 9U;


static constexpr uint8_t H_BRIDGE_RELAY_PIN = 41U;
static constexpr uint8_t DEVICES_RELAY_PIN = 40U;
static constexpr uint8_t BRIDGE_BATTERY_RELAY_PIN = 35U;
static constexpr uint8_t ESP_BATTERY_RELAY_PIN = 36U;

/* humidity sensor */
static constexpr uint8_t HUMIDITY_PIN = 15U;

#endif /* SRC_PINOUT */

