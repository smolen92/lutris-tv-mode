#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "sql.h"
#include "node_category_menu.h"
#include "node_games_grid.h"

/**
 * @brief manages all nodes
 * \todo predifined header should be loaded from settings not hardcoded
 * \todo system shutdown and restart - call with process handler
 * \todo move process handler from node_gamer_grid to manager and add action to global_data
 */
class Gui_manager {
	public:
		Gui_manager();
		
		~Gui_manager();
		
		/**
		 * @details only logic of the last node in vector is run
		 */
		bool logic();
		
		/**
		 * @details will render all nodes in vector
		 */
		void render();
		
	private:
		
		/**
		 * @brief helper function for loading images for games
		 */
		void load_images();

		Renderer* renderer;
		std::vector<Node*> nodes;
		Settings* settings;
		std::vector<Game> games;
		std::vector<Category> categories;
		SQL* sql;
		Global_data global_data;
};

#endif

