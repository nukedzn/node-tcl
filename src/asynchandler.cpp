
#include "asynchandler.h"


AsyncHandler::AsyncHandler( Nan::Callback * callback )
	: Nan::AsyncWorker( callback ), _notify( false ) {
	// constructor
}


AsyncHandler::~AsyncHandler() {
	// destructor
}


void AsyncHandler::HandleOKCallback() {

	// stack-allocated handle scope
	Nan::HandleScope scope;

	v8::Local< v8::Value > argv[] = {
		Nan::Null(),
		Nan::New< v8::String >( _data ).ToLocalChecked()
	};

	callback->Call( 2, argv );
	return;

}


void AsyncHandler::notify( const std::string &err, const std::string &data ) {

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


void AsyncHandler::Execute() {

	if (! _notify ) {

		std::unique_lock< std::mutex > lock( _notify_mutex );
		_notify_cnd.wait( lock );

	}

}

