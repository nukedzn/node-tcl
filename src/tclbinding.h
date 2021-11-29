
#ifndef TCLBINDING_H
#define TCLBINDING_H

#include <napi.h>
#include <tcl.h>

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
#include "taskrunner.h"
#endif


class TclBinding : public Napi::ObjectWrap<TclBinding> {
public:
	TclBinding(const Napi::CallbackInfo& info);
	~TclBinding();

	Napi::Value cmd( const Napi::CallbackInfo& info);
	Napi::Value cmdSync( const Napi::CallbackInfo& info);
//	Napi::Value queue( const Napi::CallbackInfo& info);
	Napi::Value toArray( const Napi::CallbackInfo& info);
	
	static Napi::Function GetClass(Napi::Env);

private:

	Tcl_Interp * _interp;

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	int * _tasks;
//	TaskRunner * _tasks;
#endif

};

#endif /* !TCLBINDING_H */
