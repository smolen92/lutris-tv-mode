#include "sql.h"

SQL::SQL(const char* database) {
	if( sqlite3_open(database, &db) != SQLITE_OK) {
		std::clog << "Can't open database: " << sqlite3_errmsg(db) << "\n";
	}
	error_message = nullptr;
}

void SQL::load_data(std::vector<Game> *game_vec) {
	if( sqlite3_exec(this->db, "SELECT * FROM games", this->callback_load_data, game_vec, &error_message) != SQLITE_OK) {
		std::clog << "SQL error: " << error_message << "\n";
	}

}

SQL::~SQL() {
	sqlite3_free(error_message);
	error_message = nullptr;
	sqlite3_close(db);
	db = nullptr;
}

int SQL::callback_load_data(void* data_vector, int argc, char** argv, char **az_col_name) {
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
