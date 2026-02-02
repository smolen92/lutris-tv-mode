#include "gui_manager.h"

Gui_manager::Gui_manager() {
	settings = new Settings("settings.ini");
	renderer = new Renderer(settings);
	
	//predefined categories
	categories.push_back(Category(0,"Games"));
	
	sql = new SQL(settings->database_path.c_str());
	sql->load_data(&games,"SELECT * FROM games ORDER BY name COLLATE NOCASE", sql->callback_load_games);
	sql->load_data(&categories, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", sql->callback_load_categories);

	nodes.push_back(new Node_games_grid(settings, renderer, &games));

	global_data.action = NONE;
	global_data.current_game = 0;
	global_data.current_category = 0;
}
		
Gui_manager::~Gui_manager() {
	while(!nodes.empty()) {
		delete nodes.back();
		nodes.pop_back();
	}

	delete renderer;
	renderer = nullptr;

	delete settings;
	settings = nullptr;

	delete sql;
	sql = nullptr;
}
		
bool Gui_manager::logic() {
	bool return_value = renderer->check_input(global_data.buttons_pressed);

	nodes.back()->logic(&global_data);
	if( global_data.action == SWITCH_TO_CATEGORY_NODE ) nodes.push_back(new Node_category_menu(renderer, settings, &categories));
	
	if( global_data.action == READ_DATABASE) {	
		global_data.current_game = 0;
		games.clear();
		renderer->clear_images();
		
		std::string sql_statement;

		if(global_data.current_category == 0) {
			sql_statement = std::string("SELECT * FROM games ORDER BY name COLLATE NOCASE");
		} else {
			sql_statement = std::string("SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = ")
				+ std::to_string(categories[global_data.current_category].id) 
				+ std::string(" ORDER BY games.name COLLATE NOCASE");
		}
		
		sql->load_data((void*)&games,sql_statement.c_str(), sql->callback_load_games);
		load_images();	
	}

	if( global_data.action == REMOVE_NODE) {
		delete nodes.back();
		nodes.pop_back();
	}
	
	global_data.action = NONE;

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
