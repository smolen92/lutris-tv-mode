#include "sql.h"

#include <cstring>

int main(int argc, char** argv) {

	if(argc != 2) {
		std::clog << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT\n";
		return 1;
	}
	
	SQL lutris_db(argv[1]);
	
	std::vector<Game> games;
	
	lutris_db.load_data(&games);

	for(uint64_t i=0; i < games.size(); i++) {
		games[i].print();
	}

	return 0;

}

