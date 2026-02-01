#include "gui_manager.h"

/// \todo add nodes, handle global_data in all nodes
Gui_manager::Gui_manager() {
	settings = new Settings("settings.ini");
	
	sql = new SQL(settings->database_path.c_str());
		
	sql->load_data(&games,"SELECT * FROM games ORDER BY name COLLATE NOCASE", sql->callback_load_games);
	sql->load_data(&categories, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", sql->callback_load_categories);
}
		
Gui_manager::~Gui_manager() {
	delete settings;
	settings = nullptr;

	delete sql;
	sql = nullptr;
}
		
bool Gui_manager::logic() {
	bool return_value = renderer->check_input(global_data.buttons_pressed);

	nodes.at(global_data.active_node_id)->logic((void*)&global_data);
	/// \todo change magic number
	if( global_data.buttons_pressed[CATEGORIES] ) global_data.active_node_id = 1;
	if( global_data.read_database) {
		global_data.current_game = 0;
		games.clear();
		renderer->clear_images();
		std::string sql_statement = std::string("SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = ")
			+ std::to_string(categories[global_data.current_category].id) 
			+ std::string(" ORDER BY games.name COLLATE NOCASE");
		sql->load_data((void*)&games,sql_statement.c_str(), sql->callback_load_games);
		load_images();	
	}

	return return_value;
}

void Gui_manager::render() {
	renderer->clear_screen();
	for(uint64_t i=0; i < nodes.size(); i++) {
		nodes[i]->render();
	}
	renderer->draw_screen();
}

void Gui_manager::load_images() {
	for(uint64_t i=0; i < games.size(); i++) {
		renderer->load_texture(games[i].slug.c_str());
	}
}
