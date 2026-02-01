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

/**
 * @brief data that will be exchanged between nodes
 */
struct GlobalData {
	/// \cond
	uint64_t current_game;
	uint64_t current_category;
	uint64_t active_node_id;
	bool read_database;
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
		virtual void logic(void* global_data) = 0;
		virtual void render() = 0;
		virtual ~Node();
		bool active;
		/// \endcond
};

#endif

