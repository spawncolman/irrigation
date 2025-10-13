#ifndef SRC_STATE_PROGRAMMING_STATE
#define SRC_STATE_PROGRAMMING_STATE

#include <state/state.hpp>
#include "data/program.h"

#include <Arduino.h>

namespace riego::state {

class ProgrammingState : public State {
  public:
    ProgrammingState();

    ~ProgrammingState() {}

    virtual void enter();

    virtual void run();

    virtual void exit();

    virtual void refresh();

    virtual void refreshEdition();

    void updateEvents();

  private:

    void programULP(uint32_t us);
 
    void checkProgram(time_t now, uint8_t progInd);

    void configureEvents();

    void executeEvents();

    void rearmEvents();

    void sleepUntilNextEvent();
};

} // namespace riego::state

#endif /* SRC_STATE_PROGRAMMING_STATE */
