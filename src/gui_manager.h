#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "sql.h"
#include "node_category_menu.h"
#include "node_games_grid.h"
#include "node_start_menu.h"
#include "process.h"

/**
 * @brief manages all nodes
 * \todo predifined header should be loaded from settings not hardcoded
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
		
		/**
		 * @brief helper fucntion when changes to game vector need to be made
		 *
		 * @param sql_statement sql statement that will be used for retrieving games from database
		 */
		void load_games_vector(const char* sql_statement);

		Renderer* renderer;
		std::vector<Node*> nodes;
		Settings* settings;
		std::vector<Game> games;
		std::vector<Category> categories;
		SQL* sql;
		Global_data global_data;
		ProcessHandler process_handler;
};

#endif

