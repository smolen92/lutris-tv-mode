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
		global_data->action = ACTION_RUN_GAME;
		selection_box_time = global_data->millis;
	}
	if( global_data->buttons_pressed[FAVORITE] ) global_data->action = ACTION_ADD_REMOVE_FAVORITE;
	if( global_data->buttons_pressed[CATEGORIES] ) global_data->action = ACTION_SWITCH_TO_CATEGORY_NODE;
	if( global_data->buttons_pressed[SELECTION] ) global_data->action = ACTION_SWITCH_TO_CAT_TABLE_NODE;
	
	current_millis = global_data->millis;

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
		if( ( i == current_game ) && (selection_box_time + settings->selection_timeout < current_millis) ) {
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

	//status bar
	time_t epoch_time = (time_t)games->at(current_game).last_played;
	char human_readable[12];
	strncpy(human_readable, ctime(&epoch_time)+8, 3);
	strncpy(human_readable+3, ctime(&epoch_time)+4, 4);
	strncpy(human_readable+7, ctime(&epoch_time)+20,4);
	human_readable[11] = '\0';
	
	renderer->render_rect( 0, settings->window_height-settings->font_size, settings->window_width/2, settings->font_size,
				0, 0, 0, 0xFF, true);
	uint64_t temp_current_game_playtime_hour = (uint64_t) games->at(current_game).playtime;
	uint64_t temp_current_game_playtime_minute = (uint64_t)( (games->at(current_game).playtime - temp_current_game_playtime_hour) * 60);
	renderer->render_one_line_of_text(settings->horizontal_padding, settings->window_height-settings->font_size,
		       				std::string("Playtime: " + std::to_string(temp_current_game_playtime_hour) + "h " + std::to_string(temp_current_game_playtime_minute) + "m" +
						"   Last Played: " + human_readable + 
						"   Runner: " + games->at(current_game).runner).c_str(),
					       	settings->window_width/2); 
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

	selection_box_time = 0;
}

Node_games_grid::~Node_games_grid() {
	settings = nullptr;
}
