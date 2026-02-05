#include "node_start_menu.h"

Node_start_menu::Node_start_menu(Renderer* renderer, Settings* settings) {
	this->renderer = renderer;
	
	this->settings = settings;

	current_entry = 0;
}

void Node_start_menu::logic(Global_data* global_data) {
	if( global_data->buttons_pressed[UP] ) {
		if( current_entry != 0) current_entry -= 1;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if(current_entry != ENTRY_COUNT-1) current_entry += 1;
	}

	if( global_data->buttons_pressed[RUN] ) {
		if(current_entry == 0) global_data->action = RESTART_SYSTEM;
		if(current_entry == 1) global_data->action = SHUTDOWN_SYSTEM;
	}

	if( global_data->buttons_pressed[START]) global_data->action = REMOVE_NODE;
}

void Node_start_menu::render() {
	uint64_t start_menu_width = settings->window_width*0.3;
	uint64_t start_menu_height = settings->window_height*0.3;

	uint64_t start_menu_x = (settings->window_width-start_menu_width)/2;
	uint64_t start_menu_y = (settings->window_height-start_menu_height)/2;

	renderer->render_rect(start_menu_x,start_menu_y,start_menu_width,start_menu_height,0x00,0x00,0x00,0xFF);
	renderer->render_rect(start_menu_x,start_menu_y,start_menu_width,start_menu_height,0xFF,0xFF,0xFF,0xFF,false);

	renderer->render_one_line_of_text(start_menu_x, start_menu_y, "Restart",start_menu_width);
	renderer->render_one_line_of_text(start_menu_x, start_menu_y+settings->font_size, "Shutdown",start_menu_width);

	renderer->render_rect(start_menu_x,start_menu_y+(settings->font_size*current_entry),start_menu_width,settings->font_size,0, 0, 0xFF, 0x6F);
}

Node_start_menu::~Node_start_menu() {
	renderer = nullptr;
	settings = nullptr;
}
