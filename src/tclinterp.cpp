
#include "tclinterp.h"


// initialise static vars
v8::Persistent<v8::Function> TclInterp::constructor;

TclInterp::TclInterp() {

	// initialise Tcl interpreter
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		NanThrowError( "Failed to initialise Tcl interpreter" );
	}

}


TclInterp::~TclInterp() {
	// cleanup
	Tcl_DeleteInterp( _interp );
}


void TclInterp::init( v8::Handle< v8::Object > exports ) {

	NanScope();

	// prepare constructor template
	v8::Local< v8::FunctionTemplate > tpl = NanNew< v8::FunctionTemplate >( construct );
	tpl->SetClassName( NanNew( "TclInterp" ) );
	tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

	// prototypes
	NODE_SET_PROTOTYPE_METHOD( tpl, "cmd", cmd );

	NanAssignPersistent( constructor, tpl->GetFunction() );
	exports->Set( NanNew( "TclInterp" ), tpl->GetFunction() );

}


NAN_METHOD( TclInterp::construct ) {

	NanScope();

	if (! args.IsConstructCall() ) {

		// invoked as `TclInterp(...)`, convert to a constructor call
		v8::Local< v8::Function > c = NanNew< v8::Function >( constructor );
		NanReturnValue( c->NewInstance() );

	}

	TclInterp * obj = new TclInterp();
	obj->Wrap( args.This() );
	NanReturnValue( args.This() );

}


NAN_METHOD( TclInterp::cmd ) {

	NanScope();

	// validate input params
	if ( args.Length() < 1 ) {
		NanThrowError( "Require a Tcl command to execute" );
	}

	if (! args[0]->IsString() ) {
		NanThrowTypeError( "Tcl command must be a string" );
	}


	TclInterp * obj = ObjectWrap::Unwrap< TclInterp >( args.Holder() );
	NanUtf8String cmd( args[0] );

	// evaluate command
	int code = Tcl_Eval( obj->_interp, *cmd );

	// check for errors
	if ( code == TCL_ERROR ) {
		NanThrowError( Tcl_GetStringResult( obj->_interp ) );
	}


	// grab the result
	Tcl_Obj * result = Tcl_GetObjResult( obj->_interp );

	// return result as a string
	const char * str_result = Tcl_GetString( result );
	v8::Local< v8::String > r_string = NanNew< v8::String >( str_result );

	NanReturnValue( r_string );

}


