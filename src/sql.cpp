#include "sql.h"

SQL::SQL(const char* database) {
	if( sqlite3_open(database, &db) != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(db));
	}
	error_message = nullptr;
	row_count = 0;
}

void SQL::load_data(void *data_ptr, const char* sql_statement, void (*callback_function)(void*,sqlite3_stmt*)) {
	row_count = 0;
	
	sqlite3_stmt* prepared_statement;

	sqlite3_prepare_v2(db, sql_statement, std::strlen(sql_statement) , &prepared_statement, nullptr);

	if(prepared_statement != nullptr) {
		
		while(sqlite3_step(prepared_statement) == SQLITE_ROW) {
			if(callback_function != nullptr) callback_function(data_ptr,prepared_statement);	
			row_count++;
		}

		uint64_t finalize_return_value = sqlite3_finalize(prepared_statement);
		if( finalize_return_value != SQLITE_OK) {
			throw std::runtime_error("Statement failed with error " + std::to_string(finalize_return_value));
		}
	}
	else {
		throw std::runtime_error("No/bad sql statement");
	}
}

SQL::~SQL() {
	sqlite3_free(error_message);
	error_message = nullptr;
	sqlite3_close(db);
	db = nullptr;
}

void SQL::callback_load_games(void* data_vector, sqlite3_stmt* pre_statement) {
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

void SQL::callback_load_categories(void* data_vector, sqlite3_stmt* pre_statement) {
	std::vector<Category>* category_ptr;
	category_ptr = (std::vector<Category>*)data_vector;

	uint64_t temp_id = 0;
	std::string temp_name;
	
	uint64_t argc = sqlite3_column_count(pre_statement);

	for(uint64_t i=0; i < argc; i++) {
	
		std::string current_column_name = sqlite3_column_name(pre_statement,i);
		
		switch(sqlite3_column_type(pre_statement,i)) {
			case(SQLITE_INTEGER) : 	if( std::string("id").compare(current_column_name) == 0) temp_id = sqlite3_column_int(pre_statement, i);
						break;

			case(SQLITE_TEXT) :	if( std::string("name").compare(current_column_name) == 0) temp_name = (const char*) sqlite3_column_text(pre_statement,i);
						break;
		}
	}

	category_ptr->push_back(Category(temp_id, temp_name.c_str()));

}

