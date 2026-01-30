#ifndef _INTERFACE_H_
#define _INTERFACE_H_


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
 * @brief genric Node class that other Nodes will inherit
 * \todo document Node system
 */
class Node {
	public:
		/// \cond
		virtual bool logic(void* data) = 0;
		virtual void render() = 0;
		virtual ~Node();
		/// \endcond
};

#endif

