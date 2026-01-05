#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <string>
#include <iostream>

class Game {
	public:
		Game(const uint64_t id, const char* name, const char* slug);
	
		void set_images_indexes(uint64_t cover_art, uint64_t banner);

		~Game();
		
		std::string name;
		std::string slug;
		uint64_t cover_art_index, banner_index;
	private:
		uint64_t id;
};

#endif
