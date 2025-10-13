#include "programming_state.hpp"
#include "context.h"
#include "main_menu.hpp"
#include "util/cruceta.hpp"
#include <fonts/casio_fx_702p8pt7b.h>

#include "driver/adc.h"
#include "driver/rtc_io.h"
#include "esp32s3/ulp.h"
#include "sdkconfig.h"

/* constants */
static constexpr uint64_t FROMSECTOUS = 1000000UL;
static constexpr float CURRENT_THRESHOLD = 90.0;
static constexpr float CURRENT_DIFF_THRESHOLD = 5.0;
static constexpr uint8_t PROGRAM_TEST = 0U;
static constexpr uint64_t SECONDS_TO_DAYS = 86400U;
#define RTC_GPIO_INDEX 18

/* variable */
static char text[32] = {0};
static riego::state::ProgrammingState *self = nullptr;
static uint8_t lastValve = 0;
static ValveCommand lastValveCommand = COMMAND_UNKNOWN;
static constexpr float MAXIMUM_HUMIDITY = 70.0f;
static float lastHumidity = 0.0f;

// Slow memory variable assignment
enum {
    SLOW_ADC,
    SLOW_PROG_ADDR // Program start address
};

namespace riego::state {

ProgrammingState::ProgrammingState() : State(StateEnum::ProgrammingMenu) { self = this; }

void ProgrammingState::enter() {
    /* first time, there is no events */
    if (activeEvents == 0) {
        /* update next events */
        self->updateEvents();
    }
}

void ProgrammingState::run() {
    float esp32Measure = 0.0f;
    float hbridgeMeasure = 0.0f;

    /* get current time */
    RtcDateTime now;

    if (activeEvents > 0) {
        /* clear display */
        customLcd.clear();
        customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);

        /* enable relays */
        relay_bridge.enable();
        relay_devices.enable();
        sleep(1);

        /* measure humdity */
        lastHumidity = humidity.measure(4U);

        /* check current */
        esp32Measure = currentEsp32Senson.measure(50);
        hbridgeMeasure = currentHBridgeSenson.measure(50);

         /* execute previous events */
         executeEvents();

        if (((esp32Measure > CURRENT_THRESHOLD) &&
             (hbridgeMeasure > CURRENT_THRESHOLD)) ||
            (abs(esp32Measure - hbridgeMeasure) < CURRENT_DIFF_THRESHOLD) ||
            (PROGRAM_TEST == 1U)) {
            /* do nothing */
        } else {
            customLcd.text(0, 90, "LOW BATTERY !!!");
            customLcd.text(0, 110, "CLOSE ALL");
            /* close all batteries */
            for (uint8_t valveId = 0; valveId < NUM_VALVES; ++valveId) {
                util::valve::Valve *valve = &valves[valveId];
                valve->close();
            }

            sprintf(text, "ESP32: %3.2f A", esp32Measure);
            customLcd.text(0, 150, text);
            sprintf(text, "VALV: %3.2f A", hbridgeMeasure);
            customLcd.text(0, 170, text);
        }
        /* disable relay */
        relay_devices.disable();
        relay_bridge.disable();

        /*rearm events */
        rearmEvents();

        /*  show time */
        customLcd.text(0, 30, "Last event:");
        if (customClock.getTime(now)) {
            /* date */
            sprintf(text, "%.2d-%.2d-%.4d", now.Day(), now.Month(), now.Year());
            customLcd.text(20, 50, text);
            /* time */
            sprintf(text, "%.2d:%.2d:%.2d", now.Hour(), now.Minute(),
                    now.Second());
            customLcd.text(30, 70, text);
        } else {
            customLcd.text(40, 105, "CLK ERROR");
        }
        if (lastValveCommand != COMMAND_UNKNOWN) {
            sprintf(text, "Valve %.2u cmd:%s", lastValve,
                    (lastValveCommand == VALVE_OPEN) ? "O" : "C");
            customLcd.text(0, 90, text);

            sprintf(text, "HUMD: %2.2f %s", lastHumidity,
                    (lastHumidity < MAXIMUM_HUMIDITY ? "W" : "N"));
            customLcd.text(0, 130, text);
            sprintf(text, "ESP32: %3.2f A", esp32Measure);
            customLcd.text(0, 150, text);
            sprintf(text, "VALV: %3.2f A", hbridgeMeasure);
            customLcd.text(0, 170, text);
        }

        sprintf(text, "Uptime: %.3d", ((time(0) - initTime) / SECONDS_TO_DAYS));
        customLcd.text(0, 190, text);
        customLcd.refresh();

        /* sleep */
        sleepUntilNextEvent();

    } else {
        /* start programming */
        if (stateMachine != nullptr) {
            stateMachine->next(new riego::state::MainMenu());
        }
    }
}

