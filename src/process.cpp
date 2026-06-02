#include "process.h"

ProcessHandler::ProcessHandler() {
	cwd = get_current_dir_name();
	if(cwd == nullptr) throw std::runtime_error("Failed to get current working dir\n");

	
	struct passwd *temp;
	temp = getpwuid(getuid());
	if(temp == nullptr) throw std::runtime_error("Failed to get user name\n");
	
	user = temp->pw_name;

}

void ProcessHandler::run_process(uint64_t game_id, const char* command) {
	int32_t pid = fork();

	if(pid == -1) {
		throw std::runtime_error("Unable to create child process");
	}
	else if(pid == 0) { 
		//child
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
		std::cerr << "exec failed\n";
		_Exit(1);
	} else {
		process_info temp_info = {pid, this->get_millis() };
		game_id_to_pid_map[game_id] = temp_info;
	}
	
}

bool ProcessHandler::is_process_running(uint64_t game_id) {
	return game_id_to_pid_map.find(game_id) != game_id_to_pid_map.end();
}

void ProcessHandler::kill_process(uint64_t game_id, int32_t signal ) {

	if( game_id_to_pid_map.find(game_id) != game_id_to_pid_map.end()) {
		kill(game_id_to_pid_map[game_id].pid, signal);
	}

}

void ProcessHandler::check_and_clean_zombie_processes() {
        //check for running subprocesses, and read their status if finished so there is no zombie process
        //status of the subprocess is not used
	for(auto i=game_id_to_pid_map.begin(); i != game_id_to_pid_map.end();  i++) {
        	if(waitpid(i->second.pid, nullptr, WNOHANG) != 0) {
			uint64_t temp_end_time = this->get_millis();
			waitpid(i->second.pid, nullptr, 0);
			game_id_to_pid_map.erase(i);
                        break;
                }
        }
}

ProcessHandler::~ProcessHandler() {
	free(cwd);
	cwd = nullptr;
}

uint64_t ProcessHandler::get_millis() {
	timespec temp_ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &temp_ts);
	return (uint64_t)(temp_ts.tv_sec*1000 + temp_ts.tv_nsec/1000000);
}

