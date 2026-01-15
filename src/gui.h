#ifndef _GUI_H_
#define _GUI_H_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "settings.h"
#include "process.h"
#include "game.h"


enum Buttons {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RUN,
	TOTAL_BUTTONS
};

/**
 * @brief class for handling gui using sdl
 * \todo read font from data
 * \todo fullscreen and resizeable window
 */
class Gui {
	public:
		/**
		 * @brief initialize the gui
		 *
		 * @param settings pointer to settings
		 * @param games pointer to game vector
		 *
		 * @return 0 on success, 1 on error
		 */
		int gui_init(Settings *settings, std::vector<Game> *games);
	
		/**
		 * @brief try to open images for covert art and banner
		 *
		 * @param slug slug of a game, slug should be obtain from game vector 
		 */
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
		
	private:
		void render_one_line_of_text(uint64_t x, uint64_t y, const char* text);
		uint32_t render_multi_line_text(uint64_t x, uint64_t y, const char* text);
		
		SDL_Window* window;
		SDL_Renderer* renderer;
		
		std::vector<SDL_Texture*> cover_art;
		std::vector<SDL_Texture*> banner;
		
		TTF_Font* font;

		SDL_Gamepad* gamepad;
		bool buttons_pressed[TOTAL_BUTTONS];
		
		Settings* settings;

		ProcessHandler process_handler;

		std::vector<Game> *games;
		uint64_t current_game;
};

#endif

