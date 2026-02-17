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

	sqlite3_prepare_v2(db, sql_statement, -1, &prepared_statement, nullptr);

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

