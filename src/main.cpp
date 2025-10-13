#include "context.h"
#include "data/event.hpp"
#include "data/program.h"
#include "pinout.h"
#include "state/edit_menu.hpp"
#include "state/main_menu.hpp"
#include "state/programming_state.hpp"
#include "state/status_menu.hpp"
#include "state/time_menu.hpp"
#include "util/relay.hpp"
#include "util/humidity.hpp"

#include <Arduino.h>

#include "driver/adc.h"
#include "driver/rtc_io.h"
#include "esp32s3/ulp.h"
#include "sdkconfig.h"

/* constants */
static constexpr uint8_t RELAY_CH1_BUTTON_PIN = 36U;
#define RTC_GPIO_INDEX 18

/* devices */
util::boot::Boot boot;
util::clock::CustomClock customClock;
util::valve::Valve valve1(SOLENOID, MOTOR_ENABLE_A_PIN, MOTOR_IN1_PIN,
                          MOTOR_IN2_PIN);
util::valve::Valve valve2(SOLENOID, MOTOR_ENABLE_B_PIN, MOTOR_IN3_PIN,
                          MOTOR_IN4_PIN);
util::sensor::CurrentSensor currentEsp32Senson(CURRENT_SENSOR_ESP32_PIN);
util::sensor::CurrentSensor currentHBridgeSenson(CURRENT_SENSOR_H_BRIDGE_PIN);

riego::lcd::Lcd customLcd;
util::hmi::Cruceta cruceta;
util::relay::Relay relay_bridge(H_BRIDGE_RELAY_PIN);
util::relay::Relay relay_devices(DEVICES_RELAY_PIN);
util::relay::Relay relay_bridgeBattery(BRIDGE_BATTERY_RELAY_PIN);
util::relay::Relay relay_espBattery(ESP_BATTERY_RELAY_PIN);
/* available valves */
util::valve::Valve valves[NUM_VALVES] = {valve1, valve2};
/* humidity sensor */
util::sensor::Humidity humidity(HUMIDITY_PIN);

/* state machine */
riego::state::StateMachine *stateMachine = nullptr;

/* static variables */
uint64_t startTimestampUs = 0;

/* RTC variables */
RTC_DATA_ATTR riego::data::Program program[NUM_PROGRAMS];
RTC_DATA_ATTR StateEnum currentState = StateEnum::MainMenu;
RTC_DATA_ATTR uint8_t currentProgram = 0;
RTC_DATA_ATTR Event eventList[NUM_EVENTS];
RTC_DATA_ATTR uint8_t activeEvents = 0;
RTC_DATA_ATTR riego::data::Events events;
RTC_DATA_ATTR uint8_t programStatus[NUM_PROGRAMS] = {0};
RTC_DATA_ATTR time_t initTime = 0;

// put function declarations here:
static void checkWakeUpReason(esp_sleep_wakeup_cause_t wakeupReason);

static void deepSleep();

#define FINAL_IMPLEMENTATION
//#define LCD

/* implementation */
void checkWakeUpReason(esp_sleep_wakeup_cause_t wakeupReason) {
    RtcDateTime now;

    Serial.printf("wakeup %u\n", wakeupReason);

    switch (wakeupReason) {
    case ESP_SLEEP_WAKEUP_TIMER: // ESP Internal RTC
        /* clock adjustment */
        if (customClock.getTime(now)) {
            /* set internal clock */
            customClock.setInternalDateTime(now);
        }
        break;
    case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
        break;
    case ESP_SLEEP_WAKEUP_GPIO: // GPIOs
        Serial.println("GPIOs wakeup");
        /* go to main menu */
        currentState = StateEnum::MainMenu;
        break;
    case ESP_SLEEP_WAKEUP_EXT1: // button Press
        break;
    default: // reset
        /* clock adjustment */
        if (customClock.getTime(now)) {
            /* set internal clock */
            customClock.setInternalDateTime(now);
        } else {
            /* warning activated */
            // boot.ledOn(CRGB::Red);
        }

        /* close all valves */
        relay_espBattery.enable();
        relay_bridgeBattery.enable();
        sleep(1);
        relay_bridge.enable();
        for (uint8_t valveId = 0; valveId < NUM_VALVES; ++valveId) {
            util::valve::Valve *valve = &valves[valveId];
            valve->close();
        }
        relay_bridge.disable();
        relay_espBattery.disable();
        relay_bridgeBattery.disable();

        /* read system program */
        if (riego::data::Program::readSystemProgram(program)) {
            // riego::data::Program::printProgram(program);
        }

        /* set initial time */
        initTime = time(0);
        break;
    }

    // riego::data::Program::printProgram(program);
}

