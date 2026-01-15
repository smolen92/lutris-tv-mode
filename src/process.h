#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

#define MAX_ARGS 32

/**
 * @brief class that handles creating processes
 */
class ProcessHandler {
	public:
		/**
		 * @brief run system command
		 *
		 * @param command comand to run
		 *
		 * @details
		 * this function will create a child process where the command will be run
		 */
		bool run_process(const char* command);	

		~ProcessHandler();
	private:
		std::vector<uint32_t> pid_list;
};

#endif

