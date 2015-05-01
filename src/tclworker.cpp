
#include "tclworker.h"
#include <tcl.h>


TclWorker::TclWorker( const char * cmd, NanCallback * callback )
	: NanAsyncWorker( callback ), _cmd( cmd ) {

}


TclWorker::~TclWorker() {

}


void TclWorker::HandleOKCallback() {

	NanScope();

	if (! ErrorMessage() ) {

		v8::Local< v8::Value > argv[] = {
			NanNull(),
			NanNew< v8::String >( _result )
		};

		callback->Call( 2, argv );
		return;

	}

	// invalid result
	v8::Local< v8::Value > argv[] = {
		v8::Exception::Error( NanNew< v8::String >( "Invalid Tcl result object" ) )
	};

	callback->Call( 1, argv );

}


void TclWorker::Execute() {

	// initialise a new Tcl interpreter for the thread
	Tcl_Interp * interp = Tcl_CreateInterp();

	if ( TCL_OK == Tcl_Init( interp ) ) {

		// evaluate command
		int code = Tcl_EvalEx( interp, _cmd.c_str(), -1, TCL_EVAL_DIRECT );

		if ( code == TCL_ERROR ) {
			SetErrorMessage( Tcl_GetStringResult( interp ) );
		} else {
			_result = std::string( Tcl_GetStringResult( interp ) );
		}

	} else {
		SetErrorMessage( "Failed to initialise Tcl interpreter" );
	}

	// cleanup
	Tcl_DeleteInterp( interp );

}

