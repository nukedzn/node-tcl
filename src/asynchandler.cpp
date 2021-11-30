
#include "asynchandler.h"

AsyncHandler::AsyncHandler( Napi::Function& callback )
	: Napi::AsyncWorker( callback ), _notify( false ) {
	// constructor
}


AsyncHandler::~AsyncHandler() {
	// destructor
}


void AsyncHandler::OnOK() {
	Napi::Env env = Env();
    
	Callback().Call({env.Null(), Napi::String::New(env, _data)});
	return;

}


void AsyncHandler::notify( const std::string &err, const std::string &data ) {

	{
		std::unique_lock< std::mutex > lock( _notify_mutex );

		if (! err.empty() ) {
			SetError( err.c_str() );
		} else {
			_data = data;
		}

		_notify = true;

	}

	_notify_cnd.notify_one();

}


void AsyncHandler::Execute() {

	if (! _notify ) {

		std::unique_lock< std::mutex > lock( _notify_mutex );
		_notify_cnd.wait( lock );

	}

}
