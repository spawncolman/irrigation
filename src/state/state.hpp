
#ifndef STATE_STATE_STATE_HPP
#define STATE_STATE_STATE_HPP

/**
 * @file state.hpp
 * @brief State base class and enums for irrigation system state machine.
 *
 * Provides the abstract base class for all states and enums for state management.
 */

/**
 * @brief Enumerates the possible states of the main state machine.
 */
enum class StateEnum {
  MainMenu,
  EditMenu,
  StatusMenu,
  TimeMenu,
  ProgrammingMenu
};

/**
 * @brief Enumerates the possible menu states.
 */
enum class MenuStateEnum {
  Normal,
  Edit
};

namespace riego::state {

/**
 * @brief Abstract base class for all states in the irrigation system.
 *
 * Each state must implement the lifecycle methods: enter, run, exit, refresh, and refreshEdition.
 * The class also manages the current menu state.
 */
class State {
public:
  /**
   * @brief Constructor. Initializes the state and sets menu state to Normal.
   * @param state The state identifier.
   */
  explicit State(StateEnum state)
    : state(state), menuState(MenuStateEnum::Normal) {}

  /**
   * @brief Virtual destructor.
   */
  virtual ~State() = default;

  /**
   * @brief Called when entering the state.
   */
  virtual void enter() = 0;

  /**
   * @brief Main execution method for the state.
   */
  virtual void run() = 0;

  /**
   * @brief Called when exiting the state.
   */
  virtual void exit() = 0;

  /**
   * @brief Refreshes the state display or logic.
   */
  virtual void refresh() = 0;

  /**
   * @brief Refreshes the edition mode display or logic.
   */
  virtual void refreshEdition() = 0;

  /**
   * @brief Gets the current menu state.
   * @return The current MenuStateEnum.
   */
  MenuStateEnum getMenuState() const { return menuState; }

  /**
   * @brief Sets the menu state.
   * @param menuState The new menu state.
   */
  void setMenuState(MenuStateEnum menuState) { this->menuState = menuState; }

  /**
   * @brief Gets the state identifier.
   * @return The current StateEnum.
   */
  StateEnum getStateEnum() const { return state; }

protected:
  StateEnum state;           ///< The state identifier
  MenuStateEnum menuState;   ///< The menu state
};

} // namespace riego::state

#endif // STATE_STATE_STATE_HPP
