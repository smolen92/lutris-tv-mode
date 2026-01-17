#include "process.h"

/// \todo quit/kill child process if exec failed
bool ProcessHandler::run_process(const char* command) {
	int32_t pid = fork();

	if(pid == -1) {
		std::clog << "Unable to create child process\n";
		return false;
	}
	else if(pid == 0) { 
		const char* args[MAX_ARGS+1];
		
		std::stringstream input_string(command);
		std::string temp_string;
		std::vector<std::string> temp_vec;
		
		temp_vec.reserve(MAX_ARGS);
		
		uint8_t args_index=0;
		while(std::getline(input_string,temp_string,' ') && (args_index < MAX_ARGS)) {
			temp_vec.push_back(temp_string);
			args[args_index] = temp_vec.back().c_str();
			args_index++;
		}

		args[args_index] = NULL;

		execvp(args[0],(char* const*)args);
		
		//only when exec failed
		std::clog << "exec failed\n";
		return false; 
	} else {
		pid_list.push_back(pid);
	}

	return true;
}

ProcessHandler::~ProcessHandler() {
	//wait for child processes to finish executing
	wait(NULL); 
}
