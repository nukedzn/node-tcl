
#include "tclworker.h"
#include <tcl.h>

TclWorker::TclWorker( Napi::Function& callback, const char * cmd )
	: Napi::AsyncWorker( callback ), _cmd( cmd ) {

}


TclWorker::~TclWorker() {

}


void TclWorker::OnOK() {
	Napi::Env env = Env();

	Callback().Call({env.Null(), Napi::String::New(env, _result)});
	return;

}


void TclWorker::Execute() {

	// initialise a new Tcl interpreter for the thread
	Tcl_Interp * interp = Tcl_CreateInterp();

	if ( TCL_OK == Tcl_Init( interp ) ) {

		// evaluate command
		int code = Tcl_EvalEx( interp, _cmd.c_str(), -1, TCL_EVAL_DIRECT );

		if ( code == TCL_ERROR ) {
			SetError( Tcl_GetStringResult( interp ) );
		} else {
			_result = std::string( Tcl_GetStringResult( interp ) );
		}

	} else {
		SetError( "Failed to initialise Tcl interpreter" );
	}

	// cleanup
	Tcl_DeleteInterp( interp );

}
