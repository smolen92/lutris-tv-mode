#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <string>
#include <sqlite3.h>
#include <vector>

/**
 * @brief class for storing info about a game
 */
class Game {
	public:
		/**
		 * @brief this settings should be loaded from database
		 * 
		 * @param id id of the game
		 * @param name name of the game that will be render in gui
		 * @param slug slug of the game, used for loading coverart and banner
		 * @param playtime set playtime 1.0 = 1.h
		 * @param platform set platform of the game
		 * @param runner runner of the game
		 * @param directory set the directory of the game
		 * @param last_played set last played date, seconds since unix epoch
		 */
		Game(const uint64_t id, const char* name, const char* slug, const double playtime,
		     const char* platform, const char* runner, const char* directory, const uint64_t last_played);
	
		/**
		 * @brief indexes of cover art and banner for a game
		 * 
		 * @param cover_art covert art index
		 * @param banner banner index
		 *
		 * @details loading images are handled by gui, this function only save indexes to an array/vectorwhere the image for a game is 
		 */
		void set_images_indexes(uint64_t cover_art, uint64_t banner);
		
		/**
		 * @brief callback function for loading game data from database
		 *
		 */
		static void callback_load_games(void* data_vector, sqlite3_stmt* pre_statement);
		
		/// \cond
		std::string name;
		std::string slug;
		std::string platform;
		std::string runner;
		std::string directory;
		uint64_t cover_art_index, banner_index;
		uint64_t id;
		double playtime;
		uint64_t last_played;
		/// \endcond
};

#endif
