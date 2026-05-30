#ifndef _NODE_CATEGORY_TABLE_H_
#define _NODE_CATEGORY_TABLE_H_

#include "renderer.h"
#include "category.h"

/**
 * @brief category table where you can select categories for games
 * \todo option to create a category
 */
class Node_category_table : public Node {
	public:
		/**
		 * @param process_handler pointer to process handler
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param global_data pointer to global data structure
		 * @param categories pointer to category vector
		 */
		Node_category_table(ProcessHandler* process_handler, Renderer* renderer, Settings* settings, Global_data* global_data, std::vector<Category> *categories);
		/// inherited from node
		void logic() override;
		/// inherited from node
		void render() override;
		~Node_category_table();
	private:
		ProcessHandler* process_handler;
		Renderer* renderer;
		Settings* settings;
		std::vector<Category> *categories;
		Global_data* global_data;
		
		//don't use current_category from global data, always start from 0
		uint64_t current_category;

		uint64_t selection_time;
};

#endif
