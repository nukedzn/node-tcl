
#ifndef TCLWORKER_H
#define TCLWORKER_H

#include <nan.h>
#include <tcl.h>
#include <string>


class TclWorker : public NanAsyncWorker {
public:

	TclWorker( const char * cmd, NanCallback * callback );
	~TclWorker();

	void Execute();

protected:

	void HandleOKCallback();


private:

	Tcl_Interp  * _interp;

	std::string _cmd;
	std::string _result;

};

#endif /* !TCLWORKER_H */

