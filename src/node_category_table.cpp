#include "node_category_table.h"

Node_category_table::Node_category_table(Renderer* renderer, Settings* settings, std::vector<Category> *categories) {
	this->renderer = renderer;
	this->settings = settings;
	this->categories = categories;
}

void Node_category_table::logic(Global_data* global_data) {

}

void Node_category_table::render() {
	
	uint64_t category_table_width = settings->window_width/2;

	renderer->set_viewport((settings->window_width-category_table_width)/2, settings->window_height/4, category_table_width, settings->window_height/2);
	
	for(uint64_t i=0; i < categories->size(); i++) {
		renderer->render_one_line_of_text(0,settings->font_size*i,categories->at(i).name.c_str(), category_table_width);
	}

	renderer->set_viewport();
}

Node_category_table::~Node_category_table() {
	renderer = nullptr;
	settings = nullptr;
	categories = nullptr;
}

