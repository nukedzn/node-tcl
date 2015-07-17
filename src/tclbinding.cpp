
#include "tclbinding.h"
#include "tclworker.h"
#include <cstring>


// initialise static vars
v8::Persistent<v8::Function> TclBinding::constructor;

TclBinding::TclBinding() {

	// initialise Tcl interpreter
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		NanThrowError( "Failed to initialise Tcl interpreter" );
	}

}


TclBinding::~TclBinding() {
	// cleanup
	Tcl_DeleteInterp( _interp );
}


void TclBinding::init( v8::Handle< v8::Object > exports ) {

	NanScope();

	// prepare constructor template
	v8::Local< v8::FunctionTemplate > tpl = NanNew< v8::FunctionTemplate >( construct );
	tpl->SetClassName( NanNew( "TclBinding" ) );
	tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

	// prototypes
	NODE_SET_PROTOTYPE_METHOD( tpl, "cmd", cmd );
	NODE_SET_PROTOTYPE_METHOD( tpl, "cmdSync", cmdSync );
	NODE_SET_PROTOTYPE_METHOD( tpl, "queue", queue );
	NODE_SET_PROTOTYPE_METHOD( tpl, "toArray", toArray );

	NanAssignPersistent( constructor, tpl->GetFunction() );
	exports->Set( NanNew( "TclBinding" ), tpl->GetFunction() );

}


NAN_METHOD( TclBinding::construct ) {

	NanScope();

	if (! args.IsConstructCall() ) {

		// invoked as `TclBinding(...)`, convert to a constructor call
		v8::Local< v8::Function > c = NanNew< v8::Function >( constructor );
		NanReturnValue( c->NewInstance() );
		return;

	}

	TclBinding * obj = new TclBinding();
	obj->Wrap( args.This() );
	NanReturnValue( args.This() );

}


NAN_METHOD( TclBinding::cmd ) {

	NanScope();

	// validate input params
	if ( args.Length() != 2 ) {
		NanThrowError( "Invalid number of arguments" );
	}

	if (! args[0]->IsString() ) {
		NanThrowTypeError( "Tcl command must be a string" );
	}

	if (! args[1]->IsFunction() ) {
		NanThrowTypeError( "Callback must be a function" );
	}


	// schedule an async task
	NanUtf8String cmd( args[0] );
	NanCallback * callback = new NanCallback( args[1].As< v8::Function >() );
	NanAsyncQueueWorker( new TclWorker( *cmd, callback ) );

	NanReturnUndefined();

}


NAN_METHOD( TclBinding::cmdSync ) {

	NanScope();

	// validate input params
	if ( args.Length() < 1 ) {
		NanThrowError( "Require a Tcl command to execute" );
	}

	if (! args[0]->IsString() ) {
		NanThrowTypeError( "Tcl command must be a string" );
	}


	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( args.Holder() );
	NanUtf8String cmd( args[0] );

	// evaluate command
	int code = Tcl_EvalEx( binding->_interp, *cmd, -1, 0 );

	// check for errors
	if ( code == TCL_ERROR ) {
		NanThrowError( Tcl_GetStringResult( binding->_interp ) );
	}


	// grab the result
	Tcl_Obj * result = Tcl_GetObjResult( binding->_interp );

	// return result as a string
	const char * str_result = Tcl_GetString( result );
	v8::Local< v8::String > r_string = NanNew< v8::String >( str_result );

	NanReturnValue( r_string );

}


NAN_METHOD( TclBinding::queue ) {

	NanScope();

	// validate input params
	if ( args.Length() != 2 ) {
		NanThrowError( "Invalid number of arguments" );
	}

	if (! args[0]->IsString() ) {
		NanThrowTypeError( "Tcl command must be a string" );
	}

	if (! args[1]->IsFunction() ) {
		NanThrowTypeError( "Callback must be a function" );
	}


	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( args.Holder() );

	// queue the task
	NanUtf8String cmd( args[0] );
	NanCallback * callback = new NanCallback( args[1].As< v8::Function >() );
	binding->_tasks.queue( * cmd, callback );

	NanReturnUndefined();

}


NAN_METHOD( TclBinding::toArray ) {

	NanScope();

	// validate input params
	if ( args.Length() < 1 ) {
		NanReturnUndefined();
	}

	if (! args[0]->IsString() ) {
		NanThrowTypeError( "Input must be a string" );
	}

	TclBinding * binding = ObjectWrap::Unwrap< TclBinding >( args.Holder() );
	NanUtf8String str( args[0] );

	// create a Tcl string object
	Tcl_Obj * obj = Tcl_NewStringObj( *str, strlen( *str ) );

	int objc = 0;
	Tcl_Obj **objv;

	// attempt to parse as a Tcl list
	if ( Tcl_ListObjGetElements( binding->_interp, obj, &objc, &objv ) == TCL_OK ) {

		v8::Local< v8::Array > r_array = NanNew< v8::Array >( objc );

		for ( int i = 0; i < objc; i++ ) {
			r_array->Set( i, NanNew< v8::String >( Tcl_GetString( objv[i] ) ) );
		}

		NanReturnValue( r_array );

	}

	// not a valid Tcl list
	NanReturnNull();

}


