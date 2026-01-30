#ifndef _NODE_CATEGORY_MENU_H_
#define _NODE_CATEGORY_MENU_H_

#include <vector>

#include "node_renderer.h"
#include "settings.h"
#include "category.h"

class Node_category_menu : public Node {
	public:
		/// inherited from Node
		bool logic(void* data) override;
		/// inherited from Node
		void render() override;

		Node_category_menu(Node_renderer* renderer, Settings* settings, std::vector<Category> *categories);

		~Node_category_menu();
	private:

		Node_renderer* renderer;

		Settings* settings;

		std::vector<Category> *categories;
		uint64_t current_category;
};

#endif

