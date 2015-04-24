
#include <nan.h>
#include "tclinterp.h"


void init( v8::Handle< v8::Object > exports ) {
	TclInterp::init( exports );
}

NODE_MODULE( tcl, init )

