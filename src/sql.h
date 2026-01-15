#ifndef _SQL_H_
#define _SQL_H_

#include <sqlite3.h> //https://www.sqlite.org/cintro.html
#include <vector>

#include "game.h"

/**
 * @brief class for managing sql
 */
class SQL {
	public:
		/**
		 * \todo throw an error when database file cannot be opened
		 *
		 * @brief open the database file
		 *
		 * @param database_path path to sqlite database file
		 */
		SQL(const char* database_path);
	
		/**
		 * @brief load info about games to game vector from database
		 *
		 * @param game_vec pointer to a vector of game
		 */
		void load_data(std::vector<Game> *game_vec);

		~SQL();

	private:
		sqlite3 *db;
		char *error_message;
		static int callback_load_data(void* data_vector, int argc, char** argv, char **az_col_name);

};

#endif

