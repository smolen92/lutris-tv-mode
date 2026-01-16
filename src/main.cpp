#include "sql.h"
#include "gui.h"

/// \file

/// \todo check for throw exceptions while creating the game vector
int main(int argc, char** argv) {

	//Setup
	
	//settings
	Settings settings(nullptr);

	//database and reading data to game vector
	SQL lutris_db(settings.database_path.c_str());
	std::vector<Game> games;
	lutris_db.load_data(&games);
	
	//gui initialization
	Gui gui;
	if( gui.gui_init(&settings,&games) != 0 ) {
		return 1;
	}
	
	bool running = true;

	//Main loop
	while(running) {
		gui.input(&running);

		gui.logic();

		gui.render();
	}

	return 0;

}

