#ifndef _NODE_RENDERER_H_
#define _NODE_RENDERER_H_

#include <vector>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "font.h"
#include "settings.h"
#include "node.h"

/**
 * @brief Node that handles input and redering through SDL3.0
 */
class Renderer {
	public:
		/**
		 *
		 * @param settings pointer to settings
		 *
		 */
		Renderer(Settings* settings);

		~Renderer();
		
		/**
		 * @brief try to open images for covert art and banner
		 *
		 * @param slug slug of a game, slug should be obtain from game vector 
		 */
		void load_texture(const char* slug);
	
		/**
		 *
		 * @brief handle the input
		 *
		 * @param buttons_pressed pointer to an array, where button state will be stored
		 *
		 * @return false when the "X" of the window was pressed, true otherwise
		 *
		 */
		bool check_input(bool* buttons_pressed);
		
		/**
		 * @brief clear the screen
		 */
		void clear_screen();
		
		/**
		 * @brief draw the screen
		 */
		void draw_screen();

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
		 * @brief destroys the textures
		 *
		 *
		 *
		 * \ingroup improvements
		 *
		 * when category change all images are destroyed and loaded again for current category
		 * improvement:
		 * 1. load all images at startup
		 * 2. store them in unordered_map<int, texture> where int = game_id, texture = game image
		 * 3. when changing categories only update games/categories vector
		 */
		void clear_images();
		
		/**
		 *
		 * @brief render rect on screen
		 *
		 * @param x x coordinate of the rect on screen
		 * @param y y coordinate of the rect on screen
		 * @param w width of the rect
		 * @param h height of the rect
		 * @param r red color value of the rect
		 * @param g green color value of the rect
		 * @param b blue color value of the rect
		 * @param a alpha value of the rect
		 * @param filled if true filled rect is rendered, if false only outline is renderer, default to true
		 *
		 */
		void render_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t r, uint64_t g, uint64_t b, uint64_t a, bool filled = true);

		
		/**
		 *
		 * @brief render cover art on screen
		 *
		 * @param index index of the covert art to render
		 * @param x x coordinate of the image on screen
		 * @param y y coordinate of the image on screen
		 * @param w width of the image on screen
		 * @param h height of the image on screen
		 *
		 * \bug aspect ratio calculation are in some situation incorrect
		 */
		void render_cover_art(uint64_t index, uint64_t x, uint64_t y, uint64_t w, uint64_t h);

		/**
		 *
		 * @param x coordinate of the viewport
		 * @param y coordinate of the viewport
		 * @param w width of the viewport
		 * @param h height of the viewport
		 *
		 * @details if either width of height of the viewport is set to 0, it will reset the viewport to whole screen, all values defaults to 0
		 *
		 */
		void set_viewport(int32_t x = 0, int32_t y = 0, int32_t w = 0, int32_t h = 0);
	private:

		SDL_Window* window;
		SDL_Renderer* renderer;

		std::vector<SDL_Texture*> cover_art;	

		TTF_Font* font;
		SDL_Gamepad* gamepad;

		Settings* settings;
		
};

#endif

