
#include "tclbinding.h"
#include <cstring>

#ifdef HAS_TCL_THREADS
#include "tclworker.h"
#endif

#define MSG_NO_TCL_THREADS       "Thread support disabled, please ensure Tcl is compiled with --enable-threads flag set"
#define MSG_NO_THREAD_SUPPORT    "Thread support disabled, check g++ version for c++11 and/or Tcl thread support"

using namespace Napi;

TclBinding::TclBinding(const Napi::CallbackInfo& info) : ObjectWrap(info) {

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	_tasks = nullptr;
#endif
 
	// initialise Tcl interpreter
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		Napi::Env env = info.Env();
		Napi::Error::New(env, "Failed to initialise Tcl interpreter").ThrowAsJavaScriptException();
	}

}


TclBinding::~TclBinding() {

	// cleanup
#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	if ( _tasks ) {
		delete _tasks;
	}
#endif

	Tcl_DeleteInterp( _interp );

}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "TclBinding"), TclBinding::GetClass(env));
  return exports;
}

Napi::Value TclBinding::cmd( const Napi::CallbackInfo& info ) {
	Napi::Env env = info.Env();

	// validate input params
	if ( info.Length() != 2 ) {
		Napi::TypeError::New(env, "Invalid number of arguments").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	if (! info[0].IsString() ) {
		Napi::TypeError::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	if (! info[1].IsFunction() ) {
		Napi::TypeError::New(env, "Callback must be a function").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	std::string cmd = info[0].As<Napi::String>().Utf8Value();
	Napi::Function callback = info[1].As<Napi::Function>();

#ifdef HAS_TCL_THREADS
	// schedule an async task
	TclWorker* tw = new TclWorker(callback, cmd.c_str());
	tw->Queue();
#else
	Napi::Value argv[] = {Napi::String::New(env, MSG_NO_TCL_THREADS)};
	callback->Call( 1, argv );
#endif

	return env.Undefined();

}


Napi::Value TclBinding::cmdSync( const Napi::CallbackInfo& info ) {
	Napi::Env env = info.Env();

	// validate input params
	if ( info.Length() < 1 ) {
		Napi::TypeError::New(env, "Require a Tcl command to execute").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	if (! info[0].IsString() ) {
		Napi::TypeError::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	std::string cmd = info[0].As<Napi::String>().Utf8Value();

	// evaluate command
	int code = Tcl_EvalEx( this->_interp, cmd.c_str(), -1, 0 );

	// check for errors
	if ( code == TCL_ERROR ) {
		Napi::Error::New(env, Tcl_GetStringResult(this->_interp)).ThrowAsJavaScriptException();
		return env.Undefined();
	}

	// grab the result
	Tcl_Obj * result = Tcl_GetObjResult( this->_interp );

	// return result as a string
	return Napi::String::New(env, Tcl_GetString( result ));
}


Napi::Value TclBinding::queue( const Napi::CallbackInfo& info ) {
	Napi::Env env = info.Env();

	// validate input params
	if ( info.Length() != 2 ) {
		Napi::TypeError::New(env, "Invalid number of arguments").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	if (! info[0].IsString() ) {
		Napi::TypeError::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	if (! info[1].IsFunction() ) {
		Napi::TypeError::New(env, "Callback must be a function").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	std::string cmd = info[0].As<Napi::String>().Utf8Value();
	Napi::Function callback = info[1].As<Napi::Function>();

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	if ( this->_tasks == nullptr ) {
		this->_tasks = new TaskRunner();
	}

	// queue the task
	this->_tasks->queue( callback, cmd.c_str() );

#else
	Napi::Value argv[] = {Napi::String::New(env, MSG_NO_THREAD_SUPPORT)};
	callback->Call( 1, argv );
#endif

	return env.Undefined();
}


Napi::Value TclBinding::toArray( const Napi::CallbackInfo& info ) {
	Napi::Env env = info.Env();

	// validate input params
	if ( info.Length() < 1 ) {
		return env.Undefined();
	}

	if (! info[0].IsString() ) {
		Napi::TypeError::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return env.Undefined();
	}

	std::string str = info[0].As<Napi::String>().Utf8Value();

	// create a Tcl string object
	Tcl_Obj* obj = Tcl_NewStringObj( str.c_str(), str.length() );

	int objc = 0;
	Tcl_Obj **objv;

	// attempt to parse as a Tcl list
	if ( Tcl_ListObjGetElements( this->_interp, obj, &objc, &objv ) == TCL_OK ) {

		Napi::Array r_array = Napi::Array::New(env, objc);

		for ( int i = 0; i < objc; i++ ) {
			r_array[i] = Napi::String::New(env, Tcl_GetString( objv[i] ));
		}
		return r_array;
	}

	// not a valid Tcl list
	return env.Undefined();

}

Napi::Function TclBinding::GetClass( Napi::Env env ) {

    return DefineClass(env, "TclBinding",
		{
			TclBinding::InstanceMethod("cmd",		&TclBinding::cmd),
			TclBinding::InstanceMethod("cmdSync",	&TclBinding::cmdSync),
			TclBinding::InstanceMethod("queue",		&TclBinding::queue),
			TclBinding::InstanceMethod("toArray",	&TclBinding::toArray),
		});
}

NODE_API_MODULE(addon, Init)
