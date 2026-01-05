#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

class ProcessHandler {
	public:
		bool run_process(const char* command);	

		void check_and_clean_zombie_processes();

		~ProcessHandler();
	private:
		std::vector<uint32_t> pid_list;
};

#endif

