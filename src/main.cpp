#include <cstring>
#include <iostream>
#include <cstdint>
#include <sqlite3.h> //https://www.sqlite.org/cintro.html

static int callback(void *not_used, int argc, char **argv, char **az_col_name) {
	for(int i=0; i < argc; i++) {
		std::cout << az_col_name[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
	}
	std::cout << "\n";
	return 0;
}

int main(int argc, char** argv) {

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	if(argc != 2) {
		std::clog << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT\n";
		return 1;
	}

	rc = sqlite3_open(argv[1], &db);
	if( rc ) {
		std::clog << "Can't open database: " << sqlite3_errmsg(db) << "\n";
		sqlite3_close(db);
		return 1;
	}
	
	bool running = true;
	char sql_statement_input[1000];

	while(running) {
		
		std::cin.getline(sql_statement_input, 1000);	
		
		if( std::strcmp(sql_statement_input, "QUIT") == 0) {
			running = false;
			continue;
		}

		rc = sqlite3_exec(db, sql_statement_input, callback, 0, &zErrMsg);
		if( rc != SQLITE_OK) {
			std::clog << "SQL error: " << zErrMsg << "\n";
			sqlite3_free(zErrMsg);
		}
	}

	sqlite3_close(db);
	return 0;

}

