#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <cstdint>
#include <string>

/**
 * @brief class that stores settings for the app
 */
class Settings {
	public:
		/**
		 * @brief load settings
		 *
		 * @param path path to file with settings
		 */
		Settings(const char* path);

		/**
		 * @brief recalculate settings that depend on another settings
		 *
		 * @details this function should be called every time any setting is changed, it is automatically called in constructor
		 */
		void calculate_settings();

		/// \cond
		std::string cover_art_path, banner_path, database_path;
		
		uint32_t window_width, window_height;
		bool window_maximized;

		int32_t game_tile_width, game_tile_height;
		uint32_t vertical_padding, horizontal_padding; 
	        uint64_t games_per_row; //this settings shouldn't be set manually, it is calculate in calculate_settings
		uint32_t font_size;

		int16_t gamepad_deadzone;
		/// \endcond
};

#endif
