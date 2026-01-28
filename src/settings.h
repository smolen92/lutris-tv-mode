#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <cstdint>
#include <string>
#include <fstream>

/**
 * @brief class that stores settings for the app
 * \todo default path should be user home dir + suffix
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
		uint32_t min_window_width, min_window_height;
		bool window_maximized;

		int32_t game_tile_width, game_tile_height;
		uint32_t vertical_padding, horizontal_padding; 
		uint32_t font_size;

		int16_t gamepad_deadzone;

		uint32_t category_menu_x, category_menu_y;

		//these settings shouldn't be set manually, it is calculate in calculate_settings
	        uint64_t games_per_row;
		uint32_t category_menu_width, category_menu_height, max_category_menu_width, min_category_menu_width;
		/// \endcond
	private:
		/**
		 * @brief load default settings
		 */
		void load_defaults();
};

/**
 *
 * \defgroup settings_file Settings File
 *
 * \ingroup settings_file
 *  
 *  - based on ini file structure\n
 *  - comments are lines that begin with ; or # symbol\n
 *  - the rest lines are key=value pair devided with = sign e.g. window_width=800, : as divider are not supported\n
 *  - white space are not ignored\n
 *  - lines without = sign are ignored\n
 *  - if there are key with multiple entries, the last one is used
 */

#endif
