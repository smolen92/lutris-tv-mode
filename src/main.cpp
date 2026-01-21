#include "sql.h"
#include "gui.h"

/// \file

/// \todo when category change delete game vector and load it again from database
int main(int argc, char** argv) {

	//Setup
	std::vector<Game> games;
	std::vector<Category> categories;
	
	//settings
	Settings settings(nullptr);

	//database and reading data to game vector
	try {
		SQL lutris_db(settings.database_path.c_str());

		lutris_db.load_data(&games,"SELECT * FROM games ORDER BY name COLLATE NOCASE", lutris_db.callback_load_games);
		/// \todo combine this with LEFT JOIN
		lutris_db.load_data(&categories, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", lutris_db.callback_load_categories);
		lutris_db.load_data(&categories, "SELECT * FROM games_categories", lutris_db.callback_load_games_categories);

		/// \todo change this triple loop when converting from game_id to vector index
		for(uint64_t i=0; i < categories.size(); i++) {
			for(uint64_t j=0; j < categories[i].games_indexes.size(); j++) {
				for(uint64_t k=0; k < games.size(); k++) {
					if(categories[i].games_indexes[j] == games[k].id) {
						categories[i].games_indexes[j] = k;
						break;
					}
				}
			}
		}

		//gui initialization
		Gui gui(&settings,&games,&categories);
		
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
	}

	return 0;

}

