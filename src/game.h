#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include <cstring>
#include <iostream>

class Game {
	public:
		Game(const uint64_t id, const char* name, const char* slug);
		
		void print();

		~Game();
	private:
		uint64_t id;
		char name[1000];
		char slug[1000];
};

#endif
