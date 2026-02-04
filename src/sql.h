#ifndef _SQL_H_
#define _SQL_H_

#include <sqlite3.h> //https://www.sqlite.org/cintro.html
#include <vector>
#include <stdexcept>
#include <cstring>

#include "game.h"
#include "category.h"

/**
 * @brief class for managing sql
 */
class SQL {
	public:
		/**
		 *
		 * @brief open the database file
		 *
		 * @param database_path path to sqlite database file
		 */
		SQL(const char* database_path);
	
		/**
		 * @brief load info about games to game vector from database
		 *
		 * @param data pointer to a vector where data will be stored
		 * @param sql_statement sql statement that will be called
		 * @param callback_function callback function that will be called for data return from database
		 * 
		 */
		void load_data(void *data, const char* sql_statement, void (*callback_function)(void*,sqlite3_stmt* pre_statement));
		
		/**
		 * @brief callback function for loading game data from database
		 *
		 * @details more info https://www.sqlite.org/c3ref/exec.html 
		 */
		static void callback_load_games(void* data_vector, sqlite3_stmt* pre_statement);
		
		/**
		 * @brief callback fuction for loading categories data from database
		 *
		 * @details more info https://www.sqlite.org/c3ref/exec.html 
		 */
		static void callback_load_categories(void* data_vector, sqlite3_stmt* pre_statement);

		~SQL();
	
		/// the number of rows that was read from the db using the last sql statement
		uint64_t row_count;

	private:
				sqlite3 *db;
		char *error_message;
};

#endif

