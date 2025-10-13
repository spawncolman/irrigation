#include "lcd.hpp"
#include "lcd/seengreat.h"

#include <GxIO/GxIO.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

// Low-level IO and display instances. Pin macros (CUSTOMLCD_*) must be
// defined elsewhere in the project.
GxIO_Class lcdIo(SPI, /*CS=5*/ SS, /*DC=*/CUSTOMLCD_DC_PIN,
                 /*RST=*/CUSTOMLCD_RST_PIN);
GxEPD_Class lcdDisplay(lcdIo, /*RST=*/CUSTOMLCD_RST_PIN,
                       /*BUSY=*/CUSTOMLCD_BUSY_PIN);

namespace riego::lcd {

Lcd::Lcd() {}

void Lcd::setup() {
    // Initialize the display driver with diagnostic output enabled at the
    // configured baud rate. Use the configured SPI frequency and initialize
    // the SPI pins. The display rotation is applied once here to avoid
    // repeated calls during drawing/refresh.
    lcdDisplay.init(config::DISPLAY_INIT_BAUD);
    // Optional: disable diagnostic output by calling init(0) if desired.

    // Set SPI frequency used by the GxIO wrapper.
    lcdIo.setFrequency(config::SPI_FREQUENCY_HZ);

    // Initialize hardware SPI: (SCK, MISO/BUSY, MOSI, SS)
    SPI.begin(CUSTOMLCD_CLK_PIN, CUSTOMLCD_BUSY_PIN, CUSTOMLCD_DIN_PIN,
              CUSTOMLCD_CS_PIN);

    // Apply rotation once; subsequent draw/refresh calls don't need to set it.
    lcdDisplay.setRotation(config::DISPLAY_ROTATION);
}

void Lcd::clear() {
    // draw background
    lcdDisplay.setRotation(config::DISPLAY_ROTATION);
    lcdDisplay.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
}

void Lcd::text(uint16_t x, uint16_t y, const char *text, uint16_t color) {
    // Draw a NUL-terminated C string at (x,y).
    lcdDisplay.setTextColor(color);
    lcdDisplay.setCursor(x, y);
    lcdDisplay.print(text);
}

void Lcd::fillRectangle(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                        uint16_t color) {
    // Fill a rectangle in the frame buffer.
    lcdDisplay.fillRect(x1, y1, w, h, color);
}

void Lcd::refresh(bool powerOff) {
    lcdDisplay.setRotation(config::DISPLAY_ROTATION);
    lcdDisplay.update();
    delay(config::FULL_UPDATE_DELAY_MS);
    if (powerOff) {
        lcdDisplay.powerDown();
    }
}

void Lcd::refresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                  bool using_rotation, bool powerOff) {
    lcdDisplay.setRotation(config::DISPLAY_ROTATION);
    lcdDisplay.updateWindow(x, y, w, h, using_rotation);
    delay(config::PARTIAL_UPDATE_DELAY_MS);
    if (powerOff) {
        lcdDisplay.powerDown();
    }
}

GxEPD_Class *Lcd::getDisplay() {
    return &lcdDisplay;
}

} // namespace riego::lcd
