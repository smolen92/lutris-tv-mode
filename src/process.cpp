#include "process.h"

bool ProcessHandler::run_process(const char* command) {
	int32_t pid = fork();

	if(pid == -1) {
		std::clog << "Unable to create child process\n";
		return false;
	}
	else if(pid == 0) { 
		std::string game = std::string("lutris:rungameid/") + std::string(command);

		execlp("lutris", "lutris", game.c_str(), NULL);
	} else {
		pid_list.push_back(pid);
	}

	return true;
}

ProcessHandler::~ProcessHandler() {
	//wait for child processes to finish executing
	wait(NULL); 
}
