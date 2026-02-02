#include "node_games_grid.h"

void Node_games_grid::logic(Global_data* global_data) {
	current_game = global_data->current_game;

	if( global_data->buttons_pressed[UP] ) {
		if(current_game >= settings->games_per_row) current_game -= settings->games_per_row;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if( (current_game + settings->games_per_row) < games->size() ) current_game += settings->games_per_row;
	}

	if( global_data->buttons_pressed[RIGHT] ) if( current_game != games->size()-1) current_game += 1;
	if( global_data->buttons_pressed[LEFT] ) if( current_game != 0) current_game -= 1;
	if( global_data->buttons_pressed[RUN] ) {
			std::string command = std::string("lutris lutris:rungameid/") + std::to_string(games->at(current_game).id);
			process_handler.run_process(command.c_str());
	}

	if( global_data->buttons_pressed[CATEGORIES] ) global_data->action = SWITCH_TO_CATEGORY_NODE;

	process_handler.check_and_clean_zombie_processes();

	global_data->current_game = current_game;
}

void Node_games_grid::render() {
	uint32_t max_renderer_font_height = 0;

	//initial cover art rect calculation
	uint64_t cover_art_rect_x = settings->horizontal_padding;
	uint64_t cover_art_rect_y = settings->vertical_padding;
	uint64_t cover_art_rect_w = settings->game_tile_width;
	uint64_t cover_art_rect_h = settings->game_tile_height;

	for(uint64_t i=current_game/settings->games_per_row*settings->games_per_row; i < games->size(); i++) {
		//font rendering
		uint32_t rendered_height = renderer->render_multi_line_text(cover_art_rect_x, cover_art_rect_y + settings->game_tile_height, games->at(i).name.c_str());
		
		if(rendered_height > max_renderer_font_height) max_renderer_font_height = rendered_height;

		//selection box rendering
		if( i == current_game ) {
			renderer->render_rect(	cover_art_rect_x - settings->horizontal_padding, cover_art_rect_y - settings->vertical_padding, 
						cover_art_rect_w + 2*settings->horizontal_padding, cover_art_rect_h + 2*settings->vertical_padding + rendered_height,
						0,0,255,0x6F);
		}

		//cover art rendering
		renderer->render_cover_art(i, cover_art_rect_x, cover_art_rect_y, cover_art_rect_w, cover_art_rect_h); 

		//new cover art rect calculation
		cover_art_rect_x += settings->game_tile_width + settings->horizontal_padding;

		if( i%settings->games_per_row == (settings->games_per_row-1)) {
			cover_art_rect_x = settings->horizontal_padding;
			cover_art_rect_y += settings->game_tile_height + settings->vertical_padding + max_renderer_font_height;
			if(cover_art_rect_y > settings->window_height) break;
			max_renderer_font_height = 0;
		}

	}
}

Node_games_grid::Node_games_grid(Settings* settings, Renderer* renderer, std::vector<Game> *games) {
	this->settings = settings;
	this->renderer = renderer;
	this->games = games;
	
	current_game = 0;

	for(uint64_t i=0; i < games->size(); i++) {
		renderer->load_texture(games->at(i).slug.c_str());
		games->at(i).set_images_indexes(i,i);
	}
}

Node_games_grid::~Node_games_grid() {
	settings = nullptr;
}
