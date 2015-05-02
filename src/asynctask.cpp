
#include "asynctask.h"


AsyncTask::AsyncTask( NanCallback * callback )
	: NanAsyncWorker( callback ), _notify( false ) {
	// constructor
}


AsyncTask::~AsyncTask() {
	// destructor
}


void AsyncTask::HandleOKCallback() {

	NanScope();

	v8::Local< v8::Value > argv[] = {
		NanNull(),
		NanNew< v8::String >( _data )
	};

	callback->Call( 2, argv );
	return;

}


void AsyncTask::notify( const std::string &err, const std::string &data ) {

	{
		std::unique_lock< std::mutex > lock( _notify_mutex );

		if (! err.empty() ) {
			SetErrorMessage( err.c_str() );
		} else {
			_data = data;
		}

		_notify = true;

	}

	_notify_cnd.notify_one();

}


void AsyncTask::Execute() {

	if (! _notify ) {

		std::unique_lock< std::mutex > lock( _notify_mutex );
		_notify_cnd.wait( lock );

	}

}

