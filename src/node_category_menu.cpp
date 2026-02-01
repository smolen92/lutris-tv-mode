#include "node_category_menu.h"

void Node_category_menu::logic(void* global_data) {
	bool *buttons_pressed = (bool*)global_data;

	if( buttons_pressed[UP] ) {
		if( current_category != 0) current_category -= 1;
	}

	if( buttons_pressed[DOWN] ) {
		if(current_category != categories->size()-1) current_category += 1;
	}

	if( buttons_pressed[RUN] ) {
		/// \todo set global data
	}

	/// \todo set global data
	if( buttons_pressed[CATEGORIES] ) active = false;
}

void Node_category_menu::render() {
	
	uint64_t category_viewport_x = settings->category_menu_x;
	uint64_t category_viewport_y = settings->category_menu_y;
	uint64_t category_viewport_w = settings->category_menu_width;
	uint64_t category_viewport_h = settings->category_menu_height;

	if(category_viewport_w > settings->max_category_menu_width) category_viewport_w = settings->max_category_menu_width;
	if(category_viewport_w < settings->min_category_menu_width) category_viewport_w = settings->min_category_menu_width;
	
	uint64_t clear_rect_x = category_viewport_x;
	uint64_t clear_rect_y = category_viewport_y;
	uint64_t clear_rect_w = category_viewport_w;
	uint64_t clear_rect_h = category_viewport_h;
	
	uint64_t category_selection_x = category_viewport_x;
	uint64_t category_selection_y = current_category*settings->font_size;
	uint64_t category_selection_w = category_viewport_w;
	uint64_t category_selection_h = settings->font_size;

	renderer->set_viewport(category_viewport_x,category_viewport_y,category_viewport_w, category_viewport_h);

	renderer->render_rect(clear_rect_x, clear_rect_y, clear_rect_w, clear_rect_h, 0,0,0,0xFF);

	renderer->render_rect(clear_rect_x, clear_rect_y, clear_rect_w, clear_rect_h, 0xFF,0xFF,0xFF,0xFF, false);

	renderer->render_rect(category_selection_x, category_selection_y, category_selection_w, category_selection_h, 0, 0, 0xFF, 0x6F);

	for(uint64_t i=0; i < categories->size(); i++) {
		renderer->render_one_line_of_text(0,i*settings->font_size,categories->at(i).name.c_str(),category_viewport_w);
	}

	renderer->set_viewport();

}

Node_category_menu::Node_category_menu(Renderer* renderer, Settings* settings, std::vector<Category> *categories) {
	this->renderer = renderer;
	this->categories = categories;
	this->settings = settings;

	active = false;

	current_category = 0;
}

Node_category_menu::~Node_category_menu() {
	settings = nullptr;
	categories = nullptr;
	renderer = nullptr;
}