void ProgrammingState::exit() {}

void ProgrammingState::refresh() {}

void ProgrammingState::refreshEdition() {}

void ProgrammingState::updateEvents() {
    Serial.printf("%s number of active events %u\n", __FUNCTION__,
                  activeEvents);

    /* update events */
    configureEvents();

    /* sort events */
    events.sortEvents();

    /* print events */
    events.printEvents();
}

void ProgrammingState::programULP(uint32_t us) {
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

void ProgrammingState::checkProgram(time_t now, uint8_t progInd) {
    Serial.printf("%s prog %u\n", __FUNCTION__, progInd);
    riego::data::Program *prog = &program[progInd];
    time_t progStart =
        util::clock::CustomClock::getTimeFromProgramDate(prog->start);
    time_t progEnd =
        util::clock::CustomClock::getTimeFromProgramDate(prog->end);

    /* new event detected */
    if (((now >= progStart) && (now <= progEnd)) && (prog->status == true)) {
        riego::data::Sequence *sequence = nullptr;

        for (uint8_t seqInd = 0; seqInd < NUM_SEQUENCE; ++seqInd) {
            sequence = &prog->sequences[seqInd];
            time_t seqStart = util::clock::CustomClock::getTimeNowToValveTime(
                sequence->start);
            time_t seqEnd =
                seqStart +
                ((sequence->duration * sequence->durationMagnitude) * 60);

            /* check if sequence has duration */
            /* and is in progressing */
            if ((sequence->duration > 0) && (sequence->status == 0)) {

                /* el evento se paso, se programa segun periodo */
                if (seqStart < now) {
                    seqStart += (prog->period * 60U);
                    seqEnd += (prog->period * 60U);
                }

                /* check event limits */
                if (activeEvents < NUM_EVENTS) {

                    /* create event for start */
                    eventList[activeEvents].valveId = sequence->valveId;
                    /* start */
                    eventList[activeEvents].time = seqStart;
                    eventList[activeEvents].command = VALVE_OPEN;
                    /* program index */
                    eventList[activeEvents].progIndex = progInd;
                    /* sequence index */
                    eventList[activeEvents].sequenceIndex = seqInd;
                    /* update sequence status */
                    sequence->status |= VALVE_OPEN;
                    /* update programmed events */
                    ++activeEvents;

                    /* create event for end */
                    eventList[activeEvents].valveId = sequence->valveId;
                    /* end */
                    eventList[activeEvents].time = seqEnd;
                    eventList[activeEvents].command = VALVE_CLOSE;
                    /* program index */
                    eventList[activeEvents].progIndex = progInd;
                    /* sequence index */
                    eventList[activeEvents].sequenceIndex = seqInd;
                    /* update sequence status */
                    sequence->status |= VALVE_CLOSE;
                    /* update programmed events */
                    ++activeEvents;
                } else {
                    Serial.println("maximum number of programmed events");
                }
            }
        }
    }
}

void ProgrammingState::configureEvents() {
    riego::data::Program *prog = nullptr;
    /* current time */
    time_t now = time(0);

    for (uint8_t ind = 0; ind < NUM_PROGRAMS; ++ind) {
        checkProgram(now, ind);
    }

    time_t last = time(0);
    Serial.printf("%s Number of events %u - time %lu seconds \n", __FUNCTION__,
                  activeEvents, (last - now));
}

void ProgrammingState::executeEvents() {
    uint8_t eventInd = 0;
    Event *event = &eventList[eventInd];
    time_t now = time(0);

    /* deshabilita las baterias porque provoca mal funcionamiento */
    /* debe de estar en normally close */
    relay_espBattery.enable();
    relay_bridgeBattery.enable();
    sleep(1);

    /* recorre la lista ordenada hasta que encuentra un evento futuro */
    while (event->time <= now) {
        util::valve::Valve *valve = &valves[event->valveId];

        /* save last action */
        lastValve = event->valveId;
        lastValveCommand = event->command;

        /* execute event program */
        if (event->command == VALVE_OPEN) {
            Serial.printf("open valve %u\n", event->valveId);
            if (lastHumidity > MAXIMUM_HUMIDITY) {
                valve->open();
            }
            /* update sequence status */
            program[event->progIndex].sequences[event->sequenceIndex].status &=
                ~(VALVE_OPEN);
            Serial.printf("programStatus %u\n",
                          programStatus[event->progIndex]);

        } else {
            Serial.printf("close valve %u\n", event->valveId);
            valve->close();
            /* update sequence status */
            program[event->progIndex].sequences[event->sequenceIndex].status &=
                ~(VALVE_CLOSE);
            Serial.printf("programStatus %u\n",
                          programStatus[event->progIndex]);
        }

        /* increment event index */
        ++eventInd;
        /* decrease active events */
        --activeEvents;
        /* next event */
        event = &eventList[eventInd];
    }

    /* discard executed events */
    if (eventInd > 0) {
        uint8_t ind1 = eventInd;
        for (uint8_t ind0 = 0; ind0 < activeEvents; ++ind0) {
            memcpy((void *)&eventList[ind0], (void *)&eventList[ind1++],
                   sizeof(Event));
        }
    }
    Serial.printf("%s number of active events %u\n", __FUNCTION__,
                  activeEvents);

    relay_espBattery.disable();
    relay_bridgeBattery.disable();
    
}

void ProgrammingState::rearmEvents() {

    /* current time */
    time_t now = time(0);

    for (uint8_t progInd = 0; progInd < NUM_PROGRAMS; ++progInd) {
        checkProgram(now, progInd);
    }

    /* sort events */
    events.sortEvents();

    Serial.printf("%s number of active events %u\n", __FUNCTION__,
                  activeEvents);

    /* print events */
    events.printEvents();
}

void ProgrammingState::sleepUntilNextEvent() {
    Event *event = &eventList[0];
    uint64_t nowUs = (uint64_t)time(0) * FROMSECTOUS;
    uint64_t eventTimeUs = ((uint64_t)event->time * FROMSECTOUS);

    /* next event */
    if (eventTimeUs > nowUs) {
        Serial.printf("Sleeping %llu microseconds\n", (eventTimeUs - nowUs));

        /* set current state */
        currentState = StateEnum::ProgrammingMenu;

        /* configure ESP32 to sleep*/
        esp_sleep_enable_timer_wakeup((eventTimeUs - nowUs));

#if 0
        /* configure GPIOs as wake-up source */
        gpio_wakeup_enable(GPIO_NUM_45, GPIO_INTR_ANYEDGE);

        // Enable GPIO wake-up source
        esp_err_t result = esp_sleep_enable_gpio_wakeup();

        if (result == ESP_OK) {
            Serial.println("GPIO Wake-Up set successfully.");
        } else {
            Serial.println("Failed to set GPIO Wake-Up as wake-up source.");
        }
#endif

        /* LCD power down */
        customLcd.getDisplay()->powerDown();

        /* TODO: ULP programming */
        // Execute ULP program at 300ms intervals
        // programULP(300000);

        // esp_sleep_enable_ulp_wakeup();

        /* go to deep sleep */
        esp_deep_sleep_start();
    } else {
        Serial.println("Programming time error");
    }
}

} // namespace riego::state
