#include "game.h"

Game::Game(const uint64_t id, const char* name, const char* slug) {
	this->id = id;
	
	this->name = name;
	this->slug = slug;

}

void Game::set_images_indexes(uint64_t cover_art_index, uint64_t banner_index) {
	this->cover_art_index = cover_art_index;
	this->banner_index = banner_index;
}

Game::~Game() {
	//nothing to do yet
}
