#include "gui_manager.h"

Gui_manager::Gui_manager() {
	settings = new Settings(&process_handler);
	renderer = new Renderer(settings);
	
	//predefined categories
	categories.push_back(Category(0,"Games"));
	
	sql = new SQL(settings->database_path.c_str());

	sql->load_data(&games,"SELECT * FROM games ORDER BY name COLLATE NOCASE", Game::callback_load_games);
	sql->load_data(&categories, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", Category::callback_load_categories);
	
	for(uint64_t i=0; i < categories.size(); i++) {
		categories_map[categories[i].id] = i;
	}

	nodes.push_back(new Node_games_grid(settings, renderer, &games));

	global_data.action = ACTION_NONE;
	global_data.current_game = 0;
	global_data.current_category = 0;

	global_data.categories_present.resize(categories.size(),false);
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
	bool return_value = renderer->check_input(&global_data);
	
	bool reload_game_vector = false;

	if( global_data.buttons_pressed[START]) global_data.action = ACTION_SHOW_START_MENU;

	nodes.back()->logic(&global_data);
	switch(global_data.action) {
		case(ACTION_SHOW_START_MENU) : 		nodes.push_back(new Node_start_menu(renderer,settings));
							break;

		case(ACTION_SWITCH_TO_CATEGORY_NODE) : 	nodes.push_back(new Node_category_menu(renderer, settings, &categories, global_data.current_category));
							break;
		
		case(ACTION_SWITCH_TO_CAT_TABLE_NODE) :	{	
								if(!games.empty()) {

									std::string sql_statement;
									sql_statement = "SELECT * FROM games LEFT JOIN games_categories ON games.id = games_categories.game_id LEFT JOIN categories ON games_categories.category_id = categories.id WHERE game_id = " + std::to_string(games[global_data.current_game].id); 
									std::vector<uint64_t> key_list;
									sql->load_data(&key_list, sql_statement.c_str(), this->callback_load_key_list);
								
									global_data.categories_present.resize(categories.size(), false);
									for(uint64_t i=0; i < key_list.size(); i++) {
										global_data.categories_present[categories_map[key_list[i]]] = true;	
									}
								
									nodes.push_back(new Node_category_table(renderer, settings, &categories, &global_data.categories_present));
								}
							}
							break;

		case(ACTION_READ_DATABASE) :		{
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

		case(ACTION_REMOVE_NODE) :		this->pop_node();
							break;

		case(ACTION_ADD_REMOVE_FAVORITE) :	if(!games.empty()) {	
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
								
								reload_game_vector = true;
															
							}
							break;
		
		case(ACTION_ADD_CATEGORIES) :		{
								//delete old categories
								std::string sql_statement = "DELETE FROM games_categories WHERE game_id=" + std::to_string(games.at(global_data.current_game).id);
								sql->load_data(nullptr, sql_statement.c_str(), nullptr);
			
								//add new categories
								for(uint64_t i=0; i < global_data.categories_present.size(); i++) {
									if( !global_data.categories_present[i] ) continue;
									sql_statement = "INSERT INTO games_categories (game_id, category_id) VALUES (" + std::to_string(games.at(global_data.current_game).id) + " ," + std::to_string(categories.at(i).id) + ")";
									global_data.categories_present[i] = false;
									sql->load_data(nullptr, sql_statement.c_str(), nullptr);
								}
								
								reload_game_vector = true;
								this->pop_node();
							}
							break;

		case(ACTION_RUN_GAME) :			{
								std::string command = std::string("lutris lutris:rungameid/") + std::to_string(games.at(global_data.current_game).id);
								process_handler.run_process(command.c_str());
							}
							break;
		
		case(ACTION_RESTART_SYSTEM) :		{
								std::string command = "shutdown -r now";
								process_handler.run_process(command.c_str());
							}
							break;

		case(ACTION_SHUTDOWN_SYSTEM) :		{
								std::string command = "shutdown -h now";
								process_handler.run_process(command.c_str());
							}
							break;

		case(ACTION_QUIT_TV_MODE) :		return false;
							break;
	}
	
	if(reload_game_vector) {
		std::string sql_statement;

		( global_data.current_category == 0) ? 
		(sql_statement = "SELECT * FROM games ORDER BY name COLLATE NOCASE") :
		(sql_statement = "SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = "
					+ std::to_string(categories[global_data.current_category].id) 
					+ " ORDER BY games.name COLLATE NOCASE");
		load_games_vector(sql_statement.c_str());
	}

	global_data.action = ACTION_NONE;

	if( global_data.current_game >= games.size() ) global_data.current_game = 0;

	process_handler.check_and_clean_zombie_processes();

	return return_value;
}

void Gui_manager::pop_node() {
	if(nodes.empty()) return;

	delete nodes.back();
	nodes.back() = nullptr;
	nodes.pop_back();						
}

void Gui_manager::load_games_vector(const char* sql_statement) {
	games.clear();
	renderer->clear_images();
	sql->load_data((void*)&games,sql_statement, Game::callback_load_games);
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

void Gui_manager::callback_load_key_list(void* data_vector, sqlite3_stmt* pre_statement) {
	std::vector<uint64_t>* key_list;
	key_list = (std::vector<uint64_t>*)data_vector;

	uint64_t temp_id = 0;
	
	uint64_t argc = sqlite3_column_count(pre_statement);

	for(uint64_t i=0; i < argc; i++) {
	
		std::string current_column_name = sqlite3_column_name(pre_statement,i);
	
		switch(sqlite3_column_type(pre_statement,i)) {
			case(SQLITE_INTEGER) : 	if( std::string("category_id").compare(current_column_name) == 0) temp_id = sqlite3_column_int(pre_statement, i);
						break;

		}
	}

	key_list->push_back(temp_id);
}
