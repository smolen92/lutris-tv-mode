#ifndef _NODE_START_MENU_H_
#define _NODE_START_MENU_H_

#include "renderer.h"

enum Entry {
	ENTRY_QUIT_TV_MODE,
	ENTRY_RESTART_SYSTEM,
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
		 * @param process_handler pointer to process handler
		 * @param renderer pointer to renderer
		 * @param settings pointer to settings
		 *
		 */
		Node_start_menu(ProcessHandler* process_handler, Renderer* renderer,Settings* settings);
		
		/// inherited from Node
		void logic(Global_data* global_data) override;
		/// inherited from Node
		void render() override;
		~Node_start_menu();
	private:
		ProcessHandler* process_handler;
		Renderer* renderer;
		Settings* settings;
		uint64_t current_entry;
		std::vector<std::string> entries_strings;
		uint64_t selection_time;
};

#endif

