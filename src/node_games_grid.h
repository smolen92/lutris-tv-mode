#ifndef _NODE_GAMES_GRID_H_
#define _NODE_GAMES_GRID_H_

#include <vector>

#include "node_renderer.h"
#include "game.h"
#include "settings.h"
#include "process.h"

class Node_games_grid : public Node {
	public:
		/// inherited from Node
		bool logic(void* data) override;
		/// inherited from Node
		void render() override;
		
		Node_games_grid(Settings* settings, Node_renderer* renderer, std::vector<Game> *games);

		~Node_games_grid();


	private:
		Node_renderer* renderer;

		Settings* settings;
		
		uint64_t current_game;
		
		std::vector<Game> *games;
		
		ProcessHandler process_handler;
};

#endif
