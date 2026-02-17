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
		 */
		Game(const uint64_t id, const char* name, const char* slug);
	
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
		uint64_t cover_art_index, banner_index;
		uint64_t id;
		/// \endcond
};

#endif
