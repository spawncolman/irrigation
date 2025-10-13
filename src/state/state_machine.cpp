#include "state_machine.hpp"

namespace riego::state {

StateMachine::StateMachine(State *initial) : current(initial) {
    initial->enter();
}

void StateMachine::next(State *state) {
    current->exit();

    state->enter();

    delete(current);

    current = state;
}

State *StateMachine::getCurrent() { return current; }

} // namespace riego::state