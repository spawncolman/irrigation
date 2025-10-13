#ifndef RIEGO_STATE_TIMEMENU_HPP
#define RIEGO_STATE_TIMEMENU_HPP

#include <state/state.hpp>
#include <cstdint>

namespace riego::state {

class TimeMenu : public State {
  public:
    TimeMenu();

    ~TimeMenu();

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

    static void editSaveData();

    static void activateNormalMode();

  private:
    static void refreshFields(bool invertBackground, bool powerOff = true);

    static void updateField(int8_t incrDecr);
};

} // namespace riego::state

#endif // RIEGO_STATE_TIMEMENU_HPP
