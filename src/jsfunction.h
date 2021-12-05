
#ifndef JSFUNCTION_H
#define JSFUNCTION_H

#include <napi.h>
#include <tcl.h>
#include <string>

class JsFunction {
public:

	JsFunction( Napi::Env, Napi::FunctionReference callback, const char * cmd );
	virtual ~JsFunction();

	void Execute(int argc, char *argv[]);

	static int jsCommand(ClientData, Tcl_Interp *, int, const char **);
	static void jsDelete(ClientData);
private:

	std::string _cmd;
	Napi::Env _env;
	Napi::FunctionReference _callback;
	Napi::AsyncContext _context;

};

#endif /* !JSFUNCTION_H */
