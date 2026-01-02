#include "sql.h"

SQL::SQL(const char* database) {
	this->rc = sqlite3_open(database, &db);
	if( rc != SQLITE_OK) {
		std::clog << "Can't open database: " << sqlite3_errmsg(db) << "\n";
	}
	zErrMsg = nullptr;
}

void SQL::load_data(std::vector<Game> *game_vec) {
	this->rc = sqlite3_exec(this->db, "SELECT * FROM games", this->callback_load_data, game_vec, &zErrMsg);
	if( this->rc != SQLITE_OK) {
		std::clog << "SQL error: " << zErrMsg << "\n";
	}

}

SQL::~SQL() {
	sqlite3_free(zErrMsg);
	sqlite3_close(db);
}

int SQL::callback(void *not_used, int argc, char **argv, char **az_col_name) {
	for(int i=0; i < argc; i++) {
		std::cout << az_col_name[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	std::cout << "\n";
	return 0;
}

int SQL::callback_load_data(void* data_vector, int argc, char** argv, char **az_col_name) {
	std::vector<Game>* game_ptr;
	game_ptr = (std::vector<Game>*)data_vector;

	uint64_t temp_id;
	char temp_name[1000];
	char temp_slug[1000];

	for(int i=0; i < argc; i++) {
		if( std::strcmp(az_col_name[i], "id") == 0) temp_id = atoi(argv[i]);
		if( std::strcmp(az_col_name[i], "name") == 0) std::strcpy(temp_name, argv[i]);
		if( std::strcmp(az_col_name[i], "slug") == 0) std::strcpy(temp_slug, argv[i]);
	}

	game_ptr->push_back(Game(temp_id, temp_name, temp_slug));

	return 0;
}
