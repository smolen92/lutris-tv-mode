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
#include <time.h>

#define MAX_ARGS 32

/**
 * @brief structure that holds info about process
 */
struct process_info {
 	///pid of the process
	int32_t pid;
 	///start time of the app - second since tv mode was started
	uint64_t start_time;
};

/**
 * @brief class that handles creating processes
 * \todo run native games directly + update time in db
 * \todo end time how to return the info so gui_manager can edit playtime in db? 
 */
class ProcessHandler {
	public:
		ProcessHandler();

		/**
		 * @brief run system command
		 *
		 * @param game_id id of the game to run
		 * @param command comand to run
		 *
		 * @return true on success, false if child process failed to be created
		 *
		 * @details this function will create a child process where the command will be run
		 *
		 * \bug if run process failed, child processes are still running
		 */
		bool run_process(uint64_t game_id, const char* command);	
	
		/**
		 * @brief check if process is running
		 *
		 * @param game_id game id to check
		 *
		 * @return true if there is a process associated with the id, false if not
		 *
		 */
		bool is_process_running(uint64_t game_id);
		
		/**
		 *
		 * @brief kill process with specified signal
		 *
		 * @param game_id id of the game to kill
		 * @param signal SIGTERM or SIGKILL
		 *
		 */
		void kill_process(uint64_t game_id, int32_t signal);

		/**
		 *
		 * @brief check for zombie processes and clean them if necessary
		 *
		 */
		void check_and_clean_zombie_processes();
	
		/**
		 *
		 * @return millisecond since the start of the programm
		 *
		 */
		uint64_t get_millis();

		~ProcessHandler();
		
		/// \cond
		char *cwd;
		std::string user;
		/// \endcond
	private:
		std::unordered_map<uint64_t, process_info> game_id_to_pid_map;
};

#endif

