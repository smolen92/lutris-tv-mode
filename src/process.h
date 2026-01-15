#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

#define MAX_ARGS 32

class ProcessHandler {
	public:
		bool run_process(const char* command);	

		~ProcessHandler();
	private:
		std::vector<uint32_t> pid_list;
};

#endif

