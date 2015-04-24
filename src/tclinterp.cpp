
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

	// TODO: prototypes

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

