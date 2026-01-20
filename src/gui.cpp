#include "gui.h"

int Gui::gui_init(Settings *settings, std::vector<Game> *games, std::vector<Category> *categories) {
	
	this->settings = settings;

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_GAMEPAD) ) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	if(!TTF_Init()) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	uint64_t window_flags = SDL_WINDOW_RESIZABLE;

	if(this->settings->window_maximized) window_flags |= SDL_WINDOW_MAXIMIZED;

	window = SDL_CreateWindow("Lutris TV Mode", settings->window_width, settings->window_height, window_flags);
	if( window == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}
	
	SDL_SetWindowMinimumSize(this->window, settings->min_window_width, settings->min_window_height);

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

	SDL_IOStream *temp_font = SDL_IOFromConstMem(embedded_font_data, embedded_font_size);
	if(temp_font == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	font = TTF_OpenFontIO(temp_font, true, settings->font_size);
	if(font == nullptr) {
		std::clog << "Error: " << SDL_GetError() << "\n";
		return 1;
	}

	this->games = games;
	this->categories = categories;

	for(uint64_t i=0; i < this->games->size(); i++) {
		this->load_texture(this->games->at(i).slug.c_str());
		this->games->at(i).set_images_indexes(i,i);
	}

	current_game = 0;
	current_category = 0;
	render_all_games = true;

	int32_t gamepad_count;
	SDL_JoystickID *joysticks = SDL_GetGamepads(&gamepad_count);
	
	this->gamepad = SDL_OpenGamepad(joysticks[0]);

	SDL_free(joysticks);
	
	render_categories = false;

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
/// \todo mouse input
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
			if(input.key.scancode == SDL_SCANCODE_C) buttons_pressed[CATEGORIES] = true; 
		}

		if(input.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) buttons_pressed[DOWN] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) buttons_pressed[UP] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) buttons_pressed[RIGHT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT) buttons_pressed[LEFT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) buttons_pressed[RUN] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) buttons_pressed[CATEGORIES] = true;
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

/// \todo category selection - enter/RUN key
void Gui::logic() {

	if( buttons_pressed[UP] ) {
		if(render_categories) {
			if( current_category != 0) current_category -= 1;
		} else {
			if(this->current_game >= this->settings->games_per_row) this->current_game -= this->settings->games_per_row;
		}
	}

	if( buttons_pressed[DOWN] ) {
		if(render_categories) {
			if(current_category != categories->size()-1) current_category += 1;
		}
		else {
			if( (this->current_game + this->settings->games_per_row) < this->games->size() ) this->current_game += this->settings->games_per_row;
		}
	}

	if( buttons_pressed[RIGHT] ) if( this->current_game != this->games->size()-1) this->current_game += 1;
	if( buttons_pressed[LEFT] ) if( this->current_game != 0) this->current_game -= 1;
	if( buttons_pressed[RUN] ) {
		if(render_categories) {
			current_game = 0;
			render_all_games = false;
		} else {
			std::string command = std::string("lutris lutris:rungameid/") + std::to_string(this->games->at(this->current_game).id);
			process_handler.run_process(command.c_str());
		}
	}
	
	if( buttons_pressed[CATEGORIES] ) {
		render_categories = !render_categories;
	}

}

