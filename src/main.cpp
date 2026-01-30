#include "gui.h"

/// \file
/// \cond
int main(int argc, char** argv) {
	try {
		//Setup
		std::vector<Game> games;
		std::vector<Category> categories;
		
		//settings
		Settings settings("settings.ini");

		//database and reading data to game vector
		SQL lutris_db(settings.database_path.c_str());

		lutris_db.load_data(&games,"SELECT * FROM games ORDER BY name COLLATE NOCASE", lutris_db.callback_load_games);
		lutris_db.load_data(&categories, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", lutris_db.callback_load_categories);

		//gui initialization
		Gui gui(&settings,&lutris_db,&games,&categories);
		
		bool running = true;

		//Main loop
		while(running) {
			gui.input(&running);

			gui.logic();

			gui.render();
		}


	}
	catch (const std::runtime_error & error) {
		std::clog << "Exception: " << error.what() << "\n";
		return 1;
	}

	return 0;

}
/// \endcond
