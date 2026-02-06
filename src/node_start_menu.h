#ifndef _NODE_START_MENU_H_
#define _NODE_START_MENU_H_

#include "renderer.h"

enum Entry {
	ENTRY_RESTART_SYSTEM,
	ENTRY_QUIT_TV_MODE,
	ENTRY_SHUTDOWN_SYSTEM,
	ENTRY_COUNT
};

/**
 * @brief menu that shows when start/ESC is pressed
 */
class Node_start_menu : public Node {
	public:
		/**
		 *
		 * @param renderer pointer to renderer
		 * @param settings pointer to settings
		 *
		 */
		Node_start_menu(Renderer* renderer,Settings* settings);
		
		/// inherited from Node
		void logic(Global_data* global_data) override;
		/// inherited from Node
		void render() override;
		~Node_start_menu();
	private:
		Renderer* renderer;
		Settings* settings;
		uint64_t current_entry;
		std::vector<std::string> entries_strings;
};

#endif

