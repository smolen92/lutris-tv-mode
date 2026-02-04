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
		nodes.back() = nullptr;
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
	switch(global_data.action) {
		case(SWITCH_TO_CATEGORY_NODE) : nodes.push_back(new Node_category_menu(renderer, settings, &categories));
						break;

		case(READ_DATABASE) :		{
							std::string sql_statement;

							if(global_data.current_category == 0) {
								sql_statement = "SELECT * FROM games ORDER BY name COLLATE NOCASE";
							} else {
								sql_statement = "SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = "
									+ std::to_string(categories[global_data.current_category].id) 
									+ " ORDER BY games.name COLLATE NOCASE";
							}
							
							load_games_vector(sql_statement.c_str());
						}
						break;

		case(REMOVE_NODE) :		delete nodes.back();
						nodes.back() = nullptr;
						nodes.pop_back();
						break;

		case(ADD_REMOVE_FAVORITE) :	if(!games.empty()) {	
							/// \todo add game to any category - will need new render node
							std::string sql_statement = "SELECT * FROM games_categories WHERE game_id=" + std::to_string(games.at(global_data.current_game).id) + " AND category_id=1";
							sql->load_data(nullptr, sql_statement.c_str(), nullptr);

							if(sql->row_count == 0) {
								sql_statement = "INSERT INTO games_categories VALUES (" + std::to_string(games.at(global_data.current_game).id) + ",1)";	
								sql->load_data(nullptr, sql_statement.c_str(), nullptr);
							}
							else {
								sql_statement = "DELETE FROM games_categories WHERE game_id=" + std::to_string(games.at(global_data.current_game).id) + " AND category_id=1";
								sql->load_data(nullptr, sql_statement.c_str(), nullptr);
							}
							
							( global_data.current_category == 0) ? 
							(sql_statement = "SELECT * FROM games ORDER BY name COLLATE NOCASE") :
							(sql_statement = "SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = "
									+ std::to_string(categories[global_data.current_category].id) 
									+ " ORDER BY games.name COLLATE NOCASE");
							load_games_vector(sql_statement.c_str());


														
						}
						break;

	}
	
	global_data.action = NONE;

	if( global_data.current_game > games.size() ) global_data.current_game = 0;

	return return_value;
}

void Gui_manager::load_games_vector(const char* sql_statement) {
	games.clear();
	renderer->clear_images();
	sql->load_data((void*)&games,sql_statement, sql->callback_load_games);
	load_images();
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
