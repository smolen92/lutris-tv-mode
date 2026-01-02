#ifndef _SQL_H_
#define _SQL_H_

#include <sqlite3.h> //https://www.sqlite.org/cintro.html
#include <vector>

#include "game.h"


class SQL {
	public:
		SQL(const char* database);
		
		void load_data(std::vector<Game> *game_vec);

		~SQL();

	private:
		sqlite3 *db;
		char *error_message;
		static int callback_load_data(void* data_vector, int argc, char** argv, char **az_col_name);

};

#endif

