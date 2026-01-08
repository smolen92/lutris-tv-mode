#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <cstdint>
#include <string>
#include <iostream>

class Settings {
	public:
		Settings(const char* path);

		std::string cover_art_path, banner_path, database_path;
		
		uint32_t window_width, window_height;
		
		int32_t game_tile_width, game_tile_height;
		uint32_t vertical_padding, horizontal_padding; 
	        uint64_t games_per_row;

		uint32_t font_size;
};

#endif
