#include "status_menu.hpp"
#include "context.h"
#include "edit_menu.hpp"
#include "time_menu.hpp"

// FreeFonts from Adafruit_GFX
#include <fonts/casio_fx_702p6pt7b.h>
#include <fonts/casio_fx_702p8pt7b.h>

/* constants */

/* types */
enum StatusMenuFields {
    SM_VALVE_FIELD,
    SM_OPEN_VALVE_FIELD,
    SM_CLOSE_VALVE_FIELD,
    SM_FIELDS
};

/* variable */
static char current_time[10] = {0};
static riego::state::StatusMenu *self = nullptr;
static uint8_t currentValve = 0;
static StatusMenuFields currentField = SM_VALVE_FIELD;
static char text[64] = {0};

namespace riego::state {

StatusMenu::StatusMenu() : State(StateEnum::StatusMenu) { self = this; }

StatusMenu::~StatusMenu() {}

void StatusMenu::enter() { activateNormalMode(); }

void StatusMenu::run() {}

void StatusMenu::exit() {}

void StatusMenu::refresh() {
    /* get current time */
    RtcDateTime now;
    /* current time */
    time_t intNow;
    struct tm *now_tm;

    intNow = time(NULL);
    now_tm = localtime(&intNow);

    /* enable sensors */
    relay_devices.enable();

    customLcd.clear();
    /* internal clock */
    customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
    customLcd.text(0, 30, "INT.CLK:");
    sprintf(current_time, "%.2d:%.2d:%.2d", now_tm->tm_hour, now_tm->tm_min,
            now_tm->tm_sec);
    customLcd.text(100, 30, current_time);

    /* external clock */
    customLcd.text(0, 50, "EXT.CLK:");
    if (customClock.getTime(now)) {
        sprintf(current_time, "%.2d:%.2d:%.2d", now.Hour(), now.Minute(),
                now.Second());
        customLcd.text(100, 50, current_time);
    } else {
        customLcd.text(100, 50, "--:--:--");
    }

    /* batery 1 */
    sprintf(text, "BAT 1: %10.2f", currentEsp32Senson.measure(10));
    customLcd.text(0, 70, text);

    /* batery 2 */
    sprintf(text, "BAT 2: %10.2f", currentHBridgeSenson.measure(10));
    customLcd.text(0, 90, text);

    /* testing */
    customLcd.text(0, 130, "TESTING:");

    sprintf(text, "Valve %u/%u", currentValve, NUM_VALVES);
    customLcd.text(10, 150, text);

    customLcd.text(10, 170, "OPEN / CLOSE");

    customLcd.refresh();

    /* disable sensors */
    relay_devices.disable();
}

void StatusMenu::refreshEdition() {}

void StatusMenu::normalNextMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::TimeMenu());
    }
}

void StatusMenu::normalPrevMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::EditMenu());
    }
}

void StatusMenu::normalActivateEdition() {
    Serial.println("Edit mode activated");
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->editPrevField);
    cruceta.setLeftCallback(self->editNextField);
    cruceta.setUpCallback(self->editIncrData);
    cruceta.setDownCallback(self->editDecData);
    cruceta.setEnterCallback(self->openCloseValve);
    cruceta.setEnterContPressCallback(self->activateNormalMode);

    self->setMenuState(MenuStateEnum::Edit);
    customLcd.refresh(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false, false);
    /* partial refresh and power down */
    self->refreshFields(true);
}

void StatusMenu::editNextField() {
    self->refreshFields(false, false);
    currentField = (currentField > SM_VALVE_FIELD)
                       ? (StatusMenuFields)((int)currentField - 1)
                       : (StatusMenuFields)((int)SM_FIELDS - 1);
    self->refreshFields(true, false);
}

void StatusMenu::editPrevField() {
    self->refreshFields(false, false);
    currentField = (currentField < (SM_FIELDS - 1))
                       ? (StatusMenuFields)((int)currentField + 1)
                       : (StatusMenuFields)0;
    self->refreshFields(true, false);
}

void StatusMenu::editIncrData() {
    updateField(1);
    if (currentField == SM_VALVE_FIELD) {
        refreshFields(true, false);
    }
}

void StatusMenu::editDecData() {
    updateField(1);
    if (currentField == SM_VALVE_FIELD) {
        refreshFields(true, false);
    }
}

void StatusMenu::openCloseValve() {
    util::valve::Valve *valve = &valves[currentValve];
    Serial.println("openCloseValve!!!");
    relay_bridge.enable();
    relay_devices.enable();
    Serial.println("disable battery!!!");
    relay_espBattery.enable();
    relay_bridgeBattery.enable();
    sleep(1);

    if (currentField == SM_OPEN_VALVE_FIELD) {
        Serial.println("active relay");

        Serial.println("valve open!!!");
        customLcd.getDisplay()->drawLine(82, 180, 132, 180, GxEPD_WHITE);
        valve->open();
        customLcd.getDisplay()->drawLine(10, 180, 60, 180, GxEPD_BLACK);
        customLcd.refresh(0, 0, 200, 200, true, false);
       
    } else if (currentField == SM_CLOSE_VALVE_FIELD) {
        Serial.println("active relay");
        Serial.println("valve close!!!");
        customLcd.getDisplay()->drawLine(10, 180, 60, 180, GxEPD_WHITE);
        valve->close();
        customLcd.getDisplay()->drawLine(82, 180, 132, 180, GxEPD_BLACK);
        customLcd.refresh(0, 0, 200, 200, true, false);
    }
    relay_espBattery.disable();
    relay_bridgeBattery.disable();
    relay_devices.disable();
    relay_bridge.disable();
}

void StatusMenu::activateNormalMode() {
    Serial.println("Normal mode activated");
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->normalNextMenu);
    cruceta.setLeftCallback(self->normalPrevMenu);
    cruceta.setEnterContPressCallback(self->normalActivateEdition);

    self->setMenuState(MenuStateEnum::Normal);
    self->refresh();
}

void StatusMenu::refreshFields(bool invertBackground, bool powerOff) {
    riego::data::Program *prog = &program[currentProgram];
    uint16_t background = (invertBackground) ? GxEPD_BLACK : GxEPD_WHITE;
    uint16_t foreground = (invertBackground) ? GxEPD_WHITE : GxEPD_BLACK;
    Serial.printf("refreshFields %u\n", powerOff);

    switch (currentField) {
    case SM_VALVE_FIELD:
        customLcd.fillRectangle(73, 137, 10, 15, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%u", currentValve);
        customLcd.text(73, 150, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case SM_OPEN_VALVE_FIELD:
        customLcd.fillRectangle(9, 158, 50, 20, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        customLcd.text(10, 170, "OPEN", foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case SM_CLOSE_VALVE_FIELD:
        customLcd.fillRectangle(80, 158, 55, 17, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        customLcd.text(82, 170, "CLOSE", foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    default:
        break;
    }
}

void StatusMenu::updateField(int8_t incrDecr) {
    riego::data::Program *prog = &program[currentProgram];
   //riego::data::Valve *valve = &prog->valves[currentValve];

    switch (currentField) {
    case SM_VALVE_FIELD:
        currentValve = mod8((currentValve + incrDecr), NUM_VALVES);
        self->refresh();
        break;

    case SM_OPEN_VALVE_FIELD:
        /* nothing to do */
        break;

    case SM_CLOSE_VALVE_FIELD:
        /* nothing to do*/
        break;

    default:
        /* do nothing */
        break;
    }
}

} // namespace riego::state
