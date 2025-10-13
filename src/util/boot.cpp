#include "boot.hpp"
#include "Arduino.h"

/* constants */
static constexpr uint8_t BOOT_BUTTON_PIN = 21U;
static constexpr uint8_t CHECK_BOOT_TRIES = 100U;
static constexpr int NUM_LEDS = 1;
static constexpr unsigned char DATA_PIN = 48U;

CRGB leds[NUM_LEDS];

namespace util::boot {

Boot::Boot() {}

void Boot::setup() {
    /* led configuration */
    FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(25);

    //leds[0] = CRGB::Aquamarine;
    //FastLED.show();
    //delay(1500);
    leds[0] = CRGB::Black;
    FastLED.show();

    /* pins */
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
}

bool Boot::checkPhase(BOOT_PHASE phase) {
    int status = HIGH;
    uint8_t ind = 0U;

    while ((status == HIGH) && (ind < CHECK_BOOT_TRIES)) {
        status = digitalRead(BOOT_BUTTON_PIN);
        delay(100U);
        ++ind;
    }

    Serial.printf("phase %u = %d\n", phase, status);

    return (status == HIGH);
}

void Boot::ledOn(CRGB color) {
    leds[0] = color;
    FastLED.show();
}

void Boot::ledOff() {
    leds[0] = CRGB::Black;
    FastLED.show();
}

} // namespace util::boot
