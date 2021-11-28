
#ifndef TCLBINDING_H
#define TCLBINDING_H

#include <napi.h>
#include <tcl.h>

//#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
//#include "taskrunner.h"
//#endif


class TclBinding : public Napi::ObjectWrap<TclBinding> {
public:
	Napi::Object init( Napi::Env env, Napi::Object target );
	TclBinding(const Napi::CallbackInfo& info);
	~TclBinding();

private:

//	static void construct( const Nan::FunctionCallbackInfo< v8::Value > &info );
//	static void cmd( const Nan::FunctionCallbackInfo< v8::Value > &info );
//	static void cmdSync( const Nan::FunctionCallbackInfo< v8::Value > &info );
//	static void queue( const Nan::FunctionCallbackInfo< v8::Value > &info );
//	static void toArray( const Nan::FunctionCallbackInfo< v8::Value > &info );
//	static Nan::Persistent< v8::Function > constructor;

	Tcl_Interp * _interp;

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
//	TaskRunner * _tasks;
#endif

};


// node addon initialisation
NODE_API_MODULE( tcl, TclBinding::init )

#endif /* !TCLBINDING_H */
