#include "settings.h"

Settings::Settings(ProcessHandler *p_handler) {
	load_defaults(p_handler->user.c_str());
	calculate_settings();

	std::fstream settings_file;

	settings_file_path = std::string(p_handler->cwd) + "/lutris-tvmode-settings.ini";
	settings_file.open(settings_file_path, std::fstream::in);
	if(!settings_file) {
		settings_file_path = "/home/" + p_handler->user + "/.config/lutris-tvmode-settings.ini";
		settings_file.open(settings_file_path, std::fstream::in);
		if(!settings_file) {
			std::cout << "No settings file found\n";
			return;
		}
	}

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
		else if(line.substr(0,first_equal_sign_location).compare("playtime_text_width") == 0) {
			playtime_text_width = std::stol(line.substr(first_equal_sign_location+1));
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
		else if(line.substr(0,first_equal_sign_location).compare("selection_timeout") == 0) {
			selection_timeout = std::stol(line.substr(first_equal_sign_location+1));
		}
		else if(line.substr(0,first_equal_sign_location).compare("status_bar_controls_offset") == 0) {
			status_bar_controls_offset = std::stol(line.substr(first_equal_sign_location+1));
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

void Settings::load_defaults(const char* user) {
	cover_art_path = "/home/" + std::string(user) + ".local/share/coverart/";
	banner_path = "home" + std::string(user) + ".local/share/banners/";
	database_path = "home" + std::string(user) + ".local/share/pga.db";

	window_width = 800;
	window_height = 600;

	min_window_width = 850;
	min_window_height = 480;

	window_maximized = true;

	game_tile_width = 120;
	game_tile_height = 160;
	vertical_padding = 5;
	horizontal_padding = 5;
	
	font_size = 12;

	playtime_text_width = 400;

	gamepad_deadzone = 1000;
	
	category_menu_x = 0;
	category_menu_y = 0;

	selection_timeout = 200;

	status_bar_controls_offset = 350;
}

Settings::~Settings() {
	std::fstream output_file(settings_file_path,std::fstream::out);

	if(!output_file) {
		std::clog << "Unable to save settings\n";
		return;
	}

	output_file 	<< "[path]"
			<< "\ncover_art_path=" << cover_art_path.c_str() 
			<< "\nbanner_path=" << banner_path.c_str()
			<< "\ndatabase_path=" << database_path.c_str()
			<< "\n\n[window]"
			<< "\nwindow_width=" << window_width 
			<< "\nwindow_height=" << window_height
			<< "\nmin_window_width=" << min_window_width
			<< "\nmin_window_height=" << min_window_height
			<< "\nwindow_maximized=";

	(window_maximized) ? (output_file << "true") : (output_file << "false");

	output_file	<< "\n\n[game_tile]" 
			<< "\ngame_tile_width=" << game_tile_width
			<< "\ngame_tile_height=" << game_tile_height
			<< "\nvertical_padding=" << vertical_padding
			<< "\nhorizontal_padding=" << horizontal_padding
			<< "\nfont_size=" << font_size
			<< "\nplaytime_text_width=" << playtime_text_width
			<< "\n\n[gamepad]"
			<< "\ngamepad_deadzone=" << gamepad_deadzone
			<< "\n\n[animation]"
			<< "\nselection_timeout=" << selection_timeout
			<< "\n\n[category]" 
			<< "\ncategory_menu_x=" << category_menu_x
			<< "\ncategory_menu_y=" << category_menu_y
			<< "\n\n[status_bar]"
			<< "\nstatus_bar_controls_offset=" << status_bar_controls_offset;
}
