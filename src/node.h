#ifndef _NODE_H_
#define _NODE_H_

enum Buttons {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	RUN,
	CATEGORIES,
	TOTAL_BUTTONS
};

enum Action {
	NONE,
	READ_DATABASE,
	SWITCH_TO_CATEGORY_NODE,
	REMOVE_NODE
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
 * \todo document Node system
 */
class Node {
	public:
		/// \cond
		virtual void logic(Global_data* global_data) = 0;
		virtual void render() = 0;
		virtual ~Node() { };
		/// \endcond
};

#endif

