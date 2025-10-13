
/**
 * @file edit_menu.cpp
 * @brief Implements the EditMenu state for editing irrigation program and sequence parameters.
 */

#include "edit_menu.hpp"
#include "context.h"
#include "main_menu.hpp"
#include "status_menu.hpp"
#include <fonts/casio_fx_702p6pt7b.h>
#include <fonts/casio_fx_702p8pt7b.h>



// --- Constants for magic numbers ---
static constexpr uint16_t MIN_YEAR = 2025U;
static constexpr uint8_t MAX_DAY = 31U;
static constexpr uint8_t MAX_MONTH = 12U;
static constexpr uint8_t MAX_HOUR = 24U;
static constexpr uint8_t MAX_MINUTE = 60U;
static constexpr uint8_t MAX_DURATION = 60U;
static constexpr uint8_t PROGRAM_RECT_X = 102U;
static constexpr uint8_t PROGRAM_RECT_Y = 0U;
static constexpr uint8_t PROGRAM_RECT_W = 20U;
static constexpr uint8_t PROGRAM_RECT_H = 33U;
static constexpr uint8_t PROGRAM_TEXT_X = 105U;
static constexpr uint8_t PROGRAM_TEXT_Y = 30U;
static constexpr uint8_t START_DAY_RECT_X = 62U;
static constexpr uint8_t START_DAY_RECT_Y = 33U;
static constexpr uint8_t START_DAY_RECT_W = 20U;
static constexpr uint8_t START_DAY_RECT_H = 25U;
static constexpr uint8_t START_DAY_TEXT_X = 62U;
static constexpr uint8_t START_DAY_TEXT_Y = 50U;
static constexpr uint8_t START_MONTH_RECT_X = 92U;
static constexpr uint8_t START_MONTH_RECT_Y = 33U;
static constexpr uint8_t START_MONTH_RECT_W = 20U;
static constexpr uint8_t START_MONTH_RECT_H = 25U;
static constexpr uint8_t START_MONTH_TEXT_X = 93U;
static constexpr uint8_t START_MONTH_TEXT_Y = 50U;
static constexpr uint8_t START_YEAR_RECT_X = 122U;
static constexpr uint8_t START_YEAR_RECT_Y = 33U;
static constexpr uint8_t START_YEAR_RECT_W = 44U;
static constexpr uint8_t START_YEAR_RECT_H = 25U;
static constexpr uint8_t START_YEAR_TEXT_X = 124U;
static constexpr uint8_t START_YEAR_TEXT_Y = 50U;
static constexpr uint8_t END_DAY_RECT_X = 62U;
static constexpr uint8_t END_DAY_RECT_Y = 53U;
static constexpr uint8_t END_DAY_RECT_W = 20U;
static constexpr uint8_t END_DAY_RECT_H = 25U;
static constexpr uint8_t END_DAY_TEXT_X = 62U;
static constexpr uint8_t END_DAY_TEXT_Y = 70U;
static constexpr uint8_t END_MONTH_RECT_X = 92U;
static constexpr uint8_t END_MONTH_RECT_Y = 53U;
static constexpr uint8_t END_MONTH_RECT_W = 20U;
static constexpr uint8_t END_MONTH_RECT_H = 25U;
static constexpr uint8_t END_MONTH_TEXT_X = 93U;
static constexpr uint8_t END_MONTH_TEXT_Y = 70U;
static constexpr uint8_t END_YEAR_RECT_X = 122U;
static constexpr uint8_t END_YEAR_RECT_Y = 53U;
static constexpr uint8_t END_YEAR_RECT_W = 44U;
static constexpr uint8_t END_YEAR_RECT_H = 25U;
static constexpr uint8_t END_YEAR_TEXT_X = 123U;
static constexpr uint8_t END_YEAR_TEXT_Y = 70U;
static constexpr uint8_t PERIOD_RECT_X = 82U;
static constexpr uint8_t PERIOD_RECT_Y = 78U;
static constexpr uint8_t PERIOD_RECT_W = 100U;
static constexpr uint8_t PERIOD_RECT_H = 25U;
static constexpr uint8_t PERIOD_TEXT_X = 83U;
static constexpr uint8_t PERIOD_TEXT_Y = 95U;
static constexpr uint8_t SEQUENCE_RECT_X = 95U;
static constexpr uint8_t SEQUENCE_RECT_Y = 98U;
static constexpr uint8_t SEQUENCE_RECT_W = 25U;
static constexpr uint8_t SEQUENCE_RECT_H = 25U;
static constexpr uint8_t SEQUENCE_TEXT_X = 96U;
static constexpr uint8_t SEQUENCE_TEXT_Y = 115U;
static constexpr uint8_t VALVE_ID_RECT_X = 114U;
static constexpr uint8_t VALVE_ID_RECT_Y = 118U;
static constexpr uint8_t VALVE_ID_RECT_W = 12U;
static constexpr uint8_t VALVE_ID_RECT_H = 25U;
static constexpr uint8_t VALVE_ID_TEXT_X = 115U;
static constexpr uint8_t VALVE_ID_TEXT_Y = 135U;
static constexpr uint8_t ACTIVE_HOUR_RECT_X = 113U;
static constexpr uint8_t ACTIVE_HOUR_RECT_Y = 138U;
static constexpr uint8_t ACTIVE_HOUR_RECT_W = 20U;
static constexpr uint8_t ACTIVE_HOUR_RECT_H = 25U;
static constexpr uint8_t ACTIVE_HOUR_TEXT_X = 114U;
static constexpr uint8_t ACTIVE_HOUR_TEXT_Y = 155U;
static constexpr uint8_t ACTIVE_MIN_RECT_X = 143U;
static constexpr uint8_t ACTIVE_MIN_RECT_Y = 138U;
static constexpr uint8_t ACTIVE_MIN_RECT_W = 20U;
static constexpr uint8_t ACTIVE_MIN_RECT_H = 25U;
static constexpr uint8_t ACTIVE_MIN_TEXT_X = 144U;
static constexpr uint8_t ACTIVE_MIN_TEXT_Y = 155U;
static constexpr uint8_t DURATION_RECT_X = 113U;
static constexpr uint8_t DURATION_RECT_Y = 158U;
static constexpr uint8_t DURATION_RECT_W = 24U;
static constexpr uint8_t DURATION_RECT_H = 25U;
static constexpr uint8_t DURATION_TEXT_X = 114U;
static constexpr uint8_t DURATION_TEXT_Y = 175U;
static constexpr uint8_t DURATION_MGN_RECT_X = 143U;
static constexpr uint8_t DURATION_MGN_RECT_Y = 158U;
static constexpr uint8_t DURATION_MGN_RECT_W = 51U;
static constexpr uint8_t DURATION_MGN_RECT_H = 25U;
static constexpr uint8_t DURATION_MGN_TEXT_X = 145U;
static constexpr uint8_t DURATION_MGN_TEXT_Y = 175U;
static constexpr uint8_t ONOFF_RECT_X = 79U;
static constexpr uint8_t ONOFF_RECT_Y = 178U;
static constexpr uint8_t ONOFF_RECT_W = 30U;
static constexpr uint8_t ONOFF_RECT_H = 33U;
static constexpr uint8_t ONOFF_TEXT_X = 80U;
static constexpr uint8_t ONOFF_TEXT_Y = 195U;
static constexpr uint8_t SAVE_RECT_X = 120U;
static constexpr uint8_t SAVE_RECT_Y = 178U;
static constexpr uint8_t SAVE_RECT_W = 52U;
static constexpr uint8_t SAVE_RECT_H = 33U;
static constexpr uint8_t SAVE_TEXT_X = 120U;
static constexpr uint8_t SAVE_TEXT_Y = 195U;
static constexpr uint8_t SAVE_FEEDBACK_TEXT_X = 130U;
static constexpr uint8_t SAVE_FEEDBACK_TEXT_Y = 170U;
static constexpr uint8_t SAVE_FEEDBACK_LINE_X1 = 130U;
static constexpr uint8_t SAVE_FEEDBACK_LINE_Y = 180U;
static constexpr uint8_t SAVE_FEEDBACK_LINE_X2 = 170U;


