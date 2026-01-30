#include "gui.h"

Gui::Gui(Settings *settings, SQL *sql, std::vector<Game> *games, std::vector<Category> *categories) {
	this->sql = sql;
}

void Gui::logic() {

	if( buttons_pressed[RUN] ) {
		if(render_categories) {
			current_game = 0;
			games->clear();
			clear_images();
			std::string sql_statement = std::string("SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = ")
				+ std::to_string(categories->at(current_category).id) 
				+ std::string(" ORDER BY games.name COLLATE NOCASE");
			sql->load_data(games,sql_statement.c_str(), sql->callback_load_games);
			load_images();	
		} 	
	}
	
	if( buttons_pressed[CATEGORIES] ) {
		render_categories = !render_categories;
	}

}

