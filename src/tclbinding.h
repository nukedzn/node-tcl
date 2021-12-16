
#ifndef TCLBINDING_H
#define TCLBINDING_H

#include <napi.h>
#include <tcl.h>

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
#include "taskrunner.h"
#endif

class TclBinding : public Napi::ObjectWrap<TclBinding> {
public:

	struct CmdDef {
		Napi::Env env;
		Napi::FunctionReference funcRef;
		CmdDef(Napi::Env env) : env(env) {};
	};

	TclBinding(const Napi::CallbackInfo& info);
	~TclBinding();

	Napi::Value cmd		( const Napi::CallbackInfo& info);
	Napi::Value cmdSync	( const Napi::CallbackInfo& info);
	Napi::Value queue	( const Napi::CallbackInfo& info);
	Napi::Value toArray	( const Napi::CallbackInfo& info);
	Napi::Value jsFunc	( const Napi::CallbackInfo& info);
	
	static Napi::Function GetClass(Napi::Env);

private:

	Tcl_Interp * _interp;

#if defined(HAS_CXX11) && defined(HAS_TCL_THREADS)
	TaskRunner * _tasks;
#endif

};

#endif /* !TCLBINDING_H */
