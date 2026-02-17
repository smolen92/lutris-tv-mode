#include "game.h"

Game::Game(const uint64_t id, const char* name, const char* slug) {
	this->id = id;
	
	this->name = name;
	this->slug = slug;

}

void Game::set_images_indexes(uint64_t cover_art_index, uint64_t banner_index) {
	this->cover_art_index = cover_art_index;
	this->banner_index = banner_index;
}

void Game::callback_load_games(void* data_vector, sqlite3_stmt* pre_statement) {
	std::vector<Game>* game_ptr;
	game_ptr = (std::vector<Game>*)data_vector;
	
	uint64_t temp_id = 0;
	std::string temp_name;
	std::string temp_slug;
	
	uint64_t argc = sqlite3_column_count(pre_statement);

	for(uint64_t i=0; i < argc; i++) {

		std::string current_column_name = sqlite3_column_name(pre_statement, i);

		switch(sqlite3_column_type(pre_statement,i)) {
				case(SQLITE_INTEGER) : 	if( std::string("id").compare(current_column_name) == 0) temp_id = sqlite3_column_int(pre_statement, i);
							break;

				case(SQLITE_TEXT) : 	if( std::string("name").compare(current_column_name) == 0) temp_name = (const char*) sqlite3_column_text(pre_statement,i);
							if( std::string("slug").compare(current_column_name) == 0) temp_slug = (const char*) sqlite3_column_text(pre_statement,i);
							break;
		}

	}
	
	game_ptr->push_back(Game(temp_id, temp_name.c_str(), temp_slug.c_str()));
	
}
