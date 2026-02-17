#include "category.h"

Category::Category(uint64_t id, const char* name) {
	this->id = id;
	this->name = name;
}

void Category::callback_load_categories(void* data_vector, sqlite3_stmt* pre_statement) {
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

