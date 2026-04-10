/**
 * @file epd_gui.h
 * @brief Simple graphics API for the EPD display used by the irrigation UI.
 *
 * This header exposes lightweight drawing primitives (points, lines, boxes,
 * text) and a minimal image container. The API is C-compatible (extern "C")
 * so it can be used from both C and C++ code.
 */

#ifndef __EPD_GUI_H
#define __EPD_GUI_H

#include "fonts.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Image attributes used by the in-memory framebuffer helpers.
 *
 * The `img` pointer references the raw pixel/bitmap buffer. Width/height are
 * the logical dimensions; mem_w/mem_h indicate the allocated buffer dimensions
 * (useful for stride/alignment). color/rotate/mirror are display attributes
 * used by the helper functions.
 */
typedef struct {
    uint8_t *img;   /**< Raw pixel buffer (device-specific format) */
    uint16_t w;     /**< Logical image width in pixels */
    uint16_t h;     /**< Logical image height in pixels */
    uint16_t mem_w; /**< Allocated buffer width in pixels (stride) */
    uint16_t mem_h; /**< Allocated buffer height in pixels */
    uint16_t color; /**< Default drawing color for the image */
    uint16_t rotate;/**< Rotation applied when drawing (degrees) */
    uint16_t mirror;/**< Mirror flags (see MIRROR_IMAGE) */
    uint16_t byte_w;/**< Width in bytes of the image buffer */
    uint16_t byte_h;/**< Height in bytes of the image buffer */
} IMAGE;
extern IMAGE Image; /**< Global current image selected by Gui_SelectImage */

/**
 * @name Display rotation constants
 * @{ */
#define ROTATE_0   0
#define ROTATE_90  90
#define ROTATE_180 180
#define ROTATE_270 270
/** @} */

/**
 * @brief Image mirror options used by the GUI helpers.
 */
typedef enum {
    MIRROR_NONE       = 0x00, /**< No mirror */
    MIRROR_HORIZONTAL = 0x01, /**< Horizontal flip */
    MIRROR_VERTICAL   = 0x02, /**< Vertical flip */
    MIRROR_ORIGIN     = 0x03, /**< Horizontal+Vertical */
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * @brief Color constants for the display driver. Values are driver-specific
 * and intended for ease of use in the UI code.
 */
#define WHITE 0xFF
#define BLACK 0x00
#define RED   BLACK

#define IMAGE_BACKGROUND WHITE
#define FONT_FOREGROUND   BLACK
#define FONT_BACKGROUND   WHITE

/**
 * @brief Pixel block sizes used to draw thicker points.
 */
typedef enum {
    PIXEL_1X1 = 1, /**< 1x1 pixel */
    PIXEL_2X2,     /**< 2x2 block */
    PIXEL_3X3,
    PIXEL_4X4,
    PIXEL_5X5,
    PIXEL_6X6,
    PIXEL_7X7,
    PIXEL_8X8,
} DOT_PIXEL;
#define DOT_PIXEL_DFT PIXEL_1X1 /**< Default pixel size */

/**
 * @brief Dot drawing styles for multi-pixel points.
 */
typedef enum {
    AROUND = 1, /**< Centered fill style */
    RIGHTUP,    /**< Top-right aligned fill */
} DOT_STYLE;
#define DOT_STYLE_DFT AROUND /**< Default dot style */

/**
 * @brief Line rendering style.
 */
typedef enum {
    SOLID = 0, /**< Continuous line */
    DOTTED,    /**< Dashed line */
} LINE_STYLE;

/**
 * @brief Fill mode for shapes.
 */
typedef enum {
    EMPTY = 0, /**< Draw outline only */
    FULL,      /**< Fill the shape */
} DRAW_FILL;

/**
 * @brief Simple date/time structure used by Gui_draw_time.
 */
typedef struct {
    uint16_t Year;  /**< Year, full (e.g. 2025) */
    uint8_t  Month; /**< Month 1-12 */
    uint8_t  Day;   /**< Day 1-31 */
    uint8_t  Hour;  /**< Hour 0-23 */
    uint8_t  Min;   /**< Minute 0-59 */
    uint8_t  Sec;   /**< Second 0-59 */
} DRAW_TIME;
extern DRAW_TIME Draw_Time;

// ------------------------------------------------------------------
// Initialization and basic operations
// ------------------------------------------------------------------
/**
 * @brief Initialize an IMAGE structure and associate a buffer.
 * @param image Pointer to buffer memory
 * @param width Image logical width in pixels
 * @param height Image logical height in pixels
 * @param rotate Initial rotation (use ROTATE_* constants)
 * @param color Default drawing color
 */
void Image_Init(uint8_t *image, uint16_t width, uint16_t height, uint16_t rotate, uint16_t color);

/**
 * @brief Select an existing buffer as the active image for subsequent draws.
 */
void Gui_SelectImage(uint8_t *image);

/**
 * @brief Set global drawing rotation (degrees).
 */
void Gui_SetRotate(uint16_t rotate);

/**
 * @brief Set global image mirror flags (use MIRROR_IMAGE values).
 */
void Gui_SetMirror(uint8_t mirror);

/**
 * @brief Set a single pixel in the active image.
 */
void Gui_SetPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Fill the whole display/image with a color.
 */
void Gui_Clear(uint16_t color);

/**
 * @brief Clear a rectangular region.
 */
void Gui_ClearWindows(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

// ------------------------------------------------------------------
// Drawing primitives
// ------------------------------------------------------------------
/**
 * @brief Draw a point/filled block at (x,y).
 */
void Gui_Draw_Point(uint16_t x, uint16_t y, uint16_t color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style);

/**
 * @brief Draw a line between two points.
 */
void Gui_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, DOT_PIXEL Dot_Pixel, LINE_STYLE style);

/**
 * @brief Draw a rectangle, optionally filled.
 */
void Gui_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, DRAW_FILL Filled , DOT_PIXEL Dot_Pixel);

/**
 * @brief Draw a circle, optionally filled.
 */
void Gui_Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, DRAW_FILL Draw_Fill , DOT_PIXEL Dot_Pixel);

// ------------------------------------------------------------------
// Text and time rendering
// ------------------------------------------------------------------
/**
 * @brief Draw a single character using the provided font.
 */
void Gui_Draw_Char(uint16_t x, uint16_t y, const char str_char, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);

/**
 * @brief Draw a null-terminated string using the provided font.
 */
void Gui_Draw_Str(uint16_t x, uint16_t y, const char *str_char, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);

/**
 * @brief Draw a signed integer number using the provided font.
 */
void Gui_Draw_Num(uint16_t x, uint16_t y, int32_t nummber, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);

/**
 * @brief Draw a time value (DRAW_TIME) using the provided font.
 */
void Gui_draw_time(uint16_t x1, uint16_t y1, DRAW_TIME *pTime, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);

// ------------------------------------------------------------------
// Bitmaps
// ------------------------------------------------------------------
/**
 * @brief Draw a bitmap buffer on the active image.
 *
 * The format of the buffer depends on the display driver.
 */
void Gui_DrawBitMap(const unsigned char* image_buffer);


#ifdef __cplusplus
}
#endif

#endif