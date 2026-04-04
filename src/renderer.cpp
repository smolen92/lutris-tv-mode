#include "renderer.h"

/// \bug doesn't detect if gamepad button is down
/// \bug gamepad axis too sensitive, take input even when returning to center
/// \todo mouse input
bool Renderer::check_input(bool* buttons_pressed) {
	SDL_Event input;

	for(uint8_t i=0; i < TOTAL_BUTTONS; i++) {
		buttons_pressed[i] = false;
	}

	while(SDL_PollEvent(&input)) {
		
		if(input.type == SDL_EVENT_QUIT) {
			return false;
		}

		if(input.type == SDL_EVENT_KEY_DOWN) {
			if(input.key.scancode == SDL_SCANCODE_DOWN) buttons_pressed[DOWN] = true;
			if(input.key.scancode == SDL_SCANCODE_UP) buttons_pressed[UP] = true;
			if(input.key.scancode == SDL_SCANCODE_RIGHT) buttons_pressed[RIGHT] = true;
			if(input.key.scancode == SDL_SCANCODE_LEFT) buttons_pressed[LEFT] = true;
			if(input.key.scancode == SDL_SCANCODE_RETURN) buttons_pressed[RUN] = true;
			if(input.key.scancode == SDL_SCANCODE_C) buttons_pressed[CATEGORIES] = true;
			if(input.key.scancode == SDL_SCANCODE_F) buttons_pressed[FAVORITE] = true;
			if(input.key.scancode == SDL_SCANCODE_ESCAPE) buttons_pressed[START] = true;
			if(input.key.scancode == SDL_SCANCODE_SPACE) buttons_pressed[SELECTION] = true;
		}

		if(input.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN) buttons_pressed[DOWN] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP) buttons_pressed[UP] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) buttons_pressed[RIGHT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT) buttons_pressed[LEFT] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) buttons_pressed[RUN] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_LEFT_SHOULDER) buttons_pressed[CATEGORIES] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_NORTH) buttons_pressed[FAVORITE] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_START) buttons_pressed[START] = true;
			if(input.gbutton.button == SDL_GAMEPAD_BUTTON_WEST) buttons_pressed[SELECTION] = true;
		}

		if(input.type == SDL_EVENT_GAMEPAD_AXIS_MOTION) {
			
			if(input.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) {
				if(input.gaxis.value < -settings->gamepad_deadzone) buttons_pressed[LEFT] = true;
				if(input.gaxis.value > settings->gamepad_deadzone) buttons_pressed[RIGHT] = true;
			}
			
			if(input.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY) {
				if(input.gaxis.value < -settings->gamepad_deadzone) buttons_pressed[UP] = true;
				if(input.gaxis.value > settings->gamepad_deadzone) buttons_pressed[DOWN] = true;
			}

		}

		if( input.type == SDL_EVENT_WINDOW_RESIZED ) {
			settings->window_width = input.window.data1;
			settings->window_height = input.window.data2;
			settings->calculate_settings();
		}

	}
	
	return true;
}

void Renderer::clear_screen() {
	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);
}

void Renderer::draw_screen() {
	SDL_RenderPresent(renderer);
}

