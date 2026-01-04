#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <cstdint>
#include <string>

class Settings {
	public:
		void load_settings(const char* path);

		std::string cover_art_path, banner_path, database_path;
		uint32_t window_width, window_height;
		uint32_t game_tile_width, game_tile_height;
		uint32_t vertical_padding, horizontal_padding; 
};

#endif
