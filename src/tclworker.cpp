
#include "tclworker.h"


TclWorker::TclWorker( const char * cmd, NanCallback * callback )
	: NanAsyncWorker( callback ), _cmd( cmd ) {

	// initialise a new Tcl interpreter for the thread
	_interp = Tcl_CreateInterp();
	if ( TCL_OK != Tcl_Init( _interp ) ) {
		SetErrorMessage( "Failed to initialise Tcl interpreter" );
	}

}


TclWorker::~TclWorker() {

	// cleanup
	Tcl_DeleteInterp( _interp );

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

	if ( TCL_OK == Tcl_Init( _interp ) ) {

		// evaluate command
		int code = Tcl_Eval( _interp, _cmd.c_str() );

		if ( code == TCL_ERROR ) {
			SetErrorMessage( Tcl_GetStringResult( _interp ) );
		} else {
			_result = std::string( Tcl_GetStringResult( _interp ) );
		}

	}

}

