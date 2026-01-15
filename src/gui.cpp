#include "gui.h"

int Gui::gui_init(Settings *settings, std::vector<Game> *games) {
	
	this->settings = settings;

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_GAMEPAD) ) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	if(!TTF_Init()) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	window = SDL_CreateWindow("Lutris TV Mode", settings->window_width, settings->window_height, SDL_WINDOW_RESIZABLE);
	if( window == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}
	
	SDL_SetWindowMinimumSize(this->window, MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

	font = TTF_OpenFont("Montserrat-Regular.ttf",settings->font_size);
	if(font == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	this->games = games;

	current_game = 0;
	
	int32_t gamepad_count;
	SDL_JoystickID *joysticks = SDL_GetGamepads(&gamepad_count);
	
	this->gamepad = SDL_OpenGamepad(joysticks[0]);

	SDL_free(joysticks);

	return 0;
}

/// \todo don't push nullptr to vec
void Gui::load_texture(const char* slug) {
	SDL_Texture* temp_texture;

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings->cover_art_path + std::string(slug) + std::string(".jpg")).c_str());
	cover_art.push_back(temp_texture);

	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " cover art texture\n";

	temp_texture = IMG_LoadTexture(this->renderer, std::string(settings->banner_path + std::string(slug) + std::string(".jpg")).c_str());
	banner.push_back(temp_texture);
	
	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " banner texture\n";

}

/// \bug doesn't detect if gamepad button is down
/// \bug gamepad axis too sensitive, take input even when returning to center
void Gui::input(bool *running) {
	SDL_Event input;

	for(uint8_t i=0; i < TOTAL_BUTTONS; i++) {
		buttons_pressed[i] = false;
	}

	while(SDL_PollEvent(&input)) {
		
		if(input.type == SDL_EVENT_QUIT) {
			*running = false;
		}

		if(input.type == SDL_EVENT_KEY_DOWN) {
			if(input.key.scancode == SDL_SCANCODE_DOWN) buttons_pressed[DOWN] = true;
			if(input.key.scancode == SDL_SCANCODE_UP) buttons_pressed[UP] = true;
			if(input.key.scancode == SDL_SCANCODE_RIGHT) buttons_pressed[RIGHT] = true;
			if(input.key.scancode == SDL_SCANCODE_LEFT) buttons_pressed[LEFT] = true;
			if(input.key.scancode == SDL_SCANCODE_RETURN) buttons_pressed[RUN] = true;
		}

		if(input.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) buttons_pressed[DOWN] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) buttons_pressed[UP] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) buttons_pressed[RIGHT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT) buttons_pressed[LEFT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) buttons_pressed[RUN] = true;
		}

		if(input.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
			
			if(input.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) {
				if(input.gaxis.value < -this->settings->gamepad_deadzone) buttons_pressed[LEFT] = true;
				if(input.gaxis.value > this->settings->gamepad_deadzone) buttons_pressed[RIGHT] = true;
			}
			
			if(input.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY) {
				if(input.gaxis.value < -this->settings->gamepad_deadzone) buttons_pressed[UP] = true;
				if(input.gaxis.value > this->settings->gamepad_deadzone) buttons_pressed[DOWN] = true;
			}

		}

		if( input.type == SDL_EVENT_WINDOW_RESIZED ) {
			settings->window_width = input.window.data1;
			settings->window_height = input.window.data2;
			settings->calculate_settings();
		}

	}

}

void Gui::logic() {

	if( buttons_pressed[UP] ) if(this->current_game >= this->settings->games_per_row) this->current_game -= this->settings->games_per_row;
	if( buttons_pressed[DOWN] ) if( (this->current_game + this->settings->games_per_row) < this->games->size() ) this->current_game += this->settings->games_per_row;
	if( buttons_pressed[RIGHT] ) if( this->current_game != this->games->size()-1) this->current_game += 1;
	if( buttons_pressed[LEFT] ) if( this->current_game != 0) this->current_game -= 1;
	if( buttons_pressed[RUN] ) { 
		std::string command = std::string("lutris lutris:rungameid/") + std::to_string(this->games->at(this->current_game).id);
		process_handler.run_process(command.c_str());
	}

}

