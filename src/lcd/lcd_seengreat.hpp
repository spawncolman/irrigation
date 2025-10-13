#ifndef LCD_DISPLAY_LCD_HPP
#define LCD_DISPLAY_LCD_HPP
#if 0
#include "lcd/fonts.h"
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "epd_gui.h"
#ifdef __cplusplus
}
#endif

namespace lcd::display {

class CustomLcd {
  public:
    CustomLcd();

    /**
     * @brief custom LCD setup.
     * 
     */
    void setup();

    /**
     * @brief Custom LCD demo.
     * 
     */
    void demo();

    /**
     * @brief reset display.
     * 
     */
    void begin();

    /**
     * @brief Draw text in the custom LCD.
     * 
     * @param x 
     * @param y 
     * @param text 
     */
    void text(uint8_t x, uint8_t y, const char *text, FONT *font, uint16_t color = BLACK);

    /**
     * @brief Draw text in the custom LCD.
     * 
     * @param x 
     * @param y 
     * @param text 
     */
    void fillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color = BLACK);

    /**
     * @brief refresh display.
     * 
     */
    void end();

  private:
};

} // namespace lcd::display
#endif
#endif // LCD_DISPLAY_LCD_HPP
