#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sstream>
#include <string>

#define MAX_ARGS 32

/**
 * @brief class that handles creating processes
 */
class ProcessHandler {
	public:
		ProcessHandler();

		/**
		 * @brief run system command
		 *
		 * @param command comand to run
		 *
		 * @details
		 * this function will create a child process where the command will be run
		 */
		bool run_process(const char* command);	

		/**
		 *
		 * @brief check for zombie processes and clean them if necessary
		 *
		 */
		void check_and_clean_zombie_processes();
		
		~ProcessHandler();
		
		/// \cond
		char *cwd;
		std::string user;
		/// \endcond
	private:
		std::vector<uint32_t> pid_list;
};

#endif

