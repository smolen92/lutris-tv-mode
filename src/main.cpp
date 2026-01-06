#include "sql.h"
#include "gui.h"

/// \file

/// \todo check for throw exceptions while creating the game vector
int main(int argc, char** argv) {

	Settings settings(nullptr);

	SQL lutris_db(settings.database_path.c_str());
	std::vector<Game> games;
	Gui gui;

	if( gui.gui_init(&settings,&games) != 0 ) {
		return 1;
	}
	
	lutris_db.load_data(&games);
	
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

