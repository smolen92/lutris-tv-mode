#ifndef _GUI_H_
#define _GUI_H_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "settings.h"
#include "process.h"
#include "game.h"
#include "category.h"
#include "font.h"

enum Buttons {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RUN,
	CATEGORIES,
	TOTAL_BUTTONS
};

/**
 * @brief class for handling gui using sdl
 * \todo move category menu and games's grid into its own class
 */
class Gui {
	public:
		/**
		 * @brief initialize the gui
		 *
		 * @param settings pointer to settings
		 * @param games pointer to game vector
		 * @param categories pointer to category vector
		 *
		 * @return 0 on success, 1 on error
		 *
		 * \todo change this into constructor and throw exceptions
		 */
		int gui_init(Settings *settings, std::vector<Game> *games, std::vector<Category> * categories);
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
		
		/**
		 * @brief try to open images for covert art and banner
		 *
		 * @param slug slug of a game, slug should be obtain from game vector 
		 */
		void load_texture(const char* slug);
		
		/**
		 * @brief render one line of centerd text
		 *
		 * @param x x-position of the text
		 * @param y y-position of the text
		 * @param text text to render
		 * @param area_width width of the area
		 *
		 *
		 * @details this function is called internally by render_multi_line_text and shouldn't be called by itself
		 */
		void render_one_line_of_text(const uint64_t x, const uint64_t y, const char* text, const int32_t area_width);
		
		/**
		 *
		 * @brief render text on multiple lines, all lines are centered
		 *
		 * @param x x-position of the text
		 * @param y y-position of the text
		 * @param text text to render
		 *
		 * @return height that the text used for rendering
		 *
		 */
		uint32_t render_multi_line_text(uint64_t x, uint64_t y, const char* text);

		/**
		 *
		 * @brief render the category menu base on settings
		 *
		 */
		void render_categories_menu();

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
		std::vector<Category> *categories;
		uint64_t current_game;
		uint64_t current_category;

		bool render_categories;
};

#endif

