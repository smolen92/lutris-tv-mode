#ifndef _GUI_H_
#define _GUI_H_


#include "sql.h"

/**
 * @brief class for handling gui using sdl
 * \todo move category menu into its own class
 * \todo switching back to rendering all games after category is selected
 */
class Gui {
	public:
		/**
		 * @brief initialize the gui
		 *
		 * @param settings pointer to settings
		 * @param sql pointer to sql class, where database file is opened
		 * @param games pointer to game vector
		 * @param categories pointer to category vector
		 *
		 */
		Gui(Settings *settings, SQL *sql, std::vector<Game> *games, std::vector<Category> * categories);
	
		/**
		 * @brief do the logic only for gui
		 */
		void logic();

	private:
		
		SQL* sql;

};

#endif

