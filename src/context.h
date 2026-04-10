#ifndef SRC_CONTEXT
#define SRC_CONTEXT

#include "data/program.h"
#include "data/event.hpp"
#include "lcd/lcd.hpp"
#include "state/state_machine.hpp"
#include "util/boot.hpp"
#include "util/clock.hpp"
#include "util/cruceta.hpp"
#include "util/current_sensor.hpp"
#include "util/valve.hpp"
#include "util/relay.hpp"
#include "util/humidity.hpp"

/**
 * @file context.h
 * @brief Global application context declarations.
 *
 * This header exposes the global objects used across the firmware to
 * simplify access from different modules. The objects are defined in
 * a single translation unit (usually `main.cpp`) and declared here as
 * `extern` so other files can reference them.
 *
 * Notes:
 * - Use of globals is intentional in this embedded project to keep
 *   resource ownership simple. Prefer passing references when adding
 *   new components.
 */

/** Timestamp (microseconds) when system started. */
extern uint64_t startTimestampUs;

/** Boot configuration and helpers. */
extern util::boot::Boot boot;

/** System clock (RTC) wrapper. */
extern util::clock::CustomClock customClock;

/** Array of valves controlled by the system. Size defined by NUM_VALVES. */
extern util::valve::Valve valves[NUM_VALVES];

/** Current sensor reading for the ESP32 power rail (or similar). */
extern util::sensor::CurrentSensor currentEsp32Senson;

/** Current sensor reading for the H-bridge / motor driver. */
extern util::sensor::CurrentSensor currentHBridgeSenson;

/** Main LCD display instance. */
extern riego::lcd::Lcd customLcd;

/** Directional pad (cruceta) input helper. */
extern util::hmi::Cruceta cruceta;

/** Relay used to switch the motor bridge. */
extern util::relay::Relay relay_bridge;

/** Relay used to power devices. */
extern util::relay::Relay relay_devices;

/** Relay controlling bridge battery power. */
extern util::relay::Relay relay_bridgeBattery;

/** Relay controlling ESP battery power. */
extern util::relay::Relay relay_espBattery;

/** Humidity sensor wrapper. */
extern util::sensor::Humidity humidity;

/* State machine */
/** Pointer to the global state machine instance. */
extern riego::state::StateMachine *stateMachine;

/** Current UI state enum. */
extern StateEnum currentState;

/* Program management */
/** Currently selected program index. */
extern uint8_t currentProgram;

/** Array of saved programs. Size defined by NUM_PROGRAMS. */
extern riego::data::Program program[NUM_PROGRAMS];

/** Status flags for each program. */
extern uint8_t programStatus[NUM_PROGRAMS];

/** Initialization time used by timers and schedules. */
extern time_t initTime;

/* Events */
/** Events manager instance. */
extern riego::data::Events events;

/** Global event list. Size defined by NUM_EVENTS. */
extern Event eventList[NUM_EVENTS];

/** Number of active events currently scheduled. */
extern uint8_t activeEvents;

#endif /* SRC_CONTEXT */
