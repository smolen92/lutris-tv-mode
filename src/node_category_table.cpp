#include "node_category_table.h"

Node_category_table::Node_category_table(ProcessHandler* process_handler, Renderer* renderer, Settings* settings, std::vector<Category> *categories, std::vector<bool> *categories_present_ptr) {
	this->renderer = renderer;
	this->settings = settings;
	this->categories = categories;
	this->process_handler = process_handler;

	current_category = 0;
	selection_time = 0;
	global_data_categories_present_ptr = categories_present_ptr;
}

void Node_category_table::logic(Global_data* global_data) {
	if( global_data->buttons_pressed[UP] ) {
		if( current_category != 0) current_category -= 1;
	}

	if( global_data->buttons_pressed[DOWN] ) {
		if(current_category != categories->size()-2) current_category += 1;
	}

	if(global_data->buttons_pressed[SELECTION] ) {
		global_data->categories_present.at(current_category+1) = !global_data->categories_present.at(current_category+1);
		selection_time = process_handler->get_millis();
	}

	if(global_data->buttons_pressed[RUN] ) {
		global_data->action = ACTION_ADD_CATEGORIES;
	}

}

void Node_category_table::render() {
	
	uint64_t category_table_width = settings->window_width/2;
	uint64_t category_table_height = settings->window_height/2;

	renderer->set_viewport((settings->window_width-category_table_width)/2, settings->window_height/4, category_table_width, category_table_height);

	renderer->render_rect(0,0,category_table_width,category_table_height,0,0,0,0xFF);
	renderer->render_rect(0,0,category_table_width,category_table_height,0xFF,0xFF,0xFF,0xFF,false);
	
	uint64_t y = 0;
	uint64_t start;
	uint64_t selection_box_offset;
	
	uint64_t category_count = category_table_height/settings->font_size;

	//+1 to skip predefined Games category
	if( category_count >= categories->size() ) {
		start = 1;
		selection_box_offset = current_category;
	}
	else if ( (current_category+1+category_count) > categories->size() ) {
		start = categories->size()-category_count;
		selection_box_offset = current_category+1-start;
	}
	else {
		start = current_category+1;
		selection_box_offset = 0;
	}
	
	for(uint64_t i=start; (i < categories->size()) && (y<category_table_height); i++) {
		renderer->render_one_line_of_text(0,y,categories->at(i).name.c_str(), category_table_width);
		renderer->render_checkbox(0,y,settings->font_size,settings->font_size,global_data_categories_present_ptr->at(i));
		y += settings->font_size;
	}
	
	if( selection_time + settings->selection_timeout < process_handler->get_millis() ) renderer->render_rect(0,selection_box_offset*settings->font_size, category_table_width, settings->font_size, 0,0,0xFF,0x6F);

	renderer->set_viewport();
}

Node_category_table::~Node_category_table() {
	renderer = nullptr;
	settings = nullptr;
	categories = nullptr;
	process_handler = nullptr;
}