void Gui::render() {

	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);

	uint32_t max_renderer_font_height = 0;

	//initial cover art rect calculation
	SDL_FRect cover_art_rect = {(float)this->settings->horizontal_padding,(float)this->settings->vertical_padding,(float)settings->game_tile_width,(float)settings->game_tile_height};

	for(uint64_t i=(this->current_game/this->settings->games_per_row*this->settings->games_per_row); i < games->size(); i++) {
		SDL_FRect selection_box;

		//selection box rect calculation
		if( i == current_game) {
			selection_box = {cover_art_rect.x - this->settings->horizontal_padding, cover_art_rect.y - this->settings->vertical_padding, cover_art_rect.w + 2*this->settings->horizontal_padding, cover_art_rect.h + 2*this->settings->vertical_padding};
		}
		
		//font rendering
		uint32_t rendered_height = this->render_multi_line_text(cover_art_rect.x, cover_art_rect.y + settings->game_tile_height, games->at(i).name.c_str());
		
		if(rendered_height > max_renderer_font_height) max_renderer_font_height = rendered_height;

		//selection box rendering
		if( i == this->current_game ) {
			selection_box.h += rendered_height;
			SDL_SetRenderDrawColor(this->renderer, 0,0,255,0x6F);
			SDL_RenderFillRect(this->renderer, &selection_box);
		}

		//cover art rendering
		if(cover_art[games->at(i).cover_art_index] != nullptr) {
			SDL_RenderTexture(this->renderer, cover_art[games->at(i).cover_art_index], nullptr, &cover_art_rect);
		}

		//new cover art rect calculation
		cover_art_rect.x += settings->game_tile_width + settings->horizontal_padding;

		if( i%this->settings->games_per_row == (this->settings->games_per_row-1)) {
			cover_art_rect.x = this->settings->horizontal_padding;
			cover_art_rect.y += settings->game_tile_height + settings->vertical_padding + max_renderer_font_height;
			if(cover_art_rect.y > this->settings->window_height) break;
			max_renderer_font_height = 0;
		}

	}

	SDL_RenderPresent(this->renderer);
}

void Gui::render_one_line_of_text(uint64_t x, uint64_t y, const char* text) {
	SDL_Surface *text_surface;

	text_surface = TTF_RenderText_Blended(this->font, text, 0, {255,255,255,255});
	if(text_surface) {
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(this->renderer, text_surface);
		if( text_texture ) {
			SDL_FRect dst_text_rect = {	(text_texture->w < settings->game_tile_width) ? (float)((settings->game_tile_width-text_texture->w)/2 + x) : (float)x,
							(float)y,
							(text_texture->w > this->settings->game_tile_width) ? (float)this->settings->game_tile_width : (float)text_texture->w,
							(float)text_texture->h};
			
			SDL_FRect src_text_rect = {	0.0,
							0.0,
							(text_texture->w > this->settings->game_tile_width) ? (float)this->settings->game_tile_width : (float)text_texture->w,
							(float)text_texture->h};


			SDL_RenderTexture(this->renderer, text_texture, &src_text_rect, &dst_text_rect); 
			SDL_DestroyTexture(text_texture);
			text_texture = nullptr;
		}
	}

	SDL_DestroySurface(text_surface);
	text_surface = nullptr;

}

uint32_t Gui::render_multi_line_text(uint64_t x, uint64_t y, const char *text) {

	std::stringstream temp_string_stream(text);
	std::string temp_element;
	std::vector<std::string> elements;
	
	while(getline(temp_string_stream,temp_element,' ')) {
		elements.push_back(temp_element);
	}
	
	std::string string_to_render = elements[0] + " ";
	int32_t total_string_width;
	TTF_GetStringSize(this->font, string_to_render.c_str(), 0, &total_string_width, nullptr);
	uint32_t return_height = this->settings->font_size;
	uint32_t offset = 0;

	for(uint64_t i=1; i < elements.size(); i++) {
		int32_t current_string_width;
		if(i != elements.size() - 1) elements[i] += " ";

		TTF_GetStringSize(this->font, elements[i].c_str(), 0, &current_string_width, nullptr);

		if(total_string_width + current_string_width > this->settings->game_tile_width) {
			this->render_one_line_of_text(x,y+offset, string_to_render.c_str());
			string_to_render = elements[i];
			TTF_GetStringSize(this->font, string_to_render.c_str(), 0, &total_string_width, nullptr);
			return_height += this->settings->font_size;
			offset += this->settings->font_size;
		}
		else {
			string_to_render += elements[i];
			total_string_width += current_string_width;
		}
	}

	this->render_one_line_of_text(x,y+offset, string_to_render.c_str());
	
	return return_height;
}

Gui::~Gui() {
	
	SDL_CloseGamepad(this->gamepad);

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

