#ifndef _NODE_CATEGORY_MENU_H_
#define _NODE_CATEGORY_MENU_H_

#include <vector>

#include "renderer.h"
#include "settings.h"
#include "category.h"

/**
 *
 * @brief handle input and render the category menu
 *
 * \todo switching back to rendering all games after category is selected
 */
class Node_category_menu : public Node {
	public:
		/// inherited from Node
		void logic(void* global_data) override;
		/// inherited from Node
		void render() override;
		
		/**
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param categories pointer to category vector
		 */
		Node_category_menu(Renderer* renderer, Settings* settings, std::vector<Category> *categories);

		~Node_category_menu();
	private:

		Renderer* renderer;

		Settings* settings;

		std::vector<Category> *categories;
		uint64_t current_category;
};

#endif

