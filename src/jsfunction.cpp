
#include "jsfunction.h"

JsFunction::JsFunction( Napi::Env env, Napi::FunctionReference callback, const char * cmd ) 
	: _context(env, cmd), _env(env), _cmd(cmd), _callback(callback) {
//	_cmd = cmd;
//	_callback = callback;
}


JsFunction::~JsFunction() {
//	delete _callback;

}

void JsFunction::Execute(int argc, char *argv[]) {
//	Napi::Env env = Env();
//	Napi::String str = Napi::String::New(env, "hi");
//	Napi::Number num = Napi::Number::New(env, 1);
//
//	_callback.MakeCallback(env, num, str, _context);

}

int JsFunction::jsCommand(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[]) {
//	std::string *cs = (std::string *)clientData;
//	const Napi::CallbackInfo *info = ct->info;
//	Napi::Env env = info->Env();
//	Napi::Function func = ct->func;
//	Napi::Value args = Napi::String::New(env, "Hi");
//	Napi::FunctionReference *func = (Napi::FunctionReference *)clientData;
//	JsFunction *jf = (JsFunction *)clientData;
	
	printf("C:%p E:\n", clientData);
//	func->Call({});
	return TCL_OK;
}

void JsFunction::jsDelete(ClientData clientData) {
	printf("D: %p\n", clientData);
}

