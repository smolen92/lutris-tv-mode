#ifndef _NODE_H_
#define _NODE_H_

#include <cstdint>

enum Buttons {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RUN,
	CATEGORIES,
	FAVORITE,
	SELECT,
	START,
	HOME,
	TOTAL_BUTTONS
};

enum Action {
	NONE,
	RUN_GAME,
	READ_DATABASE,
	SWITCH_TO_CATEGORY_NODE,
	REMOVE_NODE,
	ADD_REMOVE_FAVORITE
};

/**
 * @brief data that will be exchanged between nodes
 */
struct Global_data {
	/// \cond
	uint64_t current_game;
	uint64_t current_category;
	uint64_t action;
	bool buttons_pressed[TOTAL_BUTTONS];
	/// \endcond
};

/**
 * @brief genric Node class that other Nodes will inherit
 */
class Node {
	public:
		/// \cond
		virtual void logic(Global_data* global_data) = 0;
		virtual void render() = 0;
		virtual ~Node() { };
		/// \endcond
};

/**
 *
 * \defgroup node_system Node System
 *
 * Node:
 * - Every gui element must inherit Node class
 * - Every gui element (e.g. games's grid, category menu) is one node
 * - Nodes are handled by Gui_manager class
 * - Every inherited node must have pointer to renderer class
 * - render() function must be implemented thorugh renderer class
 * - data between nodes are exchanges through Global_data structure
 * 
 * Gui_manager:
 * - only logic() of the last node added is run
 * - nodes are renderer in ordered, that they were added (first added, first renderer)
 *
 * Renderer:
 * - this class handles rendering and input with SDL3.0
 * - if the rendering or input handling need to be done with other libraries only this class need to be edited 
 */

#endif

