#include "settings.h"

/// \todo load settings from file
Settings::Settings(const char* path) {
	cover_art_path = "./test-data/coverart/";
	banner_path = "./test-data/banners/";
	database_path = "./test-data/pga.db";

	window_width = 800;
	window_height = 600;
	
	game_tile_width = 100;
	game_tile_height = 133;
	vertical_padding = 5;
	horizontal_padding = 5;

	font_size = 16;

}

