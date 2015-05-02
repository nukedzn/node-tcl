
#ifndef TCLWORKER_H
#define TCLWORKER_H

#include <nan.h>
#include <string>


class TclWorker : public NanAsyncWorker {
public:

	TclWorker( const char * cmd, NanCallback * callback );
	virtual ~TclWorker();

	void Execute();

protected:

	void HandleOKCallback();


private:

	std::string _cmd;
	std::string _result;

};

#endif /* !TCLWORKER_H */

