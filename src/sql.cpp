#include "sql.h"

SQL::SQL(const char* database) {
	if( sqlite3_open(database, &db) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}
	error_message = nullptr;
}

void SQL::load_data(void *data_ptr, const char* sql_statement, int (*callback_function)(void*,int,char**,char**)) {
	if( sqlite3_exec(db, sql_statement, callback_function, data_ptr, &error_message) != SQLITE_OK) {
		throw std::runtime_error(error_message);
	}
}

SQL::~SQL() {
	sqlite3_free(error_message);
	error_message = nullptr;
	sqlite3_close(db);
	db = nullptr;
}

int SQL::callback_load_games(void* data_vector, int argc, char** argv, char **az_col_name) {
	std::vector<Game>* game_ptr;
	game_ptr = (std::vector<Game>*)data_vector;
	
	uint64_t temp_id;
	std::string temp_name;
	std::string temp_slug;

	for(int i=0; i < argc; i++) {
		if( std::string("id").compare(az_col_name[i]) == 0) temp_id = atoi(argv[i]);
		if( std::string("name").compare(az_col_name[i]) == 0) temp_name = argv[i];
		if( std::string("slug").compare(az_col_name[i]) == 0) temp_slug = argv[i];
	}
	
	game_ptr->push_back(Game(temp_id, temp_name.c_str(), temp_slug.c_str()));

	return 0;
}

int SQL::callback_load_categories(void* data_vector, int argc, char** argv, char **az_col_name) {
	std::vector<Category>* category_ptr;
	category_ptr = (std::vector<Category>*)data_vector;
	

	uint64_t temp_id;
	std::string temp_name;
	
	for(int i=0; i < argc; i++) {
		if( std::string("id").compare(az_col_name[i]) == 0) temp_id = atoi(argv[i]);
		if( std::string("name").compare(az_col_name[i]) == 0) temp_name = argv[i];
	}

	category_ptr->push_back(Category(temp_id, temp_name.c_str()));

	return 0;
}

