#ifndef SRC_STATE_MAIN_MENU
#define SRC_STATE_MAIN_MENU

#include <state/state.hpp>

namespace riego::state {

class MainMenu : public State {
 public:

	MainMenu();

	~MainMenu(){}

	virtual void enter();

    virtual void run();

    virtual void exit();

	virtual void refresh();

    virtual void refreshEdition();

	static void normalNextMenu();

    static void normalPrevMenu();

	static void normalActivateEdition();

	static void editActivateProgram();

	static void editNextProgram();

	static void editPrevProgram();

	static void activateNormalMode();

 private:

};

} // namespace riego::state

#endif /* SRC_STATE_MAIN_MENU */
