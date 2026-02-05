#ifndef _NODE_START_MENU_H_
#define _NODE_START_MENU_H_

#include "renderer.h"

#define ENTRY_COUNT 2


class Node_start_menu : public Node {
	public:
		/**
		 *
		 * \ingroup improvements
		 *
		 * this whole node is hardcoded, does it make sense to load it from settings?
		 * \todo document node
		 */
		Node_start_menu(Renderer* renderer,Settings* settings);
		void logic(Global_data* global_data) override;
		void render() override;
		~Node_start_menu();
	private:
		Renderer* renderer;
		Settings* settings;
		uint64_t current_entry;
};



#endif

