#include "sql.h"
#include "gui.h"

/// \file

/// \todo check for throw exceptions while creating the game vector
int main(int argc, char** argv) {

	if(argc != 2) {
		std::clog << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT\n";
		return 1;
	}
	
	SQL lutris_db(argv[1]);
	std::vector<Game> games;
	Gui gui;

	if( gui.gui_init() != 0 ) {
		return 1;
	}
	
	lutris_db.load_data(&games);
	
	std::cout << games.size() << "\n";

	for(uint64_t i=0; i < games.size(); i++) {
		gui.load_texture(games[i].slug.c_str());
		games[i].set_images_indexes(i,i);
	}

	bool running = true;

	while(running) {
		gui.input(&running);

		gui.logic();

		gui.render();
	}

	return 0;

}

