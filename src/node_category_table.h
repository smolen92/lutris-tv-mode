#ifndef _NODE_CATEGORY_TABLE_H_
#define _NODE_CATEGORY_TABLE_H_

#include "renderer.h"
#include "category.h"

/**
 * @brief category table where you can select categories for games
 * \todo read in what categories the game is
 */
class Node_category_table : public Node {
	public:
		/**
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param categories pointer to category vector
		 */
		Node_category_table(Renderer* renderer, Settings* settings, std::vector<Category> *categories);
		/// inherited from node
		void logic(Global_data* global_data) override;
		///inherited from node
		void render() override;
		~Node_category_table();
	private:
		Renderer* renderer;
		Settings* settings;
		std::vector<Category> *categories;

		uint64_t current_category;
		std::vector<bool> categories_to_add;
};

#endif