/**
 * @brief Editable fields in the EditMenu state.
 */
enum StatusMenuFields {
    EM_PROG_FIELD,
    EM_START_DAY_FIELD,
    EN_START_MONTH_FIELD,
    EM_START_YEAR_FIELD,
    EM_END_DAY_FIELD,
    EM_END_MONTH_FIELD,
    EM_END_YEAR_FIELD,
    EM_PERIOD_FIELD,

    EM_SEQUENCE_FIELD,
    EM_VALVE_ID_FIELD,
    EM_VALVE_ACTIVE_HOUR_FIELD,
    EM_VALVE_ACTIVE_MINUTE_FIELD,
    EM_DURATION_FIELD,
    EM_DURATION_MAGNITUDE_FIELD,

    EM_ON_OFF_FIELD,
    EM_SAVE_FIELD,
    EM_FIELDS
};


// Buffer for text rendering
static char text[32] = {0};
// Pointer to the current EditMenu instance
static riego::state::EditMenu *self = nullptr;
// Current selected valve and sequence
static uint8_t currentValve = 0;
static uint8_t currentSequence = 0;
// Duration configuration
static constexpr uint8_t NUM_DURATIONS = 2U;
static constexpr char *durationMgns[NUM_DURATIONS] = {"minut", "hours"};
static uint8_t currentDurationMgn = 0;
// Period configuration
static constexpr uint8_t NUM_PERIODS = 6U;
static constexpr uint8_t periods[NUM_PERIODS] = {12U, 1U, 2U, 3U, 4U, 5U};
static constexpr char *periodsMgn[NUM_PERIODS] = {"hours", "days", "days", "days",  "days", "days"};
static uint8_t currentPeriod = 0;
// Currently selected field for editing
static StatusMenuFields currentField = EM_PROG_FIELD;


