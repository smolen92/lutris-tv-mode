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
#include <signal.h>

#define MAX_ARGS 32

/**
 * @brief class that handles creating processes
 * \todo run native games directly + update time in db
 * \todo change std::vector to unordered map - key=game_id, value=pid and create function for lookup
 * \todo track start and end time of a process unorderde map value pair of pid and starttime ? 
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
		 * @brief kill process with specified signal
		 *
		 * @param pid pid of the process to kill
		 * @param signal SIGTERM or SIGKILL
		 *
		 * \todo now this process will kill all processes that was started, when unordered map is used change the parameter to game_id, do a lookup and then kill the process
		 */
		void kill_process(uint32_t pid, int32_t singal);
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

