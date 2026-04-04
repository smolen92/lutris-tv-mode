#include "process.h"

ProcessHandler::ProcessHandler() {
	cwd = get_current_dir_name();
	if(cwd == nullptr) throw std::runtime_error("Failed to get current working dir\n");

	
	struct passwd *temp;
	temp = getpwuid(getuid());
	if(temp == nullptr) throw std::runtime_error("Failed to get user name\n");
	
	user = temp->pw_name;

}

bool ProcessHandler::run_process(const char* command) {
	int32_t pid = fork();

	if(pid == -1) {
		throw std::runtime_error("Unable to create child process");
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
		_exit(1);
		return false; 
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

	free(cwd);
	cwd = nullptr;
}

