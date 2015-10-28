
#include "tclbinding.h"
#include "tclworker.h"
#include <cstring>


// initialise static vars
Nan::Persistent< v8::Function > TclBinding::constructor;

TclBinding::TclBinding() {

	// initialise Tcl interpreter
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		Nan::ThrowError( "Failed to initialise Tcl interpreter" );
	}

}


TclBinding::~TclBinding() {
	// cleanup
	Tcl_DeleteInterp( _interp );
}


void TclBinding::init( v8::Local< v8::Object > exports ) {

	// prepare constructor template
	v8::Local< v8::FunctionTemplate > tpl = Nan::New< v8::FunctionTemplate >( construct );
	tpl->SetClassName( Nan::New( "TclBinding" ).ToLocalChecked() );
	tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

	// prototypes
	Nan::SetPrototypeMethod( tpl, "cmd", cmd );
	Nan::SetPrototypeMethod( tpl, "cmdSync", cmdSync );
	Nan::SetPrototypeMethod( tpl, "queue", queue );
	Nan::SetPrototypeMethod( tpl, "toArray", toArray );

	constructor.Reset( tpl->GetFunction() );
	exports->Set( Nan::New( "TclBinding" ).ToLocalChecked(), tpl->GetFunction() );

}


void TclBinding::construct( const Nan::FunctionCallbackInfo< v8::Value > &info ) {

	if (! info.IsConstructCall() ) {

		// invoked as `TclBinding(...)`, convert to a constructor call
		const int argc = 1;
		v8::Local< v8::Value > argv[ argc ] = { info[0] };
		v8::Local< v8::Function > c = Nan::New< v8::Function >( constructor );
		return info.GetReturnValue().Set( c->NewInstance( argc, argv ) );

	}

	TclBinding * obj = new TclBinding();
	obj->Wrap( info.This() );
	info.GetReturnValue().Set( info.This() );

}


void TclBinding::cmd( const Nan::FunctionCallbackInfo< v8::Value > &info ) {

	// validate input params
	if ( info.Length() != 2 ) {
		Nan::ThrowError( "Invalid number of arguments" );
	}

	if (! info[0]->IsString() ) {
		Nan::ThrowTypeError( "Tcl command must be a string" );
	}

	if (! info[1]->IsFunction() ) {
		Nan::ThrowTypeError( "Callback must be a function" );
	}


	// schedule an async task
	Nan::Utf8String cmd( info[0] );
	Nan::Callback * callback = new Nan::Callback( info[1].As< v8::Function >() );
	Nan::AsyncQueueWorker( new TclWorker( *cmd, callback ) );

	info.GetReturnValue().Set( Nan::Undefined() );

}


void TclBinding::cmdSync( const Nan::FunctionCallbackInfo< v8::Value > &info ) {

	// validate input params
	if ( info.Length() < 1 ) {
		Nan::ThrowError( "Require a Tcl command to execute" );
	}

	if (! info[0]->IsString() ) {
		Nan::ThrowTypeError( "Tcl command must be a string" );
	}


	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );
	Nan::Utf8String cmd( info[0] );

	// evaluate command
	int code = Tcl_EvalEx( binding->_interp, *cmd, -1, 0 );

	// check for errors
	if ( code == TCL_ERROR ) {
		Nan::ThrowError( Tcl_GetStringResult( binding->_interp ) );
	}


	// grab the result
	Tcl_Obj * result = Tcl_GetObjResult( binding->_interp );

	// return result as a string
	const char * str_result = Tcl_GetString( result );
	v8::Local< v8::String > r_string = Nan::New< v8::String >( str_result ).ToLocalChecked();

	info.GetReturnValue().Set( r_string );

}


void TclBinding::queue( const Nan::FunctionCallbackInfo< v8::Value > &info ) {

	// validate input params
	if ( info.Length() != 2 ) {
		Nan::ThrowError( "Invalid number of arguments" );
	}

	if (! info[0]->IsString() ) {
		Nan::ThrowTypeError( "Tcl command must be a string" );
	}

	if (! info[1]->IsFunction() ) {
		Nan::ThrowTypeError( "Callback must be a function" );
	}


	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );

	// queue the task
	Nan::Utf8String cmd( info[0] );
	Nan::Callback * callback = new Nan::Callback( info[1].As< v8::Function >() );
	binding->_tasks.queue( * cmd, callback );

	info.GetReturnValue().Set( Nan::Undefined() );

}


void TclBinding::toArray( const Nan::FunctionCallbackInfo< v8::Value > &info ) {

	// validate input params
	if ( info.Length() < 1 ) {
		return info.GetReturnValue().Set( Nan::Undefined() );
	}

	if (! info[0]->IsString() ) {
		Nan::ThrowTypeError( "Input must be a string" );
	}

	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( info.Holder() );
	Nan::Utf8String str( info[0] );

	// create a Tcl string object
	Tcl_Obj * obj = Tcl_NewStringObj( *str, strlen( *str ) );

	int objc = 0;
	Tcl_Obj **objv;

	// attempt to parse as a Tcl list
	if ( Tcl_ListObjGetElements( binding->_interp, obj, &objc, &objv ) == TCL_OK ) {

		v8::Local< v8::Array > r_array = Nan::New< v8::Array >( objc );

		for ( int i = 0; i < objc; i++ ) {
			r_array->Set( i, Nan::New< v8::String >( Tcl_GetString( objv[i] ) ).ToLocalChecked() );
		}

		return info.GetReturnValue().Set( r_array );

	}

	// not a valid Tcl list
	info.GetReturnValue().Set( Nan::Null() );

}


