#include "cruceta.hpp"
#include "Arduino.h"

/* constants*/
static constexpr uint8_t CRUCETA_UP_BUTTON_PIN = 20U;
static constexpr uint8_t CRUCETA_DOWN_BUTTON_PIN = 21U;
static constexpr uint8_t CRUCETA_RIGHT_BUTTON_PIN = 47U;
static constexpr uint8_t CRUCETA_LEFT_BUTTON_PIN = 48U;
static constexpr uint8_t CRUCETA_ENTER_BUTTON_PIN = 45U;
static constexpr int TIME_THRESHOLD = 150;

static uint8_t buttons[] = {CRUCETA_UP_BUTTON_PIN, CRUCETA_DOWN_BUTTON_PIN,
                            CRUCETA_RIGHT_BUTTON_PIN, CRUCETA_LEFT_BUTTON_PIN,
                            CRUCETA_ENTER_BUTTON_PIN};

static constexpr uint16_t CRUCETA_PRESS_ENTER_TIME_MS = 1000U;

namespace util::hmi {

Cruceta::Cruceta()
    : upCallback(nullptr), downCallback(nullptr), rightCallback(nullptr),
      leftCallback(nullptr), enterCallback(nullptr),
      enterContPressCallback(nullptr) {}

void Cruceta::setup() {
    /* pins */
    pinMode(CRUCETA_UP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CRUCETA_DOWN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CRUCETA_RIGHT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CRUCETA_LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CRUCETA_ENTER_BUTTON_PIN, INPUT_PULLUP);
}

void Cruceta::setUpCallback(CrucetaCallback callback) { upCallback = callback; }

void Cruceta::setDownCallback(CrucetaCallback callback) {
    downCallback = callback;
}

void Cruceta::setRightCallback(CrucetaCallback callback) {
    rightCallback = callback;
}

void Cruceta::setLeftCallback(CrucetaCallback callback) {
    leftCallback = callback;
}

void Cruceta::setEnterCallback(CrucetaCallback callback) {
    enterCallback = callback;
}

void Cruceta::setEnterContPressCallback(CrucetaCallback callback) {
    enterContPressCallback = callback;
}

void Cruceta::resetCallbacks() {
    upCallback = nullptr;
    downCallback = nullptr;
    rightCallback = nullptr;
    leftCallback = nullptr;
    enterCallback = nullptr;
    enterContPressCallback = nullptr;
}

void Cruceta::events() {
    uint8_t status = getPressedButton();

    if ((status & UP_BUTTON) > 0) {
        if (upCallback != nullptr) {
            upCallback();
        }
    }

    if ((status & DOWN_BUTTON) > 0) {
        if (downCallback != nullptr) {
            downCallback();
        }
    }

    if ((status & RIGHT_BUTTON) > 0) {
        if (rightCallback != nullptr) {
            rightCallback();
        }
    }

    if ((status & LEFT_BUTTON) > 0) {
        if (leftCallback != nullptr) {
            leftCallback();
        }
    }

    if ((status & ENTER_BUTTON) > 0) {
        if (checkPressedButton(4U, CRUCETA_PRESS_ENTER_TIME_MS)) {
            if (enterContPressCallback != nullptr) {
                enterContPressCallback();
            }
        } else {
            if (enterCallback != nullptr) {
                enterCallback();
            }
        }
    }
}

uint8_t Cruceta::getPressedButton() {
    uint8_t status = 0;

    status |= (digitalRead(CRUCETA_UP_BUTTON_PIN) == LOW) ? UP_BUTTON : 0;
    status |= (digitalRead(CRUCETA_DOWN_BUTTON_PIN) == LOW) ? DOWN_BUTTON : 0;
    status |= (digitalRead(CRUCETA_RIGHT_BUTTON_PIN) == LOW) ? RIGHT_BUTTON : 0;
    status |= (digitalRead(CRUCETA_LEFT_BUTTON_PIN) == LOW) ? LEFT_BUTTON : 0;
    status |= (digitalRead(CRUCETA_ENTER_BUTTON_PIN) == LOW) ? ENTER_BUTTON : 0;

    return status;
}

bool Cruceta::checkPressedButton(uint8_t button, uint16_t milliseconds) {
    uint8_t pin = buttons[button];

    while (milliseconds > 0) {
        --milliseconds;
        delay(1U);
    }

    return (digitalRead(pin) == LOW);
}

} // namespace util::hmi