void Gui::render() {

	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);

	uint32_t max_renderer_font_height = 0;

	//initial cover art rect calculation
	SDL_FRect cover_art_rect = {(float)settings->horizontal_padding,(float)settings->vertical_padding,(float)settings->game_tile_width,(float)settings->game_tile_height};

	uint64_t temp_games_start_index = current_game/settings->games_per_row*settings->games_per_row;
	uint64_t temp_games_length;

	(render_all_games) ? temp_games_length = games->size() : temp_games_length = categories->at(current_category).games_indexes.size();

	for(uint64_t i=temp_games_start_index; i < temp_games_length; i++) {
		uint64_t temp_game_index;
		(render_all_games) ? temp_game_index = i : temp_game_index = categories->at(current_category).games_indexes[i];

		//font rendering
		uint32_t rendered_height = render_multi_line_text(cover_art_rect.x, cover_art_rect.y + settings->game_tile_height, games->at(temp_game_index).name.c_str());
		
		if(rendered_height > max_renderer_font_height) max_renderer_font_height = rendered_height;

		//selection box rendering
		if( i == current_game ) {
			SDL_FRect selection_box = {	cover_art_rect.x - settings->horizontal_padding, 
							cover_art_rect.y - settings->vertical_padding,
							cover_art_rect.w + 2*settings->horizontal_padding,
							cover_art_rect.h + 2*settings->vertical_padding};

			selection_box.h += rendered_height;
			SDL_SetRenderDrawColor(renderer, 0,0,255,0x6F);
			SDL_RenderFillRect(renderer, &selection_box);
		}

		//cover art rendering
		if(cover_art[temp_game_index] != nullptr) {
			SDL_RenderTexture(renderer, cover_art[temp_game_index], nullptr, &cover_art_rect);
		}

		//new cover art rect calculation
		cover_art_rect.x += settings->game_tile_width + settings->horizontal_padding;

		if( i%settings->games_per_row == (settings->games_per_row-1)) {
			cover_art_rect.x = settings->horizontal_padding;
			cover_art_rect.y += settings->game_tile_height + settings->vertical_padding + max_renderer_font_height;
			if(cover_art_rect.y > settings->window_height) break;
			max_renderer_font_height = 0;
		}

	}
	
	//category selection box rendering
	if(render_categories) {
		render_categories_menu();
	}
	
	SDL_RenderPresent(renderer);
}

void Gui::render_one_line_of_text(const uint64_t x, const uint64_t y, const char* text, const int32_t area_width) {
	SDL_Surface *text_surface;

	text_surface = TTF_RenderText_Blended(font, text, 0, {255,255,255,255});
	if(text_surface) {
		SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		if( text_texture ) {
			SDL_FRect dst_text_rect = {	(text_texture->w < area_width) ? (float)((area_width-text_texture->w)/2 + x) : (float)x,
							(float)y,
							(text_texture->w > area_width) ? (float)area_width : (float)text_texture->w,
							(float)text_texture->h};
			
			SDL_FRect src_text_rect = {	0.0,
							0.0,
							(text_texture->w > area_width) ? (float)area_width: (float)text_texture->w,
							(float)text_texture->h};


			SDL_RenderTexture(renderer, text_texture, &src_text_rect, &dst_text_rect); 
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
			this->render_one_line_of_text(x,y+offset, string_to_render.c_str(), settings->game_tile_width);
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

	this->render_one_line_of_text(x,y+offset, string_to_render.c_str(), settings->game_tile_width);
	
	return return_height;
}

void Gui::render_categories_menu() {
	SDL_Rect category_viewport = {	(int)settings->category_menu_x,
					(int)settings->category_menu_y,
					(int)settings->category_menu_width,
					(int)settings->category_menu_height};

	if(category_viewport.w > (int)settings->max_category_menu_width) category_viewport.w = settings->max_category_menu_width;
	if(category_viewport.w < (int)settings->min_category_menu_width) category_viewport.w = settings->min_category_menu_width;
	
	SDL_FRect clear_rect = { 	(float)category_viewport.x,
					(float)category_viewport.y,
					(float)category_viewport.w,
					(float)category_viewport.h};
	
	SDL_FRect category_selection = {	(float)category_viewport.x,
						(float)(current_category*settings->font_size),
						(float)category_viewport.w,
						(float)settings->font_size};

	SDL_SetRenderViewport(renderer, &category_viewport);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(renderer, &clear_rect);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderRect(renderer, &clear_rect);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, 0x6F);
	SDL_RenderFillRect(renderer, &category_selection);

	for(uint64_t i=0; i < categories->size(); i++) {
		render_one_line_of_text(0,i*settings->font_size,categories->at(i).name.c_str(),category_viewport.w);
	}
	
	SDL_SetRenderViewport(renderer, nullptr);

}

Gui::~Gui() {
	
	SDL_CloseGamepad(gamepad);

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

