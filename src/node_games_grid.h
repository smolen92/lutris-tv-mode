#ifndef _NODE_GAMES_GRID_H_
#define _NODE_GAMES_GRID_H_

#include <vector>

#include "node_renderer.h"
#include "game.h"
#include "settings.h"
#include "process.h"

/**
 * @brief handle input and render the games grid
 */
class Node_games_grid : public Node {
	public:
		/// inherited from Node
		void logic(void* global_data) override;
		/// inherited from Node
		void render() override;
		
		/**
		 * @param renderer pointer to renderer node
		 * @param settings pointer to settings
		 * @param games pointer to game vector
		 */	
		Node_games_grid(Settings* settings, Renderer* renderer, std::vector<Game> *games);

		~Node_games_grid();


	private:
		Renderer* renderer;

		Settings* settings;
		
		uint64_t current_game;
		
		std::vector<Game> *games;
		
		ProcessHandler process_handler;
};

#endif
