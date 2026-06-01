#include "node_games_grid.h"

void Node_games_grid::logic() {
	if( global_data->buttons_pressed[UP] ) {
		if(global_data->current_game >= settings->games_per_row) global_data->current_game -= settings->games_per_row;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if( (global_data->current_game + settings->games_per_row) < games->size() ) global_data->current_game += settings->games_per_row;
	}

	if( global_data->buttons_pressed[RIGHT] ) if( global_data->current_game != games->size()-1) global_data->current_game += 1;
	if( global_data->buttons_pressed[LEFT] ) if( global_data->current_game != 0) global_data->current_game -= 1;
	
	if( global_data->buttons_pressed[RUN] ) {
		global_data->action = ACTION_RUN_GAME;
		selection_box_time = process_handler->get_millis();
	}
	if( global_data->buttons_pressed[FAVORITE] ) global_data->action = ACTION_ADD_REMOVE_FAVORITE;  
	if( global_data->buttons_pressed[CATEGORIES] ) global_data->action = ACTION_SWITCH_TO_CATEGORY_NODE; 
	if( global_data->buttons_pressed[SELECTION] ) global_data->action = ACTION_SWITCH_TO_CAT_TABLE_NODE;
	if( global_data->buttons_pressed[KILL] ) global_data->action = ACTION_KILL_PROCESS;
}

void Node_games_grid::render() {

	uint32_t max_renderer_font_height = 0;

	//initial cover art rect calculation
	int64_t cover_art_rect_x = settings->horizontal_padding;
	int64_t cover_art_rect_y = (int64_t)settings->vertical_padding - move_offset;
	int64_t cover_art_rect_w = settings->game_tile_width;
	int64_t cover_art_rect_h = settings->game_tile_height;

	for(uint64_t i=global_data->current_game/settings->games_per_row*settings->games_per_row; i < games->size(); i++) {
		//font rendering
		uint32_t rendered_height = renderer->render_multi_line_text(cover_art_rect_x, cover_art_rect_y + settings->game_tile_height, games->at(i).name.c_str());
		
		if(rendered_height > max_renderer_font_height) max_renderer_font_height = rendered_height;

		//selection box rendering
		if( ( i == global_data->current_game ) && (selection_box_time + settings->selection_timeout < process_handler->get_millis()) ) {
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
	uint64_t status_bar_rect_x = 0; 
	uint64_t status_bar_rect_y = settings->window_height-settings->font_size;
	uint64_t status_bar_rect_w = settings->window_width/2;
	uint64_t status_bar_rect_h = settings->font_size;

	renderer->render_rect( status_bar_rect_x, status_bar_rect_y, settings->window_width, status_bar_rect_h, 0, 0, 0, 0xFF, true);
	
	uint64_t temp_text_offset;

	if(!games->empty()) {
		uint64_t temp_current_game_playtime_hour = (uint64_t) games->at(global_data->current_game).playtime;
		uint64_t temp_current_game_playtime_minute = (uint64_t)( (games->at(global_data->current_game).playtime - temp_current_game_playtime_hour) * 60);

		std::string temp_status_bar_text = "Playtime: " + std::to_string(temp_current_game_playtime_hour) + "h " + std::to_string(temp_current_game_playtime_minute) + "m ";
		if(games->at(global_data->current_game).last_played != 0) {
			time_t epoch_time = (time_t)games->at(global_data->current_game).last_played;
			char human_readable[12];
			strncpy(human_readable, ctime(&epoch_time)+8, 3);
			strncpy(human_readable+3, ctime(&epoch_time)+4, 4);
			strncpy(human_readable+7, ctime(&epoch_time)+20,4);
			human_readable[11] = '\0';

			temp_status_bar_text += "Last Played: " + std::string(human_readable) + " ";
		}

		temp_status_bar_text += "Runner: " + games->at(global_data->current_game).runner;

		renderer->render_one_line_of_text(settings->horizontal_padding, status_bar_rect_y, temp_status_bar_text.c_str(), 0);
	
		char play_stop_text[5];
		if( process_handler->is_process_running(global_data->current_game) ) {
			strcpy(play_stop_text, "Stop");
		}
		else {
			strcpy(play_stop_text, "Play");
		}

		renderer->render_asset(GFX_A_BUTTON, status_bar_rect_w, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
		temp_text_offset = renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h, status_bar_rect_y, play_stop_text, 0);
		
		temp_text_offset += status_bar_rect_h + settings->vertical_padding;
		
		if( process_handler->is_process_running(global_data->current_game) ) {
			renderer->render_asset(GFX_B_BUTTON, status_bar_rect_w + temp_text_offset, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
			temp_text_offset += renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h + temp_text_offset, status_bar_rect_y, "Kill", 0);
			temp_text_offset += status_bar_rect_h + settings->vertical_padding;
		}

		renderer->render_asset(GFX_X_BUTTON, status_bar_rect_w + temp_text_offset, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
		temp_text_offset += renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h + temp_text_offset, status_bar_rect_y, "Add Category", 0);
	
		temp_text_offset += status_bar_rect_h + settings->vertical_padding;
		
		renderer->render_asset(GFX_Y_BUTTON, status_bar_rect_w + temp_text_offset, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
		temp_text_offset += renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h + temp_text_offset, status_bar_rect_y, "Add/Remove Favorite", 0);

		temp_text_offset += status_bar_rect_h + settings->vertical_padding;
	}

	//status bar controls
	renderer->render_asset(GFX_LB_BUTTON, status_bar_rect_w + temp_text_offset, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
	temp_text_offset += renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h + temp_text_offset, status_bar_rect_y, "Select Category", 0);

	temp_text_offset += status_bar_rect_h + settings->vertical_padding;

	renderer->render_asset(GFX_START_BUTTON, status_bar_rect_w + temp_text_offset, status_bar_rect_y, status_bar_rect_h, status_bar_rect_h);
	temp_text_offset += renderer->render_one_line_of_text(status_bar_rect_w + status_bar_rect_h + temp_text_offset, status_bar_rect_y, "Main Menu", 0);

}

Node_games_grid::Node_games_grid(ProcessHandler* process_handler, Settings* settings, Renderer* renderer, Global_data* global_data, std::vector<Game> *games) {
	this->settings = settings;
	this->renderer = renderer;
	this->games = games;
	this->process_handler = process_handler;
	this->global_data = global_data;

	for(uint64_t i=0; i < games->size(); i++) {
		renderer->load_texture(games->at(i).slug.c_str());
		games->at(i).set_images_indexes(i,i);
	}

	selection_box_time = 0;
	move_offset = 0;
}

Node_games_grid::~Node_games_grid() {
	settings = nullptr;
}
