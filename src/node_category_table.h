#ifndef _NODE_CATEGORY_TABLE_H_
#define _NODE_CATEGORY_TABLE_H_

#include "renderer.h"
#include "category.h"

class Node_category_table : public Node {
	public:
		Node_category_table(Renderer* renderer, Settings* settings, std::vector<Category> *categories);
		void logic(Global_data* global_data) override;
		void render() override;
		~Node_category_table();
	private:
		Renderer* renderer;
		Settings* settings;
		std::vector<Category> *categories;

		uint64_t current_category;
		std::vector<bool> categories_to_add;
		uint64_t selection_box_offset;
};

#endif
