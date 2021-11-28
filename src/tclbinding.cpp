
#include "tclbinding.h"
#include <cstring>

//#ifdef HAS_TCL_THREADS
//#include "tclworker.h"
//#endif

#define MSG_NO_TCL_THREADS       "Thread support disabled, please ensure Tcl is compiled with --enable-threads flag set"
#define MSG_NO_THREAD_SUPPORT    "Thread support disabled, check g++ version for c++11 and/or Tcl thread support"


// initialise static vars
//Nan::Persistent< v8::Function > TclBinding::constructor;
using namespace Napi;

TclBinding::TclBinding(Napi::CallbackInfo& info) {

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	_tasks = nullptr;
#endif
 
	// initialise Tcl interpreter
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		Napi::Env env = info.Env();
		Napi::Error::New(env, "Failed to initialize Tcl interpretor").ThrowAsJavaScriptException();
		return Number();
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


Napi::Object TclBinding::init( Napi::Env env, Napi::Object exports ) {

	// stack-allocated handle scope
//	Nan::HandleScope scope;

	// prepare constructor template
//	v8::Local< v8::FunctionTemplate > tpl = Nan::New< v8::FunctionTemplate >( construct );
//	tpl->SetClassName( Nan::New( "TclBinding" ).ToLocalChecked() );
//	tpl->InstanceTemplate()->SetInternalFieldCount( 1 );
	Napi::Function func = DefineClass(env, 
		"TclBinding",
		{
			InstanceMethod("cmdSync", &TclBinding.cmdSync),
		});
	Napi::FunctionReference* constructor = new Napi::FunctionReference();
	*constructor = Napi::Persistent(func);
	env.SetInstanceData(constructor);
	exports.Set("TclBinding", func)

	// prototypes
//	Nan::SetPrototypeMethod( tpl, "cmd", cmd );
//	Nan::SetPrototypeMethod( tpl, "cmdSync", cmdSync );
//	Nan::SetPrototypeMethod( tpl, "queue", queue );
//	Nan::SetPrototypeMethod( tpl, "toArray", toArray );
//	exports.Set( Napi::String::New(env, "cmd"), Napi::Function::New(env, cmd) );
//	exports.Set( Napi::String::New(env, "cmdSync"), Napi::Function::New(env, cmdSync) );
//	exports.Set( Napi::String::New(env, "queue"), Napi::Function::New(env, queue) );
//	exports.Set( Napi::String::New(env, "toArray"), Napi::Function::New(env, toArray) );

//	constructor.Reset( tpl->GetFunction() );
//	exports->Set( Nan::New( "TclBinding" ).ToLocalChecked(), tpl->GetFunction() );
//	return exports;

}

//Fixme: what is this needed for?
//void TclBinding::construct( const Nan::FunctionCallbackInfo< v8::Value > &info ) {
//
//	if (! info.IsConstructCall() ) {
//
//		// invoked as `TclBinding(...)`, convert to a constructor call
//		const int argc = 1;
//		v8::Local< v8::Value > argv[ argc ] = { info[0] };
//		v8::Local< v8::Function > c = Nan::New< v8::Function >( constructor );
//		return info.GetReturnValue().Set( Nan::NewInstance( c, argc, argv ).ToLocalChecked() );
//
//	}
//
//	TclBinding * obj = new TclBinding();
//	obj->Wrap( info.This() );
//	info.GetReturnValue().Set( info.This() );
//
//}


Napi::String TclBinding::cmd( const Napi::CallbackInfo& info ) {

	// validate input params
	size_t length = info.Length();
	if ( length != 2 ) {
		Napi::Error::New(env, "Invalid number of arguments").ThrowAsJavaScriptException();
		return Number();
	}

	if (! isString(info[0]) ) {
		Napi::Error::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return Number();
	}

	if (! isFunction(info[1]) ) {
		Napi::Error::New(env, "Callback must be a function").ThrowAsJavaScriptException();
		return Number();
	}

//Fixme
//	Nan::Callback * callback = new Nan::Callback( info[1].As< v8::Function >() );

//#ifdef HAS_TCL_THREADS
//	// schedule an async task
//	Nan::Utf8String cmd( info[0] );
//	Nan::AsyncQueueWorker( new TclWorker( *cmd, callback ) );
//#else
//	v8::Local< v8::Value > argv[] = {
//			Nan::Error( Nan::New< v8::String >( MSG_NO_TCL_THREADS ).ToLocalChecked() )
//	};
//	callback->Call( 1, argv );
//#endif

//	info.GetReturnValue().Set( Nan::Undefined() );
//	return Napi::String::New(env, )

}


Napi::String TclBinding::cmdSync( const Napi::CallbackInfo& info ) {

	// validate input params
	size_t length = info.Length();
	if ( length < 1 ) {
		Napi::Error::New(env, "Require a Tcl command to execute").ThrowAsJavaScriptException();
		return Number();
	}

	if (! isString(info[0]) ) {
		Napi::Error::New(env, "Tcl command must be a string").ThrowAsJavaScriptException();
		return Number();
	}

	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );
//	Nan::Utf8String cmd( info[0] );
	std::string cmd = info[0].ToString().Utf8Value();

	// evaluate command
	int code = Tcl_EvalEx( binding->_interp, *cmd, -1, 0 );

	// check for errors
	if ( code == TCL_ERROR ) {
		Napi::Error::New(env, Tcl_GetStringResult(binding->_interp)).ThrowAsJavaScriptException();
		return Number();
	}

	// grab the result
	Tcl_Obj * result = Tcl_GetObjResult( binding->_interp );

	// return result as a string
	const char * str_result = Tcl_GetString( result );
	return Napi::String::New(env, str_result);
}


