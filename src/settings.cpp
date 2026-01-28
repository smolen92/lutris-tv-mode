#include "settings.h"

/// \todo save current settings when quitting the app
Settings::Settings(const char* path) {
	
	std::fstream settings_file;
	settings_file.open(path, std::fstream::in);

	load_defaults();
	
	if(settings_file) {
		std::string line;
		while(std::getline(settings_file, line)) {
			if( (line[0] == ';') || (line[0] == '#') ) continue;

			uint64_t first_equal_sign_location = line.find_first_of("=");
			
			if( first_equal_sign_location == std::string::npos) continue;
		
			//+1 to ignore the = sign
			if(line.substr(0,first_equal_sign_location).compare("cover_art_path") == 0) {
				cover_art_path = line.substr(first_equal_sign_location+1); 
			}
			else if(line.substr(0,first_equal_sign_location).compare("banner_path") == 0) {
				banner_path = line.substr(first_equal_sign_location+1);
			} 
			else if(line.substr(0,first_equal_sign_location).compare("database_path") == 0) {
				database_path = line.substr(first_equal_sign_location+1);
			} 
			else if(line.substr(0,first_equal_sign_location).compare("window_width") == 0) {
				window_width = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("window_height") == 0) {
				window_height = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("min_window_width") == 0) {
				min_window_width = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("min_window_height") == 0) {
				min_window_height = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("window_maximized") == 0) {
				if(line.substr(first_equal_sign_location+1).compare("false") == 0) window_maximized = false;
				if(line.substr(first_equal_sign_location+1).compare("true") == 0) window_maximized = true;
			} 
			else if(line.substr(0,first_equal_sign_location).compare("game_tile_width") == 0) {
				game_tile_width = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("game_tile_height") == 0) {
				game_tile_height = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("vertical_padding") == 0) {
				vertical_padding = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("horizontal_padding") == 0) {
				horizontal_padding = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("font_size") == 0) {
				font_size = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("gamepad_deadzone") == 0) {
				gamepad_deadzone = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("category_menu_x") == 0) {
				category_menu_x = std::stol(line.substr(first_equal_sign_location+1));
			} 
			else if(line.substr(0,first_equal_sign_location).compare("category_menu_y") == 0) {
				category_menu_y = std::stol(line.substr(first_equal_sign_location+1));
			} 

		}

	}
	
	calculate_settings();
}

void Settings::calculate_settings() {
	games_per_row = (uint64_t)(window_width - horizontal_padding)/(int64_t)(game_tile_width + horizontal_padding);
	category_menu_width = window_width>>3;
	category_menu_height = window_height;
	
	min_category_menu_width = min_window_width>>2;
	max_category_menu_width = window_width>>2;
}

void Settings::load_defaults() {
	cover_art_path = "./test-data/coverart/";
	banner_path = "./test-data/banners/";
	database_path = "./test-data/pga.db";

	window_width = 800;
	window_height = 600;

	min_window_width = 640;
	min_window_height = 480;

	window_maximized = true;

	game_tile_width = 120;
	game_tile_height = 160;
	vertical_padding = 5;
	horizontal_padding = 5;
	
	font_size = 12;

	gamepad_deadzone = 1000;
	
	category_menu_x = 0;
	category_menu_y = 0;
}
