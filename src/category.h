#ifndef _CATEGORIES_H_
#define _CATEGORIES_H_

#include <string>
#include <vector>
#include <cstdint>

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

		/// \cond
		uint64_t id;
		std::string name;

		std::vector<uint64_t> games_indexes;
		/// \endcond
};

#endif

