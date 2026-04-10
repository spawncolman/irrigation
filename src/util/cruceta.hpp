#ifndef UTIL_HMI_CRUCETA_HPP
#define UTIL_HMI_CRUCETA_HPP
#include <cstdint>

/**
 * @file cruceta.hpp
 * @brief Simple directional pad (cruceta) input helper for embedded UI.
 *
 * This header declares a tiny, C++-friendly wrapper around a set of
 * GPIO buttons that behave like a directional pad: up, down, left, right
 * and an enter key. It is intentionally minimal to suit embedded
 * environments (no dynamic allocation, plain function callbacks).
 */

/**
 * @brief Bitmask values for the directional pad buttons.
 *
 * These values are intended to be OR'ed together so multiple buttons
 * can be reported at once by @c Cruceta::getPressedButton(). Use
 * equality or bitwise tests to examine the returned mask.
 */
enum CrucetaButton : uint8_t {
    UP_BUTTON = 1U,   /**< Up direction pressed */
    DOWN_BUTTON = 2U, /**< Down direction pressed */
    RIGHT_BUTTON = 4U,/**< Right direction pressed */
    LEFT_BUTTON = 8U, /**< Left direction pressed */
    ENTER_BUTTON = 16U/**< Enter (select) pressed */
};

/**
 * @brief Callback type for button events.
 *
 * A Cruceta callback is a simple function pointer with no parameters
 * and no return value. Users should keep callbacks lightweight and
 * non-blocking because they are invoked from the main loop/task.
 */
typedef void (*CrucetaCallback)();

namespace util::hmi {

/**
 * @brief Small helper wrapping a 5-button directional pad.
 *
 * Responsibilities:
 * - Configure the GPIO pins as inputs with pull-ups via @c setup().
 * - Provide simple, function-pointer based callback registration for
 *   each button event (short press and long press on enter).
 * - Read the buttons and invoke callbacks from @c events().
 *
 * Notes:
 * - Button logic expects active-low wiring (button pulls pin to GND).
 * - Callbacks run in the context where @c events() is called; avoid
 *   long blocking operations in the callbacks.
 */
class Cruceta {
  public:
    /**
     * @brief Construct an empty Cruceta with no callbacks registered.
     */
    Cruceta();

    /**
     * @brief Configure the GPIO pins.
     *
     * Should be called once during system initialization.
     */
    void setup();

    /** Register a callback invoked on short UP presses. */
    void setUpCallback(CrucetaCallback callback);

    /** Register a callback invoked on short DOWN presses. */
    void setDownCallback(CrucetaCallback callback);

    /** Register a callback invoked on short RIGHT presses. */
    void setRightCallback(CrucetaCallback callback);

    /** Register a callback invoked on short LEFT presses. */
    void setLeftCallback(CrucetaCallback callback);

    /** Register a callback invoked on short ENTER presses. */
    void setEnterCallback(CrucetaCallback callback);

    /**
     * @brief Register a callback invoked when ENTER is held long.
     *
     * The long-press detection uses a simple blocking delay in the
     * implementation; callbacks should be brief to keep the UI
     * responsive.
     */
    void setEnterContPressCallback(CrucetaCallback callback);

    /**
     * @brief Clear all registered callbacks.
     */
    void resetCallbacks();

    /**
     * @brief Read the button states and invoke any registered callbacks.
     *
     * This method is intended to be called frequently from the main
     * loop to poll the buttons and dispatch events. It is non-blocking
     * except for the enter long-press check which contains a short
     * delay.
     */
    void events();

  private:
    CrucetaCallback upCallback;
    CrucetaCallback downCallback;
    CrucetaCallback rightCallback;
    CrucetaCallback leftCallback;
    CrucetaCallback enterCallback;
    CrucetaCallback enterContPressCallback;

    /**
     * @brief Sample the GPIO pins and return a bitmask of pressed buttons.
     *
     * @return uint8_t Bitmask using @c CrucetaButton values.
     */
    uint8_t getPressedButton();

    /**
     * @brief Check if a specific button remains pressed for the given time.
     *
     * This is a simple helper used to detect long-press for the enter
     * button. The implementation uses a millisecond delay loop; it is
     * intentionally simple and portable for small embedded systems.
     *
     * @param button Index of the button in the internal button table.
     * @param milliseconds Duration to poll the button state in ms.
     * @return true if the button is still pressed after the duration.
     * @return false otherwise.
     */
    bool checkPressedButton(uint8_t button, uint16_t milliseconds);
};

} // namespace util::hmi

#endif // UTIL_HMI_CRUCETA_HPP
