#include "gui_manager.h"

/// \file
/// \todo final touch-up for status bar rendering
/// \cond
int main(int argc, char** argv) {
	try {
		//gui initialization
		Gui_manager gui_manager;
		
		bool running = true;

		//Main loop
		while(running) {
			running = gui_manager.logic();

			gui_manager.render();
		}


	}
	catch (const std::runtime_error & error) {
		std::clog << "Exception: " << error.what() << "\n";
		return 1;
	}

	return 0;

}
/// \endcond

/**
 * \defgroup improvements Improvements
 *
 * low prio improvements 
 */
