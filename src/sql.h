#ifndef _SQL_H_
#define _SQL_H_

#include <sqlite3.h> //https://www.sqlite.org/cintro.html
#include <stdexcept>
#include <cstdint>

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
		 * @param prepared_statement prepared sqlite3 query that will be run in database
		 * @param callback_function callback function that will be called for data return from database
		 *
		 * @details this function will call sqlite3_finalize for prepared statement
		 * 
		 */
		void load_data(void *data, sqlite3_stmt* prepared_statement, void (*callback_function)(void*,sqlite3_stmt* pre_statement));
	
		~SQL();
	
		/// the number of rows that was read from the db using the last sql statement
		uint64_t row_count;
		
		/** database
		 * \ingroup improvements
		 * sql prepared statement in gui_manager.cpp use a lot of repeated code
		 * move db back to private and figure out some interface how to prepare statement and bind any type of values
		 */
		sqlite3 *db;
	private:
		char *error_message;
};

#endif

