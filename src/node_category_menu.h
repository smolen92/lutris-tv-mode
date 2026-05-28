#ifndef _NODE_CATEGORY_MENU_H_
#define _NODE_CATEGORY_MENU_H_

#include "renderer.h"
#include "category.h"

/**
 *
 * @brief handle input and render the category menu
 *
 */
class Node_category_menu : public Node {
	public:
		/// inherited from Node
		void logic(Global_data* global_data) override;
		/** inherited from Node
		 */
		void render() override;
		
		/**
		 * @param process_handler pointer to process handler
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param categories pointer to category vector
		 * @param current_category current category index
		 */
		Node_category_menu(ProcessHandler* process_handler, Renderer* renderer, Settings* settings, std::vector<Category> *categories, uint64_t current_category);

		~Node_category_menu();
	private:
		ProcessHandler* process_handler;		

		Renderer* renderer;

		Settings* settings;

		std::vector<Category> *categories;
		uint64_t current_category;

		uint64_t selection_time;
};

#endif