namespace riego::state {


/**
 * @brief Constructor. Sets the static self pointer.
 */
EditMenu::EditMenu() : State(StateEnum::StatusMenu) { self = this; }


/**
 * @brief Destructor.
 */
EditMenu::~EditMenu() {}

/**
 * @brief Called when entering the EditMenu state. Initializes period and duration magnitude.
 */
void EditMenu::enter() {
    riego::data::Program *prog = &program[currentProgram];
    riego::data::Sequence *sequence = &prog->sequences[currentSequence];

    /* current period */
    currentPeriod = (prog->period == 720) ? 0 : prog->period / 1440U;
    /* current magnitude */
    currentDurationMgn = (sequence->durationMagnitude == 1) ? 0 : 1;

    activateNormalMode();
}

void EditMenu::run() {}

void EditMenu::exit() {}

/**
 * @brief Refreshes the EditMenu display with current program and sequence data.
 */
void EditMenu::refresh() {
    riego::data::Program *prog = &program[currentProgram];
    riego::data::Sequence *sequence = &prog->sequences[currentSequence];

    customLcd.clear();
    /* program number */
    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
    sprintf(text, "PROGRAM %u", currentProgram);
    customLcd.text(0, 30, text);
    /* start date */
    customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
    sprintf(text, "start:%.2u/%.2u/%.2u", prog->start.day, prog->start.month,
            prog->start.year);
    customLcd.text(0, 50, text);
    /* end date */
    sprintf(text, "end:  %.2u/%.2u/%.2u", prog->end.day, prog->end.month,
            prog->end.year);
    customLcd.text(0, 70, text);
    /* period */
    sprintf(text, "period: %.2u %s", periods[currentPeriod],
            periodsMgn[currentPeriod]);
    customLcd.text(0, 95, text);

    /* sequence */
    sprintf(text, "Sequence %.2u/%.2u", currentSequence, NUM_SEQUENCE);
    customLcd.text(0, 115, text);
    /* valve identifier */
    sprintf(text, "Valve id: %u/%u", currentValve, NUM_VALVES);
    customLcd.text(10, 135, text);
    /* active triggers */
    sprintf(text, "Active:   %.2u:%.2u", sequence->start.hour,
            sequence->start.minute);
    customLcd.text(10, 155, text);
    /* duration */
    sprintf(text, "Duration: %.2u %s", sequence->duration,
            durationMgns[currentDurationMgn]);
    customLcd.text(10, 175, text);
    /* (ON/OFF) save */
    sprintf(text, "%s SAVE", (prog->status) ? "ON " : "OFF");
    customLcd.text(80, 195, text);

    customLcd.refresh();
}

void EditMenu::refreshEdition() {}

/**
 * @brief Navigates to the next menu (StatusMenu).
 */
void EditMenu::normalNextMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::StatusMenu());
    }
}

