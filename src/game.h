#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <string>
#include <iostream>
#include <SDL3_image/SDL_image.h>

class Game {
	public:
		Game(const uint64_t id, const char* name, const char* slug, SDL_Texture* cover_art, SDL_Texture* banner);
		
		void print();

		~Game();
	private:
		uint64_t id;
		std::string name;
		std::string slug;
		SDL_Texture *cover_art, *banner;
};

#endif
