#include "gui.h"

int Gui::gui_init() {

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	if(!TTF_Init()) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	window = SDL_CreateWindow("Lutris TV Mode", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if( window == NULL) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	return 0;
}

void Gui::load_texture(const char* slug) {
	SDL_Texture* temp_texture;

	temp_texture = IMG_LoadTexture(this->renderer, std::string(std::string("./test-data/coverart/") + std::string(slug) + std::string(".jpg")).c_str());
	cover_art.push_back(temp_texture);

	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " cover art texture\n";

	temp_texture = IMG_LoadTexture(this->renderer, std::string(std::string("./test-data/banners/") + std::string(slug) + std::string(".jpg")).c_str());
	banner.push_back(temp_texture);
	
	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " banner texture\n";

}

void Gui::input(bool *running) {
	SDL_Event input;

	while(SDL_PollEvent(&input)) {
		
		if(input.type == SDL_EVENT_QUIT) {
			*running = false;
		}
	}
		
}

void Gui::logic() {

}

void Gui::render() {
	static uint64_t old_time = 0, index = 0;
	uint64_t current_time = SDL_GetTicks();

	if( current_time - old_time < 1000) return;

	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);

	if(cover_art[index] != nullptr) SDL_RenderTexture(this->renderer, cover_art[index], nullptr, nullptr);

	index++;

	index %= cover_art.size();


	SDL_RenderPresent(renderer);
	old_time = current_time;
}

Gui::~Gui() {
	while(!cover_art.empty()) {
		SDL_DestroyTexture(cover_art.back());
		cover_art.pop_back();
	}
	
	while(!banner.empty()) {
		SDL_DestroyTexture(banner.back());
		banner.pop_back();
	}

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();
	SDL_Quit();
}

