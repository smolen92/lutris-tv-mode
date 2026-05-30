#include "node_category_menu.h"

void Node_category_menu::logic() {
	if( global_data->buttons_pressed[UP] ) {
		if( global_data->current_category != 0) global_data->current_category -= 1;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if(global_data->current_category != categories->size()-1) global_data->current_category += 1;
	}

	if( global_data->buttons_pressed[RUN] ) {
		global_data->action = ACTION_READ_DATABASE;
		selection_time = process_handler->get_millis();
	}
	
	if( global_data->buttons_pressed[CATEGORIES] ) global_data->action = ACTION_REMOVE_NODE;

}

void Node_category_menu::render() {
	
	uint64_t category_viewport_x = settings->category_menu_x;
	uint64_t category_viewport_y = settings->category_menu_y;
	uint64_t category_viewport_w = settings->category_menu_width;
	uint64_t category_viewport_h = settings->category_menu_height;

	if(category_viewport_w > settings->max_category_menu_width) category_viewport_w = settings->max_category_menu_width;
	if(category_viewport_w < settings->min_category_menu_width) category_viewport_w = settings->min_category_menu_width;
	
	uint64_t category_selection_x = category_viewport_x;
	uint64_t category_selection_w = category_viewport_w;
	uint64_t category_selection_h = settings->font_size;

	renderer->set_viewport(category_viewport_x,category_viewport_y,category_viewport_w, category_viewport_h);
	//clear viewport
	renderer->render_rect(category_viewport_x, category_viewport_y, category_viewport_w, category_viewport_h, 0,0,0,0xFF);
	
	uint64_t y = 0;
	uint64_t start;
	uint64_t selection_box_offset;
	
	uint64_t category_count = category_viewport_h/settings->font_size;

	if( category_count >= categories->size() ) {
		start = 0;
		selection_box_offset = global_data->current_category;
	}
	else if ( (global_data->current_category+category_count) > categories->size() ) {
		start = categories->size()-category_count;
		selection_box_offset = global_data->current_category-start;
	}
	else {
		start = global_data->current_category;
		selection_box_offset = 0;
	}
	
	//predefined categories separation line
	if(start == 0) renderer->render_rect(category_viewport_x, category_viewport_y, category_viewport_w, settings->font_size, 0xFF,0xFF,0xFF,0xFF, false);
	//border
	renderer->render_rect(category_viewport_x, category_viewport_y, category_viewport_w, category_viewport_h, 0xFF,0xFF,0xFF,0xFF, false);


	if( selection_time + settings->selection_timeout < process_handler->get_millis() ) renderer->render_rect(category_selection_x, selection_box_offset*settings->font_size, category_selection_w, category_selection_h, 0, 0, 0xFF, 0x6F);

	for(uint64_t i=start; (i < categories->size()) && (y<category_viewport_h); i++) {
		renderer->render_one_line_of_text(0,y,categories->at(i).name.c_str(),category_viewport_w);
		y += settings->font_size;
	}

	renderer->set_viewport();

}

Node_category_menu::Node_category_menu(ProcessHandler* process_handler, Renderer* renderer, Settings* settings, Global_data* global_data, std::vector<Category> *categories) {
	this->renderer = renderer;
	this->categories = categories;
	this->settings = settings;

	this->process_handler = process_handler;

	this->global_data = global_data;

	selection_time = 0;
}

Node_category_menu::~Node_category_menu() {
	settings = nullptr;
	categories = nullptr;
	renderer = nullptr;
	process_handler = nullptr;
}

