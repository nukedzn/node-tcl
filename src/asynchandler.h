
#ifndef ASYNCHANDLER_H
#define ASYNCHANDLER_H

#include <nan.h>
#include <atomic>
#include <mutex>
#include <string>
#include <condition_variable>


class AsyncHandler : public Nan::AsyncWorker {
public:

	AsyncHandler( Nan::Callback * callback );
	virtual ~ AsyncHandler();

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

#endif /*! ASYNCHANDLER_H */

