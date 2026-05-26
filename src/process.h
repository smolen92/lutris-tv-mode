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
#include <unordered_map>

#define MAX_ARGS 32

/**
 * @brief class that handles creating processes
 * \todo run native games directly + update time in db
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
		 * @details this function will create a child process where the command will be run
		 */
		void run_process(uint64_t game_id, const char* command);	
	
		/**
		 *
		 * @brief kill process with specified signal
		 *
		 * @param pid pid of the process to kill
		 * @param signal SIGTERM or SIGKILL
		 *
		 */

		/**
		 * @brief check if process is running
		 *
		 * @param game_id game id to check
		 *
		 * @return true if there is a process associated with the id, false if not
		 *
		 */
		bool is_process_running(uint64_t game_id);

		void kill_process(uint64_t game_id, int32_t singal);
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
		std::unordered_map<uint64_t, uint32_t> game_id_to_pid_map;
};

#endif

