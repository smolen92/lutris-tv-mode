#include "gui.h"

int Gui::gui_init() {
	
	settings.load_settings(nullptr);

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	if(!TTF_Init()) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	window = SDL_CreateWindow("Lutris TV Mode", settings.window_width, settings.window_height, 0);
	if( window == NULL) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	vertical_offset = 0;
	horizontal_offset = 0;

	return 0;
}

void Gui::load_texture(const char* slug) {
	SDL_Texture* temp_texture;

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings.cover_art_path + std::string(slug) + std::string(".jpg")).c_str());
	cover_art.push_back(temp_texture);

	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " cover art texture\n";

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings.banner_path + std::string(slug) + std::string(".jpg")).c_str());
	banner.push_back(temp_texture);
	
	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " banner texture\n";

}

void Gui::input(bool *running) {
	SDL_Event input;

	while(SDL_PollEvent(&input)) {
		
		if(input.type == SDL_EVENT_QUIT) {
			*running = false;
		}

		if(input.type == SDL_EVENT_KEY_DOWN) {
			if(input.key.scancode == SDL_SCANCODE_DOWN) {
				vertical_offset -= 5;
			}

			if(input.key.scancode == SDL_SCANCODE_UP) {
				vertical_offset += 5;
			}

			if(input.key.scancode == SDL_SCANCODE_RIGHT) {
				horizontal_offset += 5;
			}

			if(input.key.scancode == SDL_SCANCODE_LEFT) {
				horizontal_offset -= 5;
			}
		}

	}
		
}

void Gui::logic() {

}

void Gui::render() {

	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);

	SDL_FRect temp = {horizontal_offset,vertical_offset,(float)settings.game_tile_width,(float)settings.game_tile_height};
	for(uint64_t i=0; i < cover_art.size(); i++) {
		
		if(cover_art[i] != nullptr) SDL_RenderTexture(this->renderer, cover_art[i], nullptr, &temp);
		
		temp.x += settings.game_tile_width + settings.horizontal_padding;

		if(temp.x >= (settings.window_width - settings.game_tile_width)) {
			temp.x = horizontal_offset;
			temp.y += settings.game_tile_height + settings.vertical_padding;
		}

	}

	SDL_RenderPresent(renderer);
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