//void TclBinding::queue( const Nan::FunctionCallbackInfo< v8::Value > &info ) {
//
//	// validate input params
//	if ( info.Length() != 2 ) {
//		return Nan::ThrowError( "Invalid number of arguments" );
//	}
//
//	if (! info[0]->IsString() ) {
//		return Nan::ThrowTypeError( "Tcl command must be a string" );
//	}
//
//	if (! info[1]->IsFunction() ) {
//		return Nan::ThrowTypeError( "Callback must be a function" );
//	}
//
//
//	Nan::Callback * callback = new Nan::Callback( info[1].As< v8::Function >() );
//
//#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
//	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );
//
//	if ( binding->_tasks == nullptr ) {
//		binding->_tasks = new TaskRunner();
//	}
//
//	// queue the task
//	Nan::Utf8String cmd( info[0] );
//	binding->_tasks->queue( * cmd, callback );
//#else
//	v8::Local< v8::Value > argv[] = {
//			Nan::Error( Nan::New< v8::String >( MSG_NO_THREAD_SUPPORT ).ToLocalChecked() )
//	};
//	callback->Call( 1, argv );
//#endif
//
//	info.GetReturnValue().Set( Nan::Undefined() );
//
//}


//void TclBinding::toArray( const Nan::FunctionCallbackInfo< v8::Value > &info ) {
//
//	// validate input params
//	if ( info.Length() < 1 ) {
//		return info.GetReturnValue().Set( Nan::Undefined() );
//	}
//
//	if (! info[0]->IsString() ) {
//		return Nan::ThrowTypeError( "Input must be a string" );
//	}
//
//	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );
//	Nan::Utf8String str( info[0] );
//
//	// create a Tcl string object
//	Tcl_Obj * obj = Tcl_NewStringObj( *str, strlen( *str ) );
//
//	int objc = 0;
//	Tcl_Obj **objv;
//
//	// attempt to parse as a Tcl list
//	if ( Tcl_ListObjGetElements( binding->_interp, obj, &objc, &objv ) == TCL_OK ) {
//
//		v8::Local< v8::Array > r_array = Nan::New< v8::Array >( objc );
//
//		for ( int i = 0; i < objc; i++ ) {
//			r_array->Set( i, Nan::New< v8::String >( Tcl_GetString( objv[i] ) ).ToLocalChecked() );
//		}
//
//		return info.GetReturnValue().Set( r_array );
//
//	}
//
//	// not a valid Tcl list
//	info.GetReturnValue().Set( Nan::Null() );
//
//}
