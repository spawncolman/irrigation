#ifndef SRC_STATE_STATUS_MENU
#define SRC_STATE_STATUS_MENU

#include <cstdint>
#include <state/state.hpp>

namespace riego::state {

class StatusMenu : public State {
  public:
    StatusMenu();

    ~StatusMenu();

    virtual void enter();

    virtual void run();

    virtual void exit();

    virtual void refresh();

    virtual void refreshEdition();

    static void normalNextMenu();

    static void normalPrevMenu();

    static void normalActivateEdition();

    static void editNextField();

    static void editPrevField();

    static void editIncrData();

    static void editDecData();

    static void openCloseValve();

    static void activateNormalMode();

  private:
    static void refreshFields(bool invertBackground, bool powerOff = true);

    static void updateField(int8_t incrDecr);
};

} // namespace riego::state

#endif /* SRC_STATE_STATUS_MENU */