void createMenu() {
    switch (currentState) {
    case StateEnum::MainMenu:
        stateMachine =
            new riego::state::StateMachine(new riego::state::MainMenu());
        break;

    case StateEnum::EditMenu:
        stateMachine =
            new riego::state::StateMachine(new riego::state::EditMenu());
        break;

    case StateEnum::StatusMenu:
        stateMachine =
            new riego::state::StateMachine(new riego::state::StatusMenu());
        break;

    case StateEnum::TimeMenu:
        stateMachine =
            new riego::state::StateMachine(new riego::state::TimeMenu());
        break;

    case StateEnum::ProgrammingMenu:
        stateMachine = new riego::state::StateMachine(
            new riego::state::ProgrammingState());
        break;

    default:
        stateMachine =
            new riego::state::StateMachine(new riego::state::MainMenu());
        break;
    }
}

void setup() {
    /* start timestamp */
    startTimestampUs = micros();
    /* debugging */
    Serial.begin(115200);
    /* boot setup */
    boot.setup();
    /* clock setup */
    customClock.setup();
    /* valve1 setup */
    valve1.setup();
    /* valve2 setup */
    valve2.setup();
    /* current sensor setup */
    currentEsp32Senson.setup();
    /* lcd setup */
    customLcd.setup();
    // gxepd_example_setup();
    /* cruceta setup */
    cruceta.setup();
    /* relay 1*/
    relay_bridge.setup();
    /* relay 2 */
    relay_devices.setup();
    /* relay 3 */
    relay_bridgeBattery.setup();
    /* relay 4 */
    relay_espBattery.setup();


    /* humidity */
    humidity.setup();

    /* get wake up reason */
    esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();

    /* check reason */
    checkWakeUpReason(wakeupReason);

    /* state machine initialization */
    createMenu();

    Serial.printf("Setup size %u\n", sizeof(program));
}

void loop() {

#ifdef RTC
    RtcDateTime now;
    /* clock */
    if (customClock.getTime(now)) {
        customClock.printDateTime(now);
    } else {
        Serial.println("No clock, critical error\n");
    }
#endif

#ifdef RELAY
    digitalWrite(RELAY_CH1_BUTTON_PIN, HIGH);
    Serial.println("HIGH");
    delay(5000U);
    digitalWrite(RELAY_CH1_BUTTON_PIN, LOW);
    Serial.println("LOW");
    delay(5000U);
#endif

#ifdef SENSOR_CORRIENTE
    // read current sensor
    Serial.printf("Intensidad %f A\n", currentSensonr.measure(200));

#endif

#ifdef CRUCETA
    uint8_t button = cruceta.getPressedButton();
    Serial.printf("cruceta button 0x%x\n", button);
    if ((button & UP_BUTTON) > 0) {
        Serial.printf("cruceta pressed result 0x%d\n",
                      cruceta.checkPressedButton(UP_BUTTON, 2000U));
    }
#endif

#ifdef LCD
    gxepd_example_loop();
#endif

#ifdef LED
    /*
    delay(5000U);
    Serial.println("Open valve");
    boot.ledOn(CRGB::Green);
    valve.open();
    boot.ledOff();
    delay(10000U);
    Serial.println("Close valve");
    boot.ledOn(CRGB::Red);
    valve.close();
    boot.ledOff();
    */
#endif

#ifdef FINAL_IMPLEMENTATION

    stateMachine->getCurrent()->run();

    cruceta.events();

    usleep(100U);
#endif
}

static void programULP(uint32_t us) {
    // Set ULP activation interval
    ulp_set_wakeup_period(0, us);
    // ULP Program
    const ulp_insn_t ulp_prog[] = {
        // Read GPIO18 state (bit 28) via RTC_GPIO_IN_REG
        I_RD_REG(RTC_GPIO_IN_REG, RTC_GPIO_INDEX + RTC_GPIO_IN_NEXT_S,
                 RTC_GPIO_INDEX + RTC_GPIO_IN_NEXT_S),
        I_WAKE(), I_END(),
        I_HALT() // Stop the program
    };
    // Run the program shifted backward by the number of variables
    size_t size = sizeof(ulp_prog) / sizeof(ulp_insn_t);
    Serial.printf("program ULP with %u bytes", size);
    ulp_process_macros_and_load(0, ulp_prog, &size);
    ulp_run(0);
}

void deepSleep() {
    Serial.println("Sleep");

    /* TODO: ULP programming */
    // Execute ULP program at 300ms intervals
    programULP(300000);

    // esp_sleep_enable_ulp_wakeup();

    /* TODO: sleep lcd ??*/
#if 0
    /* wake up by GPIO */
    esp_sleep_enable_ext1_wakeup(
        BTN_PIN_MASK,
        ESP_EXT1_WAKEUP_ANY_HIGH);  // enable deep sleep wake on button press
    log_i("%6d *** sleeping after %llu.%03llums ***\n", millis(), elapsed / 1000,
          elapsed % 1000);
#endif
    uint64_t elapsed = micros() - startTimestampUs;
    esp_sleep_enable_timer_wakeup(60000000U - elapsed);
    esp_deep_sleep_start();
}