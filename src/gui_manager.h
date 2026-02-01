#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include <vector>


#include "renderer.h"
#include "game.h"
#include "category.h"
#include "sql.h"
#include "settings.h"


class Gui_manager {
	public:
		Gui_manager();
		
		~Gui_manager();
		
		bool logic();
		void render();
		
		void load_images();
	private:
		Renderer* renderer;
		std::vector<Node*> nodes;
		Settings* settings;
		std::vector<Game> games;
		std::vector<Category> categories;
		SQL* sql;
		GlobalData global_data;
};

#endif

