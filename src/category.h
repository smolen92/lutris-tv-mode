#ifndef _CATEGORIES_H_
#define _CATEGORIES_H_

#include <string>
#include <cstdint>
#include <vector>
#include <sqlite3.h>

/**
 * @brief class for storing info about categories
 */
class Category {
	public:
		/**
		 * @brief this settings shoudl be load from a database
		 *
		 * @param id id of the category
		 * @param name name of the category
		 *
		 */
		Category(uint64_t id, const char* name);
		
		/**
		 * @brief callback fuction for loading categories data from database
		 */
		static void callback_load_categories(void* data_vector, sqlite3_stmt* pre_statement);

		/// \cond
		uint64_t id;
		std::string name;
		/// \endcond
};

#endif

