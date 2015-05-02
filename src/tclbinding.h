
#ifndef TCLBINDING_H
#define TCLBINDING_H

#include <nan.h>
#include <tcl.h>

#include "taskrunner.h"


class TclBinding : public node::ObjectWrap {
public:
	static void init( v8::Handle< v8::Object > exports );

private:
	explicit TclBinding();
	~TclBinding();

	static NAN_METHOD( construct );
	static NAN_METHOD( cmd );
	static NAN_METHOD( cmdSync );
	static NAN_METHOD( queue );
	static NAN_METHOD( toArray );

	static v8::Persistent<v8::Function> constructor;

	Tcl_Interp * _interp;

	TaskRunner _tasks;

};


// node addon initialisation
NODE_MODULE( tcl, TclBinding::init )

#endif /* !TCLBINDING_H */

