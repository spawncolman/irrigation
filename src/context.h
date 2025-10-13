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

extern uint64_t startTimestampUs;
extern util::boot::Boot boot;
extern util::clock::CustomClock customClock;
extern util::valve::Valve valves[NUM_VALVES];
extern util::sensor::CurrentSensor currentEsp32Senson;
extern util::sensor::CurrentSensor currentHBridgeSenson;
extern riego::lcd::Lcd customLcd;
extern util::hmi::Cruceta cruceta;
extern util::relay::Relay relay_bridge;
extern util::relay::Relay relay_devices;
extern util::relay::Relay relay_bridgeBattery;
extern util::relay::Relay relay_espBattery;
extern util::sensor::Humidity humidity;
/* state machine */
extern riego::state::StateMachine *stateMachine;
extern StateEnum currentState;
/* program */
extern uint8_t currentProgram;
extern riego::data::Program program[NUM_PROGRAMS];
extern uint8_t programStatus[NUM_PROGRAMS];
extern time_t initTime;
/* events */
extern riego::data::Events events;
extern Event eventList[NUM_EVENTS];
extern uint8_t activeEvents;

#endif /* SRC_CONTEXT */
