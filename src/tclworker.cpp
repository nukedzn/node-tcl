
#include "tclworker.h"
#include <tcl.h>


TclWorker::TclWorker( const char * cmd, Nan::Callback * callback )
	: Nan::AsyncWorker( callback ), _cmd( cmd ) {

}


TclWorker::~TclWorker() {

}


void TclWorker::HandleOKCallback() {

	// stack-allocated handle scope
	Nan::HandleScope scope;

	v8::Local< v8::Value > argv[] = {
		Nan::Null(),
		Nan::New< v8::String >( _result ).ToLocalChecked()
	};

	callback->Call( 2, argv );
	return;

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

