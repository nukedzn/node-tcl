
#ifndef TASKRUNNER_H
#define TASKRUNNER_H

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>

#include <napi.h>
#include "asynchandler.h"

class TaskRunner {
public:

	struct task_t {
		std::string cmd;
		AsyncHandler * handler;
	};


	TaskRunner();
	virtual ~TaskRunner();

	void queue( Napi::Function& callback, const char * cmd );


private:

	void worker();

	std::atomic< bool >  _terminate;
	std::queue< task_t > _tasks;

	std::thread _worker;
	std::mutex  _task_mutex;

	std::condition_variable _worker_cnd;

};

#endif /*! TASKRUNNER_H */
