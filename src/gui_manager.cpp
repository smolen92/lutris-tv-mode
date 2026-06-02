#include "gui_manager.h"

Gui_manager::Gui_manager() {
	settings = new Settings(&process_handler);
	renderer = new Renderer(settings);
	
	//predefined categories
	categories.push_back(Category(0,"Games"));
	
	sql = new SQL(settings->database_path.c_str());
	
	sqlite3_stmt* prepared_statement;
	sqlite3_prepare_v2(sql->db, "SELECT * FROM games ORDER BY name COLLATE NOCASE", -1, &prepared_statement, NULL);
	sql->load_data(&games,prepared_statement, Game::callback_load_games);
	
	sqlite3_prepare_v2(sql->db, "SELECT * FROM categories ORDER BY name COLLATE NOCASE", -1, &prepared_statement, NULL);
	sql->load_data(&categories, prepared_statement , Category::callback_load_categories);
	
	for(uint64_t i=0; i < categories.size(); i++) {
		categories_map[categories[i].id] = i;
	}

	nodes.push_back(new Node_games_grid(&process_handler, settings, renderer, &global_data, &games));

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

	nodes.back()->logic();
	switch(global_data.action) {
		case(ACTION_SHOW_START_MENU) : 		nodes.push_back(new Node_start_menu(&process_handler, renderer,settings,&global_data));
							break;

		case(ACTION_SWITCH_TO_CATEGORY_NODE) : 	nodes.push_back(new Node_category_menu(&process_handler, renderer, settings, &global_data, &categories));
							break;
		
		case(ACTION_SWITCH_TO_CAT_TABLE_NODE) :	{	
								if(!games.empty()) {

									sqlite3_stmt* prepared_statement;
									std::string sql_statement;
									sql_statement = "SELECT * FROM games LEFT JOIN games_categories ON games.id = games_categories.game_id LEFT JOIN categories ON games_categories.category_id = categories.id WHERE game_id = (?)"; 
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
									sqlite3_bind_int(prepared_statement, 1, games[global_data.current_game].id);

									std::vector<uint64_t> key_list;
									sql->load_data(&key_list, prepared_statement, this->callback_load_key_list);
								
									global_data.categories_present.resize(categories.size(), false);
									for(uint64_t i=0; i < key_list.size(); i++) {
										global_data.categories_present[categories_map[key_list[i]]] = true;	
									}
								
									nodes.push_back(new Node_category_table(&process_handler, renderer, settings, &global_data, &categories));
								}
							}
							break;

		case(ACTION_READ_DATABASE) :		{
								sqlite3_stmt* prepared_statement;

								if(global_data.current_category == 0) {
									std::string sql_statement = "SELECT * FROM games ORDER BY name COLLATE NOCASE";
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
								} else {
									std::string sql_statement = "SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = (?) ORDER BY games.name COLLATE NOCASE";
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
									sqlite3_bind_int(prepared_statement, 1, categories[global_data.current_category].id); 
								}
							
								load_games_vector(prepared_statement);
							}
							break;

		case(ACTION_REMOVE_NODE) :		this->pop_node();
							break;

		case(ACTION_ADD_REMOVE_FAVORITE) :	if(!games.empty()) {

								sqlite3_stmt *prepared_statement;
								std::string sql_statement = "SELECT * FROM games_categories WHERE game_id= (?) AND category_id=1"; 
								sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
								sqlite3_bind_int(prepared_statement, 1, games.at(global_data.current_game).id); 
								sql->load_data(nullptr, prepared_statement, nullptr);

								if(sql->row_count == 0) {
									sqlite3_stmt* prepared_statement;
									sql_statement = "INSERT INTO games_categories VALUES ((?),1)"; 
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
									sqlite3_bind_int(prepared_statement, 1, games.at(global_data.current_game).id);	
									sql->load_data(nullptr, prepared_statement, nullptr);
								}
								else {
									sqlite3_stmt* prepared_statement;
									sql_statement = "DELETE FROM games_categories WHERE game_id = (?) AND category_id=1"; 
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
									sqlite3_bind_int(prepared_statement, 1, games.at(global_data.current_game).id);
									sql->load_data(nullptr, prepared_statement, nullptr);
								}
								
								reload_game_vector = true;
															
							}
							break;
		
		case(ACTION_ADD_CATEGORIES) :		{
								//delete old categories
								sqlite3_stmt *prepared_statement;
								std::string sql_statement = "DELETE FROM games_categories WHERE game_id = (?)";
								sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
								sqlite3_bind_int(prepared_statement, 1, games.at(global_data.current_game).id);
								sql->load_data(nullptr, prepared_statement, nullptr);
			
								//add new categories
								for(uint64_t i=0; i < global_data.categories_present.size(); i++) {
									if( !global_data.categories_present[i] ) continue;
									sqlite3_stmt *prepared_statement;
									sql_statement = "INSERT INTO games_categories (game_id, category_id) VALUES (?,?)"; 
									sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
									sqlite3_bind_int(prepared_statement, 1, games.at(global_data.current_game).id); 
									sqlite3_bind_int(prepared_statement, 2, categories.at(i).id);
									global_data.categories_present[i] = false;
									sql->load_data(nullptr, prepared_statement, nullptr);
								}
								
								reload_game_vector = true;
								this->pop_node();
							}
							break;

		case(ACTION_RUN_GAME) :			{
								if( process_handler.is_process_running(global_data.current_game) ) {
									process_handler.kill_process(global_data.current_game, SIGTERM);
								}
								else {
									std::string command = std::string("lutris lutris:rungameid/") + std::to_string(games.at(global_data.current_game).id);
									//std::string command = std::string("ping 127.0.0.1");
									process_handler.run_process(global_data.current_game,command.c_str());
								}
							}
							break;
		
		case(ACTION_RESTART_SYSTEM) :		{
								std::string command = "shutdown -r now";
								process_handler.run_process(global_data.current_game,command.c_str());
							}
							break;

		case(ACTION_SHUTDOWN_SYSTEM) :		{
								std::string command = "shutdown -h now";
								process_handler.run_process(global_data.current_game,command.c_str());
							}
							break;

		case(ACTION_QUIT_TV_MODE) :		return false;
							break;

		case(ACTION_KILL_PROCESS) :		if( process_handler.is_process_running(global_data.current_game) ) process_handler.kill_process(global_data.current_game, SIGKILL);
							break;
	}
	
	if(reload_game_vector) {
		sqlite3_stmt* prepared_statement;

		if ( global_data.current_category == 0) {
			std::string sql_statement = "SELECT * FROM games ORDER BY name COLLATE NOCASE";
			sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
		}
		else {
			std::string sql_statement = "SELECT * FROM categories LEFT JOIN games_categories ON categories.id = games_categories.category_id LEFT JOIN games ON games_categories.game_id = games.id WHERE category_id = (?) ORDER BY games.name COLLATE NOCASE";
			sqlite3_prepare_v2(sql->db, sql_statement.c_str(), -1, &prepared_statement, NULL);
			sqlite3_bind_int(prepared_statement, 1, categories[global_data.current_category].id);	
		}

		load_games_vector(prepared_statement);
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

void Gui_manager::load_games_vector(sqlite3_stmt* prepared_statement) {
	games.clear();
	renderer->clear_images();
	sql->load_data((void*)&games,prepared_statement, Game::callback_load_games);
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
