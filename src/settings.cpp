#include "settings.h"

/// \todo load settings from file
Settings::Settings(const char* path) {
	//cover_art_path = "/home/bazzite/.local/share/lutris/coverart/";
	//banner_path = "/home/bazzite/.local/share/lutris/banners/";
	//database_path = "/home/bazzite/.local/share/lutris/pga.db";

	cover_art_path = "./test-data/coverart/";
	banner_path = "./test-data/banners/";
	database_path = "./test-data/pga.db";

	window_width = 800;
	window_height = 600;
	
	game_tile_width = 120;
	game_tile_height = 160;
	vertical_padding = 5;
	horizontal_padding = 5;
	
	games_per_row = (uint64_t)(window_width - horizontal_padding)/(int64_t)(game_tile_width + horizontal_padding);
	
	font_size = 12;

	gamepad_deadzone = 1000;
}

