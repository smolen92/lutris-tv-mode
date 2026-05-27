#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "sql.h"
#include "node_category_menu.h"
#include "node_games_grid.h"
#include "node_start_menu.h"
#include "node_category_table.h"

/**
 * @brief manages all nodes
 * \todo predifined header should be loaded from settings not hardcoded
 * \todo split predefined and user defined categories into two vectors
 * \todo parameterized queries for sqlite
 * \todo control for killing process
 *
 */
class Gui_manager {
	public:
 		/**
		* \ingroup improvements
 		*
 		* check if categories vector and categories map can be replaced with std::map
		*/
		Gui_manager();
		
		~Gui_manager();
		
		/**
		 * @details only logic of the last node in vector is run
		 * \todo millis are set in global data, add process_handler to every node and rewrite the code
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
	
		/**
		 * @brief pop node
		 */
		void pop_node();

		/**
		 * @brief callback function to load key list 
		 */
		static void callback_load_key_list(void* data_vector, sqlite3_stmt* pre_statement);
		
		Renderer* renderer;
		std::vector<Node*> nodes;
		Settings* settings;
		std::vector<Game> games;
		std::vector<Category> categories;
		std::unordered_map<uint64_t, uint64_t> categories_map; //key = category_id, value = vector index
		SQL* sql;
		Global_data global_data;
		ProcessHandler process_handler;
};

#endif

