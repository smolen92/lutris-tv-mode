#ifndef _NODE_CATEGORY_TABLE_H_
#define _NODE_CATEGORY_TABLE_H_

#include "renderer.h"
#include "category.h"

/**
 * @brief category table where you can select categories for games
 */
class Node_category_table : public Node {
	public:
		/**
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param categories pointer to category vector
		 * @param categories_present_ptr pointer to global data categories present vector
		 */
		Node_category_table(Renderer* renderer, Settings* settings, std::vector<Category> *categories, std::vector<bool> *categories_present_ptr);
		/// inherited from node
		void logic(Global_data* global_data) override;
		/// inherited from node
		void render() override;
		~Node_category_table();
	private:
		Renderer* renderer;
		Settings* settings;
		std::vector<Category> *categories;

		uint64_t current_category;

		std::vector<bool> *global_data_categories_present_ptr;
};

#endif
