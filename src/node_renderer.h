#ifndef _NODE_RENDERER_H_
#define _NODE_RENDERER_H_

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "node.h"
#include "font.h"
#include "settings.h"

/**
 * @brief Node that handles input and redering through SDL3.0
 */
class Node_renderer : public Node {
	public:
		/// inherited from Node
		bool logic(void* data) override;
		/// intherited from Node
		void render() override;

		/**
		 *
		 * @param settings pointer to settings
		 *
		 */
		Node_renderer(Settings* settings);

		~Node_renderer();
		
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
		 * @brief destroys the textures
		 *
		 */
		void clear_images();

		void render_rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h, uint64_t r, uint64_t g, uint64_t b, uint64_t a, bool filled = true);

		/// \todo keep aspec ratio of images
		void render_cover_art(uint64_t index, uint64_t x, uint64_t y, uint64_t w, uint64_t h);
	private:

		SDL_Window* window;
		SDL_Renderer* renderer;

		std::vector<SDL_Texture*> cover_art;	

		TTF_Font* font;
		SDL_Gamepad* gamepad;

		Settings* settings;
		
};
	
#endif

