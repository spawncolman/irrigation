#include "main_menu.hpp"
#include "context.h"
#include "data/program.h"
#include "edit_menu.hpp"
#include "fonts/gato.h"
#include "programming_state.hpp"
#include "time_menu.hpp"
#include <fonts/casio_fx_702p6pt7b.h>
#include <fonts/casio_fx_702p8pt7b.h>

/* constants */
static constexpr uint8_t NUM_FIELDS = 0;

/* variable */
static char text[10] = {0};
static riego::state::MainMenu *self = nullptr;

namespace riego::state {

MainMenu::MainMenu() : State(StateEnum::MainMenu) { self = this; }

void MainMenu::enter() { activateNormalMode(); }

void MainMenu::run() {

    /* current time */
    time_t now;
    struct tm *now_tm;

    now = time(NULL);
    now_tm = localtime(&now);
}

void MainMenu::exit() {}

void MainMenu::refresh() {

    /* get current time */
    RtcDateTime now;

    customLcd.clear();

    customLcd.getDisplay()->drawExampleBitmap(epd_bitmap_gato, 0, 0, 50, 50,
                                              GxEPD_BLACK);

    /*  show time */
    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
    if (customClock.getTime(now)) {
        sprintf(text, "%.2d:%.2d:%.2d", now.Hour(), now.Minute(), now.Second());
        customLcd.text(30, 105, text);
    } else {
        customLcd.text(40, 105, "CLK ERROR");
    }

    /* battery 1 status */
    customLcd.fillRectangle(150, 30, 30, 20, GxEPD_BLACK);
    customLcd.text(138, 35, "1");

    /* battery 2 status */
    customLcd.fillRectangle(150, 70, 30, 20, GxEPD_BLACK);
    customLcd.text(138, 75, "2");

    /* selected program */
    sprintf(text, "PROG%u", currentProgram);
    customLcd.text(110, 180, text);
    customLcd.refresh();
}

void MainMenu::refreshEdition() {
    customLcd.clear();
    customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
    customLcd.text(0, 30, "Press enter");
    customLcd.text(0, 50, "  to confirm.");
    customLcd.text(0, 110, "Press enter");
    customLcd.text(0, 140, " 2 seconds to exit.");
    customLcd.refresh();
}

void MainMenu::normalNextMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::EditMenu());
    }
}

void MainMenu::normalPrevMenu() {
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::TimeMenu());
    }
}

void MainMenu::normalActivateEdition() {

    cruceta.resetCallbacks();
    cruceta.setEnterCallback(self->editActivateProgram);
    cruceta.setEnterContPressCallback(self->activateNormalMode);

    self->setMenuState(MenuStateEnum::Edit);
    self->refreshEdition();
}

void MainMenu::editActivateProgram() {

    customLcd.clear();
    customLcd.getDisplay()->setFont(&casio_fx_702p6pt7b);
    customLcd.text(30, 70, "EXECUTE PROG.");
    customLcd.refresh();

    /* start programming */
    if (stateMachine != nullptr) {
        stateMachine->next(new riego::state::ProgrammingState());
    }
}

void MainMenu::editNextProgram() {
    currentProgram =
        ((currentProgram + 1) < NUM_PROGRAMS) ? (currentProgram + 1) : 0;
    Serial.printf("program %u\n", currentProgram);

    customLcd.fillRectangle(0, 160, 200, 40, GxEPD_WHITE);
    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
    sprintf(text, "PROG%u", currentProgram);
    customLcd.text(110, 180, text);
    customLcd.refresh(0, 160, 200, 40);
}

void MainMenu::editPrevProgram() {
    currentProgram = (currentProgram > 0) ? (currentProgram - 1) : NUM_PROGRAMS;
    Serial.printf("program %u\n", currentProgram);

    customLcd.fillRectangle(0, 160, 200, 40, GxEPD_WHITE);
    customLcd.getDisplay()->setFont(&casio_fx_702p8pt7b);
    sprintf(text, "PROG%u", currentProgram);
    customLcd.text(110, 180, text);
    customLcd.refresh(0, 160, 200, 40);
}

void MainMenu::activateNormalMode() {
    cruceta.resetCallbacks();
    cruceta.setRightCallback(self->normalNextMenu);
    cruceta.setLeftCallback(self->normalPrevMenu);
    cruceta.setEnterContPressCallback(self->normalActivateEdition);

    self->setMenuState(MenuStateEnum::Normal);
    self->refresh();
}

} // namespace riego::state
