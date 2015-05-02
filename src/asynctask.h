
#ifndef ASYNCTASK_H
#define ASYNCTASK_H

#include <nan.h>
#include <atomic>
#include <mutex>
#include <string>
#include <condition_variable>


class AsyncTask : public NanAsyncWorker {
public:

	AsyncTask( NanCallback * callback );
	virtual ~ AsyncTask();

	void notify( const std::string &err, const std::string &data );
	void Execute();


protected:

	void HandleOKCallback();


private:

	std::atomic< bool > _notify;

	std::mutex _notify_mutex;
	std::condition_variable _notify_cnd;

	std::string _data;

};

#endif /*! ASYNCTASK_H */

