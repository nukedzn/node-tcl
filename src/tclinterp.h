
#ifndef TCLINTERP_H
#define TCLINTERP_H

#include <nan.h>
#include <tcl.h>


class TclInterp : public node::ObjectWrap {
public:
	static void init( v8::Handle< v8::Object > exports );

private:
	explicit TclInterp();
	~TclInterp();

	static NAN_METHOD( construct );
	static NAN_METHOD( cmd );
	static NAN_METHOD( toArray );

	static v8::Persistent<v8::Function> constructor;

	Tcl_Interp * _interp;

};

#endif /* !TCLINTERP_H */

