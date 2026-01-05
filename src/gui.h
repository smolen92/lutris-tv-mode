#ifndef _GUI_H_
#define _GUI_H_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "settings.h"
#include "process.h"
#include "game.h"

#define FONT_SIZE 16

/**
 * @brief class for handling gui using sdl
 * \todo FONT_SIZE shoudl be read from settings
 * \todo read font from data
 */
class Gui {
	public:
		/**
		 * @param font poitner to font, that will be used for text
		 */
		int gui_init(std::vector<Game> *games);
		
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
	
		/// \todo don't strech text with low char count, don't squish text with large char count
		void render_text(uint64_t x, uint64_t y, const char* text);

		~Gui();
		
		std::vector<SDL_Texture*> cover_art;
		std::vector<SDL_Texture*> banner;

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		
		TTF_Font* font;

		Settings settings;

		float vertical_offset, horizontal_offset;
		
		ProcessHandler process_handler;

		std::vector<Game> *games;
		uint64_t current_game;

};

#endif

