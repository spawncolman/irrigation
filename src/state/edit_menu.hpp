
#ifndef STATE_EDIT_MENU
#define STATE_EDIT_MENU

#include <cstdint>
#include <state/state.hpp>

/**
 * @file edit_menu.hpp
 * @brief EditMenu state for irrigation system. Handles editing of program and sequence parameters.
 */

namespace riego::state {

/**
 * @class EditMenu
 * @brief State for editing irrigation program and sequence parameters.
 *
 * This state allows the user to navigate and edit the fields of a program or sequence,
 * including start/end dates, period, valve ID, activation time, duration, and status.
 * It provides both normal and edition modes, with static callbacks for UI navigation.
 */
class EditMenu : public State {
public:
    /**
     * @brief Constructor.
     */
    EditMenu();

    /**
     * @brief Destructor.
     */
    ~EditMenu() override;

    /**
     * @brief Called when entering the EditMenu state.
     */
    void enter() override;

    /**
     * @brief Main execution method for EditMenu state.
     */
    void run() override;

    /**
     * @brief Called when exiting the EditMenu state.
     */
    void exit() override;

    /**
     * @brief Refreshes the EditMenu display or logic.
     */
    void refresh() override;

    /**
     * @brief Refreshes the edition mode display or logic.
     */
    void refreshEdition() override;

    /**
     * @brief Navigates to the next menu (static callback).
     */
    static void normalNextMenu();

    /**
     * @brief Navigates to the previous menu (static callback).
     */
    static void normalPrevMenu();

    /**
     * @brief Activates edition mode (static callback).
     */
    static void normalActivateEdition();

    /**
     * @brief Moves to the next editable field (static callback).
     */
    static void editNextField();

    /**
     * @brief Moves to the previous editable field (static callback).
     */
    static void editPrevField();

    /**
     * @brief Increments the value of the current field (static callback).
     */
    static void editIncrData();

    /**
     * @brief Decrements the value of the current field (static callback).
     */
    static void editDecData();

    /**
     * @brief Saves the current data (static callback).
     */
    static void editSaveData();

    /**
     * @brief Returns to normal mode (static callback).
     */
    static void activateNormalMode();

private:
    /**
     * @brief Refreshes the display of editable fields.
     * @param invertBackground Whether to invert the background color.
     * @param powerOff Whether to power off the display after refresh (default: true).
     */
    static void refreshFields(bool invertBackground, bool powerOff = true);

    /**
     * @brief Updates the value of the current field.
     * @param incrDecr Increment or decrement value.
     */
    static void updateField(int8_t incrDecr);
};

} // namespace riego::state

#endif /* STATE_EDIT_MENU */
