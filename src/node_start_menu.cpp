#include "node_start_menu.h"

Node_start_menu::Node_start_menu(Renderer* renderer, Settings* settings) {
	this->renderer = renderer;
	
	this->settings = settings;

	current_entry = 0;

	entries_strings.push_back("Restart");
	entries_strings.push_back("Quit TV Mode");
	entries_strings.push_back("Shutdown");
}

void Node_start_menu::logic(Global_data* global_data) {
	if( global_data->buttons_pressed[UP] ) {
		if( current_entry != 0) current_entry -= 1;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if(current_entry != ENTRY_COUNT-1) current_entry += 1;
	}

	if( global_data->buttons_pressed[RUN] ) {
		
		switch(current_entry) {
			case(ENTRY_RESTART_SYSTEM) :	global_data->action = ACTION_RESTART_SYSTEM;
							break;

			case(ENTRY_SHUTDOWN_SYSTEM) :	global_data->action = ACTION_SHUTDOWN_SYSTEM;
							break;

			case(ENTRY_QUIT_TV_MODE) :	global_data->action = ACTION_QUIT_TV_MODE;
							break;
		}
	}

	if( global_data->buttons_pressed[START]) global_data->action = ACTION_REMOVE_NODE;
}

void Node_start_menu::render() {
	uint64_t start_menu_width = settings->window_width*0.3;
	uint64_t start_menu_height = settings->window_height*0.3;

	renderer->set_viewport((settings->window_width-start_menu_width)/2, (settings->window_height-start_menu_height)/2, start_menu_width, start_menu_height);	

	renderer->render_rect(0,0,start_menu_width,start_menu_height,0x00,0x00,0x00,0xFF);
	renderer->render_rect(0,0,start_menu_width,start_menu_height,0xFF,0xFF,0xFF,0xFF,false);

	for(uint64_t i=0; i < ENTRY_COUNT; i++) {
		renderer->render_one_line_of_text(0, settings->font_size*i, entries_strings[i].c_str(),start_menu_width);
	}

	renderer->render_rect(0,settings->font_size*current_entry,start_menu_width,settings->font_size,0, 0, 0xFF, 0x6F);

	renderer->set_viewport();
}

Node_start_menu::~Node_start_menu() {
	renderer = nullptr;
	settings = nullptr;
}
