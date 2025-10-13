#include "time_menu.hpp"
#include "context.h"
#include "main_menu.hpp"
#include "status_menu.hpp"
#include <cmath>

#include <fonts/casio_fx_702p6pt7b.h>
#include <fonts/casio_fx_702p8pt7b.h>

/* types */
enum TimeMenuFields {
    TM_HOUR_FIELD,
    TM_MINUTE_FIELD,
    TM_SECOND_FIELD,
    TM_DAY_FIELD,
    TM_MONTH_FIELD,
    TM_YEAR_FIELD,
    TM_SAVE_FIELD,
    TM_FIELDS
};

/* variable */
static char current_time[10] = {0};
static riego::state::TimeMenu *self = nullptr;
/* current field */
static TimeMenuFields currentField = TM_HOUR_FIELD;
/* current time */
uint16_t year = 0;
uint8_t month = 0;
uint8_t dayOfMonth = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;

namespace riego::state {

TimeMenu::TimeMenu() : State(StateEnum::TimeMenu) { self = this; }

TimeMenu::~TimeMenu() {}

void TimeMenu::enter() { activateNormalMode(); }

void TimeMenu::run() {}

void TimeMenu::exit() {}

void TimeMenu::refresh() {
    /* get current time */
    RtcDateTime now;

    customLcd.clear();
    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
    /* title */
    customLcd.text(20, 20, "CHANGE TIME");

    /* external clock */
    if (customClock.getTime(now)) {
        /* set current time */
        hour = now.Hour();
        minute = now.Minute();
        second = now.Second();
        dayOfMonth = now.Day();
        month = now.Month();
        year = now.Year();

        /* print time information */
        sprintf(current_time, "%.2u:%.2u:%.2u", now.Hour(), now.Minute(),
                now.Second());
        customLcd.text(50, 60, current_time);
        sprintf(current_time, "%.2u/%.2u/%.4u", now.Day(), now.Month(),
                now.Year());
        customLcd.text(40, 110, current_time);
    } else { /* internal if there is no other */
        customLcd.text(50, 60, "NO external clock");
    }
    /* SAVE */
    customLcd.text(80, 195, "SAVE");

    customLcd.refresh();
}

void TimeMenu::refreshEdition() {}

void TimeMenu::normalNextMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::MainMenu());
    }
}

void TimeMenu::normalPrevMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::StatusMenu());
    }
}

void TimeMenu::normalActivateEdition() {
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->editPrevField);
    cruceta.setLeftCallback(self->editNextField);
    cruceta.setUpCallback(self->editIncrData);
    cruceta.setDownCallback(self->editDecData);
    cruceta.setEnterCallback(self->editSaveData);
    cruceta.setEnterContPressCallback(self->activateNormalMode);

    self->setMenuState(MenuStateEnum::Edit);
    customLcd.refresh(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false, false);
    /* partial refresh and power down */
    self->refreshFields(true);
}

void TimeMenu::editNextField() {
    self->refreshFields(false, false);
    currentField = (currentField > TM_HOUR_FIELD)
                       ? (TimeMenuFields)((int)currentField - 1)
                       : (TimeMenuFields)((int)TM_FIELDS - 1);
    self->refreshFields(true, false);
}

void TimeMenu::editPrevField() {
    self->refreshFields(false, false);
    currentField = (currentField < (TM_FIELDS - 1))
                       ? (TimeMenuFields)((int)currentField + 1)
                       : (TimeMenuFields)0;
    self->refreshFields(true, false);
}

void TimeMenu::editIncrData() {
    updateField(1);
    if (currentField != TM_SAVE_FIELD) {
        refreshFields(true, false);
    }
}

void TimeMenu::editDecData() {
    updateField(-1);
    if (currentField != TM_SAVE_FIELD) {
        refreshFields(true, false);
    }
}

void TimeMenu::editSaveData() {
    struct tm now_tm;

    if (currentField == TM_SAVE_FIELD) {
        /* change hour */
        RtcDateTime now(year, month, dayOfMonth, hour, minute, second);
        if (customClock.setTime(now)) {
            util::clock::CustomClock::setInternalDateTime(now);
            util::clock::CustomClock::printInternalDateTime();
        }

        customLcd.getDisplay()->drawLine(80, 193, 170, 180, GxEPD_WHITE);
        customLcd.refresh();
    }
}

void TimeMenu::activateNormalMode() {
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->normalNextMenu);
    cruceta.setLeftCallback(self->normalPrevMenu);
    cruceta.setEnterContPressCallback(self->normalActivateEdition);

    self->setMenuState(MenuStateEnum::Normal);
    currentField = TM_HOUR_FIELD;
    self->refresh();
}

void TimeMenu::refreshFields(bool invertBackground, bool powerOff) {
    uint16_t background = (invertBackground) ? GxEPD_BLACK : GxEPD_WHITE;
    uint16_t foreground = (invertBackground) ? GxEPD_WHITE : GxEPD_BLACK;

    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);

    switch (currentField) {
    case TM_HOUR_FIELD:
        customLcd.fillRectangle(50, 40, 28, 20, background);
        sprintf(current_time, "%.2u", hour);
        customLcd.text(50, 60, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case TM_MINUTE_FIELD:
        customLcd.fillRectangle(90, 40, 28, 20, background);
        sprintf(current_time, "%.2u", minute);
        customLcd.text(90, 60, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case TM_SECOND_FIELD:
        customLcd.fillRectangle(128, 40, 28, 20, background);
        sprintf(current_time, "%.2u", second);
        customLcd.text(129, 60, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case TM_DAY_FIELD:
        customLcd.fillRectangle(40, 90, 28, 20, background);
        sprintf(current_time, "%.2u", dayOfMonth);
        customLcd.text(40, 110, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case TM_MONTH_FIELD:
        customLcd.fillRectangle(80, 90, 28, 20, background);
        sprintf(current_time, "%.2u", month);
        customLcd.text(80, 110, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    case TM_YEAR_FIELD:
        customLcd.fillRectangle(119, 90, 56, 20, background);
        sprintf(current_time, "%.2u", year);
        customLcd.text(119, 110, current_time, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);

        break;

    case TM_SAVE_FIELD:
        customLcd.fillRectangle(80, 165, 52, 33, background);
        customLcd.text(80, 195, "SAVE", foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;

    default:
        break;
    }
}

void TimeMenu::updateField(int8_t incrDecr) {
    switch (currentField) {
    case TM_HOUR_FIELD:
        hour = mod8((hour + incrDecr), 24U);
    case TM_MINUTE_FIELD:
        minute = mod8((minute + incrDecr), 60U);
        break;

    case TM_SECOND_FIELD:
        second = mod8((second + incrDecr), 60U);
        break;

    case TM_DAY_FIELD:
        dayOfMonth = mod8((dayOfMonth + incrDecr), 32U);
        break;

    case TM_MONTH_FIELD:
        month = mod8((dayOfMonth + incrDecr), 32U);
        break;

    case TM_YEAR_FIELD:
        year = (year + incrDecr) % 32U;
        break;

    case TM_SAVE_FIELD:
        break;

    default:
        break;
    }
}

} // namespace riego::state
