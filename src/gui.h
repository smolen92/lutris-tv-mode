#ifndef _GUI_H_
#define _GUI_H_

#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

/// \cond
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
/// \endcond

/**
 * @brief class for handling gui using sdl
 */
class Gui {
	public:
		/**
		 * @param font poitner to font, that will be used for text
		 */
		int gui_init();
		
		void load_texture(const char* slug);

		/**
		 * @brief checks the input
		 *
		 * @param running boolean that will tell if the main loop should run
		 */
		void input(bool *running);
		
		/**
		 * @brief do the logic only for gui
		 */
		void logic();

		/**
		 * @brief render the scene to window
		 */
		void render();
		
		~Gui();
		
		std::vector<SDL_Texture*> cover_art;
		std::vector<SDL_Texture*> banner;

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};

#endif

