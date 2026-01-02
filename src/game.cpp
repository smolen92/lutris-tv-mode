#include "game.h"

Game::Game(const uint64_t id, const char* name, const char* slug ) {
	this->id = id;
	
	std::strcpy(this->name, name);

	std::strcpy(this->slug, slug);
}

void Game::print() {
	std::cout << "id: " << this->id << " ;name: " << this->name << " ;slug: " << this->slug << "\n";
}

Game::~Game() {
	//nothing to do yet
}
