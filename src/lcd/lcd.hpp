#ifndef SRC_LCD_LCD
#define SRC_LCD_LCD

#include <Arduino.h>
#include <GxEPD.h>
#include <GxGDEW0154M09/GxGDEW0154M09.h> // 1.54" b/w 200x200, JD79653A
// FreeFonts from Adafruit_GFX
#include <gfxfont.h>

namespace riego::lcd {

/**
 * @brief LCD configuration constants.
 *
 * Centralize magic numbers here so they are easy to change and document.
 */
namespace config {
    // Display rotation applied on init and refresh operations (degrees).
    constexpr uint16_t DISPLAY_ROTATION = 135;

    // SPI bus frequency used for the display (Hz).
    // 4MHz is a safe default for many EPD modules; increase if your hardware supports it.
    constexpr uint32_t SPI_FREQUENCY_HZ = 4000000UL;

    // Baud rate used when initializing the display library for diagnostic output.
    constexpr uint32_t DISPLAY_INIT_BAUD = 115200UL;

    // Delay (ms) after a full update to give the panel time to finish.
    constexpr uint16_t FULL_UPDATE_DELAY_MS = 2000;

    // Delay (ms) after a partial window update.
    constexpr uint16_t PARTIAL_UPDATE_DELAY_MS = 500;
}

/**
 * @brief Lightweight wrapper around a GxEPD display used in the irrigation project.
 *
 * Responsibilities:
 *  - initialize the SPI bus and the GxEPD display object
 *  - offer simple drawing primitives (clear, text, rectangle)
 *  - provide refresh helpers for full and partial updates
 *
 * Usage:
 *  - Call setup() once during board startup
 *  - Use text()/fillRectangle() to draw into the framebuffer
 *  - Call refresh() or refresh(x,y,w,h) to update the e-paper panel
 */
class Lcd {
  public:
    Lcd();

    /**
     * @brief Configure SPI, initialize the display and apply default rotation.
     *
     * This method configures the SPI bus using the pin macros expected by the
     * project and initializes the underlying GxEPD driver. Diagnostic output
     * is enabled at DISPLAY_INIT_BAUD.
     */
    void setup();

    /**
     * @brief Clear the display to white.
     *
     * This only draws to the internal buffer. Call refresh() to push changes
     * to the panel.
     */
    void clear();

    /**
     * @brief Draw a NUL-terminated string at the given position.
     *
     * @param x Horizontal pixel coordinate (left = 0)
     * @param y Vertical pixel coordinate (top = 0)
     * @param text NUL-terminated C string
     * @param color Draw color (GxEPD_WHITE or GxEPD_BLACK)
     */
    void text(uint16_t x, uint16_t y, const char *text, uint16_t color = GxEPD_BLACK);

    /**
     * @brief Fill a rectangle in the frame buffer.
     *
     * @param x
     * @param y
     * @param text
     */
    void fillRectangle(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                       uint16_t color = GxEPD_BLACK);

    /**
     * @brief Perform a full display refresh.
     *
     * @param powerOff If true, power down the display after the update to save energy.
     */
    void refresh(bool powerOff = true);

    /**
     * @brief Perform a partial window refresh.
     *
     * @param x Left coordinate of the window
     * @param y Top coordinate of the window
     * @param w Window width
     * @param h Window height
     * @param using_rotation If true, apply the configured rotation to the update
     * @param powerOff If true, power down the display after the update
     */
    void refresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 bool using_rotation = true, bool powerOff = true);

    /**
     * @brief Access the underlying GxEPD display object.
     *
     * @return Pointer to GxEPD_Class instance used by this wrapper.
     */
    GxEPD_Class *getDisplay();

  private:
};

} // namespace riego::lcd

#endif /* SRC_LCD_LCD */
