#include "gui.h"

int Gui::gui_init(Settings *settings, std::vector<Game> *games) {
	

	this->settings = settings;

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	if(!TTF_Init()) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	window = SDL_CreateWindow("Lutris TV Mode", settings->window_width, settings->window_height, 0);
	if( window == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}
	
	font = TTF_OpenFont("Montserrat-Regular.ttf",settings->font_size);
	if(font == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	vertical_offset = 0;
	horizontal_offset = 0;

	this->games = games;
	current_game = 0;

	return 0;
}

void Gui::load_texture(const char* slug) {
	SDL_Texture* temp_texture;

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings->cover_art_path + std::string(slug) + std::string(".jpg")).c_str());
	cover_art.push_back(temp_texture);

	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " cover art texture\n";

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings->banner_path + std::string(slug) + std::string(".jpg")).c_str());
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

			if(input.key.scancode == SDL_SCANCODE_R) {
				if(current_game == 0) continue;

				std::string command = std::string("env LUTRIS_SKIP_INIT=1 lutris lutris:rungameid/") + std::to_string(current_game);
				process_handler.run_process(command.c_str());
			}
		}

	}
		
}

void Gui::logic() {
	process_handler.check_and_clean_zombie_processes();
}

void Gui::render() {

	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);

	SDL_FRect temp = {horizontal_offset,vertical_offset,(float)settings->game_tile_width,(float)settings->game_tile_height};
	for(uint64_t i=0; i < games->size(); i++) {
		
		if(cover_art[games->at(i).cover_art_index] != nullptr) {
			SDL_RenderTexture(this->renderer, cover_art[games->at(i).cover_art_index], nullptr, &temp);
		}

		this->render_text(temp.x, temp.y + settings->game_tile_height, games->at(i).name.c_str());
		
		temp.x += settings->game_tile_width + settings->horizontal_padding;

		if(temp.x >= (settings->window_width - settings->game_tile_width)) {
			temp.x = horizontal_offset;
			temp.y += settings->game_tile_height + settings->vertical_padding + settings->font_size;
		}

	}

	SDL_RenderPresent(renderer);
}

void Gui::render_text(uint64_t x, uint64_t y, const char* text) {
	SDL_Surface *text_surface;

	text_surface = TTF_RenderText_Blended(this->font, text, 0, {255,255,255,255});
	if(text_surface) {
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(this->renderer, text_surface);
		if( text_texture ) {
			SDL_FRect text_rect = {	(text_texture->w < settings->game_tile_width) ? (float)(settings->game_tile_width-text_texture->w)/2 + x: (float)x,
						(float)y,
						(text_texture->w < settings->game_tile_width) ? text_texture->w : (float)settings->game_tile_width, 
						(float)settings->font_size};
			
			SDL_RenderTexture(this->renderer, text_texture, NULL, &text_rect); 
			SDL_DestroyTexture(text_texture);
			text_texture = nullptr;
		}
	}

	SDL_DestroySurface(text_surface);
	text_surface = nullptr;

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
	
	TTF_CloseFont(font);
	font = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	games = nullptr;
	settings = nullptr;

	TTF_Quit();
	SDL_Quit();
}