Renderer::Renderer(Settings* settings) {
	this->settings = settings;

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_GAMEPAD) ) {
		throw std::runtime_error(SDL_GetError());
	}

	if(!TTF_Init()) {
		throw std::runtime_error(SDL_GetError());
	}

	uint64_t window_flags = SDL_WINDOW_RESIZABLE;

	if(this->settings->window_maximized) window_flags |= SDL_WINDOW_MAXIMIZED;

	window = SDL_CreateWindow("Lutris TV Mode", settings->window_width, settings->window_height, window_flags);
	if( window == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}
	
	SDL_SetWindowMinimumSize(this->window, settings->min_window_width, settings->min_window_height);

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);

	SDL_IOStream *temp_font = SDL_IOFromConstMem(embedded_font_data, embedded_font_size);
	if(temp_font == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	font = TTF_OpenFontIO(temp_font, true, settings->font_size);
	if(font == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	checkbox[0] = IMG_LoadTexture(renderer, "./assets/check_square_grey.png");
	if( checkbox[0] == nullptr) std::clog << "failed to load checkbox\n";
	checkbox[1] = IMG_LoadTexture(renderer, "./assets/check_square_grey_checkmark.png");
	if( checkbox[1] == nullptr) std::clog << "failed to load checkbox\n";


	int32_t gamepad_count;
	SDL_JoystickID *joysticks = SDL_GetGamepads(&gamepad_count);
	
	gamepad = SDL_OpenGamepad(joysticks[0]);

	SDL_free(joysticks);
}
		
Renderer::~Renderer() {
	SDL_CloseGamepad(gamepad);

	clear_images();
	
	TTF_CloseFont(font);
	font = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	TTF_Quit();
	SDL_Quit();

	settings = nullptr;
}
		
void Renderer::load_texture(const char* slug) {
	SDL_Texture* temp_texture;

	temp_texture = IMG_LoadTexture(renderer, std::string(settings->cover_art_path + std::string(slug) + std::string(".jpg")).c_str());
	cover_art.push_back(temp_texture);

	if(temp_texture == nullptr) std::clog << "failed to load " << slug << " cover art texture\n";
}

void Renderer::render_one_line_of_text(const uint64_t x, const uint64_t y, const char* text, const int32_t area_width) {
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

uint32_t Renderer::render_multi_line_text(uint64_t x, uint64_t y, const char* text) {
	std::stringstream temp_string_stream(text);
	std::string temp_element;
	std::vector<std::string> elements;
	
	while(getline(temp_string_stream,temp_element,' ')) {
		elements.push_back(temp_element);
	}
	
	std::string string_to_render = elements[0] + " ";
	int32_t total_string_width;
	TTF_GetStringSize(font, string_to_render.c_str(), 0, &total_string_width, nullptr);
	uint32_t return_height = settings->font_size;
	uint32_t offset = 0;

	for(uint64_t i=1; i < elements.size(); i++) {
		int32_t current_string_width;
		if(i != elements.size() - 1) elements[i] += " ";

		TTF_GetStringSize(font, elements[i].c_str(), 0, &current_string_width, nullptr);

		if(total_string_width + current_string_width > settings->game_tile_width) {
			this->render_one_line_of_text(x,y+offset, string_to_render.c_str(), settings->game_tile_width);
			string_to_render = elements[i];
			TTF_GetStringSize(font, string_to_render.c_str(), 0, &total_string_width, nullptr);
			return_height += settings->font_size;
			offset += settings->font_size;
		}
		else {
			string_to_render += elements[i];
			total_string_width += current_string_width;
		}
	}

	this->render_one_line_of_text(x,y+offset, string_to_render.c_str(), settings->game_tile_width);
	
	return return_height;
}


void Renderer::clear_images() {
	while(!cover_art.empty()) {
		SDL_DestroyTexture(cover_art.back());
		cover_art.back() = nullptr;
		cover_art.pop_back();
	}
}

void Renderer::render_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t r, uint64_t g, uint64_t b, uint64_t a, bool filled) {
	SDL_FRect temp = {(float)x, (float)y, (float) w, (float) h};

	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	(filled) ? SDL_RenderFillRect(renderer, &temp) : SDL_RenderRect(renderer, &temp);
}
		
void Renderer::render_cover_art(uint64_t index, uint64_t x, uint64_t y, uint64_t w, uint64_t h) {
	if(cover_art.at(index) == nullptr) return;

	SDL_FRect temp;

	if(cover_art.at(index)->w >= cover_art.at(index)->h) {
		temp.x = (float)x;
		temp.w = (float)w;
		
		temp.h = (float)((int)((float)w/(float)cover_art.at(index)->w*(float)cover_art.at(index)->h));
		temp.y = (float)(y + ((h - (int)temp.h) >> 1));
	}
	else {
		temp.y = (float)y;
		temp.h = (float)h;

		temp.w = (float)((int)((float)h/(float)cover_art.at(index)->h*(float)cover_art.at(index)->w));
		temp.x = (float)(x + ((w - (int)temp.w) >> 1));
	}

	SDL_RenderTexture(renderer, cover_art.at(index), nullptr, &temp);
}

void Renderer::set_viewport(int32_t x, int32_t y, int32_t w, int32_t h) {
	SDL_Rect temp = {x,y,w,h};

	( (w == 0) || (h == 0) ) ? SDL_SetRenderViewport(renderer, nullptr) : SDL_SetRenderViewport(renderer, &temp);
}

void Renderer::render_checkbox(uint64_t x, uint64_t y, uint64_t w, uint64_t h, bool checked) {
	SDL_FRect temp = {(float)x, (float)y, (float)w, (float)h};

	SDL_RenderTexture(renderer, checkbox[checked], nullptr, &temp);
}