/**
 * @brief Navigates to the previous menu (MainMenu).
 */
void EditMenu::normalPrevMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::MainMenu());
    }
}

/**
 * @brief Activates edition mode and sets up callbacks for field editing.
 */
void EditMenu::normalActivateEdition() {
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

/**
 * @brief Moves to the previous editable field.
 */

void EditMenu::editPrevField() {
    self->refreshFields(false, false);
    currentField = static_cast<StatusMenuFields>((static_cast<int>(currentField) + 1) % EM_FIELDS);
    self->refreshFields(true, false);
}

/**
 * @brief Moves to the next editable field.
 */

void EditMenu::editNextField() {
    self->refreshFields(false, false);
    currentField = static_cast<StatusMenuFields>((static_cast<int>(currentField) - 1 + EM_FIELDS) % EM_FIELDS);
    self->refreshFields(true, false);
}

/**
 * @brief Increments the value of the current field.
 */

void EditMenu::editIncrData() {
    updateField(1);
    // Solo refrescar si el campo realmente cambia visualmente
    if (currentField != EM_PROG_FIELD) {
        refreshFields(true, false);
    }
}

/**
 * @brief Decrements the value of the current field.
 */

void EditMenu::editDecData() {
    updateField(-1);
    if (currentField != EM_PROG_FIELD) {
        refreshFields(true, false);
    }
}

/**
 * @brief Saves the current data if the save field is selected.
 */
void EditMenu::editSaveData() {
    if (currentField == EM_SAVE_FIELD) {
        if (riego::data::Program::saveSystemProgram(program)) {
            customLcd.text(130, 170, "SAVE");
            customLcd.getDisplay()->drawLine(130, 180, 170, 180, GxEPD_WHITE);
        }
        customLcd.refresh();
    }
}

/**
 * @brief Returns to normal mode and sets up navigation callbacks.
 */
void EditMenu::activateNormalMode() {
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->normalNextMenu);
    cruceta.setLeftCallback(self->normalPrevMenu);
    cruceta.setEnterContPressCallback(self->normalActivateEdition);

    self->setMenuState(MenuStateEnum::Normal);
    currentField = EM_PROG_FIELD;
    self->refresh();
}

/**
 * @brief Refreshes the display of the currently selected field.
 * @param invertBackground Whether to invert the background color.
 * @param powerOff Whether to power off the display after refresh.
 */
