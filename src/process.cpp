#include "process.h"

bool ProcessHandler::run_process(const char* command) {
	int32_t pid = fork();

	if(pid == -1) {
		std::clog << "Unable to create child process\n";
		return false;
	}
	else if(pid == 0) { 
		uint64_t status = system(command); 
		exit(status);
	} else {
		pid_list.push_back(pid);
	}

	return true;
}

void ProcessHandler::check_and_clean_zombie_processes() {
	//check for running subprocesses, and read their status if finished so there is no zombie process
	//status of the subprocess is not used
	for(uint32_t i=0; i < pid_list.size(); i++) {
		if(waitpid(pid_list[i], nullptr, WNOHANG) != 0) {
			waitpid(pid_list[i], nullptr, 0);
			pid_list.erase(pid_list.begin() + i);
			break;
		}
	}
}

ProcessHandler::~ProcessHandler() {
	//wait for child processes to finish executing
	wait(NULL); 
}
