#ifndef UTIL_HMI_CRUCETA_HPP
#define UTIL_HMI_CRUCETA_HPP
#include <cstdint>

/**
 * @brief cruceta button enumerator
 *
 */
enum CrucetaButton : uint8_t {
    UP_BUTTON = 1,
    DOWN_BUTTON = 2,
    RIGHT_BUTTON = 4,
    LEFT_BUTTON = 8,
    ENTER_BUTTON = 16
};

/**
 * @brief cruceta callback.
 *
 */
typedef void (*CrucetaCallback)();

namespace util::hmi {

class Cruceta {
  public:
    Cruceta();

    /**
     * @brief Cruceta setup.
     *
     */
    void setup();

    /**
     * @brief Set the Up Callback object
     *
     * @param callback
     */
    void setUpCallback(CrucetaCallback callback);

    /**
     * @brief Set the Down Callback object
     *
     * @param callback
     */
    void setDownCallback(CrucetaCallback callback);

    /**
     * @brief Set the Right Callback object
     *
     * @param callback
     */
    void setRightCallback(CrucetaCallback callback);

    /**
     * @brief Set the Left Callback object
     *
     * @param callback
     */
    void setLeftCallback(CrucetaCallback callback);

    /**
     * @brief Set the Enter Callback object
     *
     * @param callback
     */
    void setEnterCallback(CrucetaCallback callback);

    /**
     * @brief Set the Enter Cont Press Callback object
     *
     * @param callback
     */
    void setEnterContPressCallback(CrucetaCallback callback);

    /**
     * @brief reset all callbacks.
     * 
     */
    void resetCallbacks();
    
    /**
     * @brief button events.
     *
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
     * @brief Get the Pressed Button object
     *
     * @return CrucetaButton
     */
    uint8_t getPressedButton();

    /**
     * @brief Check pressed button n milliseconds.
     *
     * @param button Button to check
     * @param milliseconds time to check in milliseconds
     * @return true
     * @return false
     */
    bool checkPressedButton(uint8_t button, uint16_t milliseconds);
};

} // namespace util::hmi

#endif // UTIL_HMI_CRUCETA_HPP