void EditMenu::refreshFields(bool invertBackground, bool powerOff) {

    riego::data::Program *prog = &program[currentProgram];
    riego::data::Sequence *sequence = &prog->sequences[currentSequence];
    uint16_t background = (invertBackground) ? GxEPD_BLACK : GxEPD_WHITE;
    uint16_t foreground = (invertBackground) ? GxEPD_WHITE : GxEPD_BLACK;

    switch (currentField) {
    case EM_PROG_FIELD:
        customLcd.fillRectangle(PROGRAM_RECT_X, PROGRAM_RECT_Y, PROGRAM_RECT_W, PROGRAM_RECT_H, background);
        sprintf(text, "%u", currentProgram);
        customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
        customLcd.text(PROGRAM_TEXT_X, PROGRAM_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_START_DAY_FIELD:
        customLcd.fillRectangle(START_DAY_RECT_X, START_DAY_RECT_Y, START_DAY_RECT_W, START_DAY_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->start.day);
        customLcd.text(START_DAY_TEXT_X, START_DAY_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EN_START_MONTH_FIELD:
        customLcd.fillRectangle(START_MONTH_RECT_X, START_MONTH_RECT_Y, START_MONTH_RECT_W, START_MONTH_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->start.month);
        customLcd.text(START_MONTH_TEXT_X, START_MONTH_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_START_YEAR_FIELD:
        customLcd.fillRectangle(START_YEAR_RECT_X, START_YEAR_RECT_Y, START_YEAR_RECT_W, START_YEAR_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->start.year);
        customLcd.text(START_YEAR_TEXT_X, START_YEAR_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_END_DAY_FIELD:
        customLcd.fillRectangle(END_DAY_RECT_X, END_DAY_RECT_Y, END_DAY_RECT_W, END_DAY_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->end.day);
        customLcd.text(END_DAY_TEXT_X, END_DAY_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_END_MONTH_FIELD:
        customLcd.fillRectangle(END_MONTH_RECT_X, END_MONTH_RECT_Y, END_MONTH_RECT_W, END_MONTH_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->end.month);
        customLcd.text(END_MONTH_TEXT_X, END_MONTH_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_END_YEAR_FIELD:
        customLcd.fillRectangle(END_YEAR_RECT_X, END_YEAR_RECT_Y, END_YEAR_RECT_W, END_YEAR_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", prog->end.year);
        customLcd.text(END_YEAR_TEXT_X, END_YEAR_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_PERIOD_FIELD:
        customLcd.fillRectangle(PERIOD_RECT_X, PERIOD_RECT_Y, PERIOD_RECT_W, PERIOD_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u %s", periods[currentPeriod], periodsMgn[currentPeriod]);
        customLcd.text(PERIOD_TEXT_X, PERIOD_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_SEQUENCE_FIELD:
        customLcd.fillRectangle(SEQUENCE_RECT_X, SEQUENCE_RECT_Y, SEQUENCE_RECT_W, SEQUENCE_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", currentSequence);
        customLcd.text(SEQUENCE_TEXT_X, SEQUENCE_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_VALVE_ID_FIELD:
        customLcd.fillRectangle(VALVE_ID_RECT_X, VALVE_ID_RECT_Y, VALVE_ID_RECT_W, VALVE_ID_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%u", currentValve);
        customLcd.text(VALVE_ID_TEXT_X, VALVE_ID_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_VALVE_ACTIVE_HOUR_FIELD:
        customLcd.fillRectangle(ACTIVE_HOUR_RECT_X, ACTIVE_HOUR_RECT_Y, ACTIVE_HOUR_RECT_W, ACTIVE_HOUR_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", sequence->start.hour);
        customLcd.text(ACTIVE_HOUR_TEXT_X, ACTIVE_HOUR_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_VALVE_ACTIVE_MINUTE_FIELD:
        customLcd.fillRectangle(ACTIVE_MIN_RECT_X, ACTIVE_MIN_RECT_Y, ACTIVE_MIN_RECT_W, ACTIVE_MIN_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", sequence->start.minute);
        customLcd.text(ACTIVE_MIN_TEXT_X, ACTIVE_MIN_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_DURATION_FIELD:
        customLcd.fillRectangle(DURATION_RECT_X, DURATION_RECT_Y, DURATION_RECT_W, DURATION_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%.2u", sequence->duration);
        customLcd.text(DURATION_TEXT_X, DURATION_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_DURATION_MAGNITUDE_FIELD:
        customLcd.fillRectangle(DURATION_MGN_RECT_X, DURATION_MGN_RECT_Y, DURATION_MGN_RECT_W, DURATION_MGN_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        sprintf(text, "%s", durationMgns[currentDurationMgn]);
        customLcd.text(DURATION_MGN_TEXT_X, DURATION_MGN_TEXT_Y, text, foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_ON_OFF_FIELD:
        customLcd.fillRectangle(ONOFF_RECT_X, ONOFF_RECT_Y, ONOFF_RECT_W, ONOFF_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        customLcd.text(ONOFF_TEXT_X, ONOFF_TEXT_Y, (prog->status) ? "ON" : "OFF", foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    case EM_SAVE_FIELD:
        customLcd.fillRectangle(SAVE_RECT_X, SAVE_RECT_Y, SAVE_RECT_W, SAVE_RECT_H, background);
        customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
        customLcd.text(SAVE_TEXT_X, SAVE_TEXT_Y, "SAVE", foreground);
        customLcd.refresh(0, 0, 200, 200, true, powerOff);
        break;
    default:
        break;
    }
}

/**
 * @brief Updates the value of the currently selected field.
 * @param incrDecr Increment or decrement value.
 */

void EditMenu::updateField(int8_t incrDecr) {
    auto& prog = program[currentProgram];
    auto& sequence = prog.sequences[currentSequence];

    switch (currentField) {
    case EM_PROG_FIELD:
        currentProgram = mod8((currentProgram + incrDecr), NUM_PROGRAMS);
        currentValve = 0;
        currentSequence = 0;
        currentPeriod = (program[currentProgram].period == 720) ? 0 : program[currentProgram].period / 1440U;
        currentDurationMgn = (program[currentProgram].sequences[0].durationMagnitude == 1) ? 0 : 1;
        self->refresh();
        break;
    case EM_START_DAY_FIELD:
        prog.start.day = mod8((prog.start.day + incrDecr), MAX_DAY);
        break;
    case EN_START_MONTH_FIELD:
        prog.start.month = mod8((prog.start.month + incrDecr), MAX_MONTH);
        break;
    case EM_START_YEAR_FIELD:
        prog.start.year = std::max<uint16_t>(MIN_YEAR, prog.start.year + incrDecr);
        break;
    case EM_END_DAY_FIELD:
        prog.end.day = mod8((prog.end.day + incrDecr), MAX_DAY);
        break;
    case EM_END_MONTH_FIELD:
        prog.end.month = mod8((prog.end.month + incrDecr), MAX_MONTH);
        break;
    case EM_END_YEAR_FIELD:
        prog.end.year = std::max<uint16_t>(MIN_YEAR, prog.end.year + incrDecr);
        break;
    case EM_PERIOD_FIELD:
        currentPeriod = mod8((currentPeriod + incrDecr), NUM_PERIODS);
        prog.period = (periods[currentPeriod] == 12U) ? periods[currentPeriod] * 60U : periods[currentPeriod] * 1440U;
        break;
    case EM_SEQUENCE_FIELD:
        currentSequence = mod8((currentSequence + incrDecr), NUM_SEQUENCE);
        currentDurationMgn = (prog.sequences[currentSequence].durationMagnitude == 1) ? 0 : 1;
        self->refresh();
        break;
    case EM_VALVE_ID_FIELD:
        currentValve = mod8((currentValve + incrDecr), NUM_VALVES);
        sequence.valveId = currentValve;
        break;
    case EM_VALVE_ACTIVE_HOUR_FIELD:
        sequence.start.hour = mod8((sequence.start.hour + incrDecr), MAX_HOUR);
        break;
    case EM_VALVE_ACTIVE_MINUTE_FIELD:
        sequence.start.minute = mod8((sequence.start.minute + incrDecr), MAX_MINUTE);
        break;
    case EM_DURATION_FIELD:
        sequence.duration = mod8((sequence.duration + incrDecr), MAX_DURATION);
        break;
    case EM_DURATION_MAGNITUDE_FIELD:
        currentDurationMgn = mod8((currentDurationMgn + incrDecr), NUM_DURATIONS);
        sequence.durationMagnitude = (currentDurationMgn == 0) ? 1 : 60U;
        break;
    case EM_ON_OFF_FIELD:
        prog.status = !prog.status;
        break;
    case EM_SAVE_FIELD:
        break;
    default:
        break;
    }
}

} // namespace riego::state
