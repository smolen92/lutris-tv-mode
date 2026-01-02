#include "game.h"

Game::Game(const uint64_t id, const char* name, const char* slug, SDL_Texture* cover_art, SDL_Texture* banner) {
	this->id = id;
	
	this->name = name;
	this->slug = slug;

	this->cover_art = cover_art;
	this->banner = banner;

}

void Game::print() {
	std::cout << "id: " << this->id << " ;name: " << this->name << " ;slug: " << this->slug << "\n";
}

Game::~Game() {
	cover_art = nullptr;
	banner = nullptr;
}
