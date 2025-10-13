#ifndef RIEGO_STATE_STATEMACHIE_HPP
#define RIEGO_STATE_STATEMACHIE_HPP

#include "state.hpp"

namespace riego::state {

class StateMachine {
 public:
	StateMachine(State *initial);

	void next(State *state);

	State *getCurrent();

 private:

	State *current;


};

} // namespace riego::state

#endif  // RIEGO_STATE_STATEMACHIE_HPP
