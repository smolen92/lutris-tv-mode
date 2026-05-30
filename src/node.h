#ifndef _NODE_H_
#define _NODE_H_

#include <cstdint>
#include <vector>

enum Buttons {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RUN, //A
	CATEGORIES,//RB
	FAVORITE, //Y
	SELECTION, //X
	START, //START
	TOTAL_BUTTONS
};

enum Action {
	ACTION_NONE,
	ACTION_RUN_GAME,
	ACTION_READ_DATABASE,
	ACTION_SWITCH_TO_CATEGORY_NODE,
	ACTION_SWITCH_TO_CAT_TABLE_NODE,
	ACTION_REMOVE_NODE,
	ACTION_ADD_REMOVE_FAVORITE,
	ACTION_ADD_CATEGORIES,
	ACTION_SHUTDOWN_SYSTEM,
	ACTION_RESTART_SYSTEM,
	ACTION_SHOW_START_MENU,
	ACTION_QUIT_TV_MODE
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
	uint16_t mouse_x, mouse_y;
	uint32_t mouse_flags; //0x01 - LMB, 0x02 - MMB, 0x04- RMB
	std::vector<bool> categories_present;
	/// \endcond
};

/**
 * @brief genric Node class that other Nodes will inherit
 */
class Node {
	public:
		/// \cond
		virtual void logic() = 0;
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
 * - if the rendering or input handling need to be done with other libraries only this class needs to be changed 
 */

#endif

