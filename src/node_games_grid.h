#ifndef _NODE_GAMES_GRID_H_
#define _NODE_GAMES_GRID_H_

#include "renderer.h"
#include "game.h"

/**
 * @brief handle input and render the games grid
 * \todo animation for moving the grid
 * \todo status bar controls gfx
 * \todo improve status bar rendering - coordinate to status bar rect
 */
class Node_games_grid : public Node {
	public:
		/// inherited from Node
		void logic(Global_data* global_data) override;
		/// inherited from Node
		void render() override;
		
		/**
		 * @param process_handler pointer to process handler
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param games pointer to game vector
		 */	
		Node_games_grid(ProcessHandler* process_handler, Settings* settings, Renderer* renderer, std::vector<Game> *games);

		~Node_games_grid();

	private:
		ProcessHandler* process_handler;

		Renderer* renderer;

		Settings* settings;
		
		uint64_t current_game;
		
		std::vector<Game> *games;

		uint64_t selection_box_time;
};

#endif
