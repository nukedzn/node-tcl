
#ifndef TCLWORKER_H
#define TCLWORKER_H

#include <nan.h>
#include <string>


class TclWorker : public Nan::AsyncWorker {
public:

	TclWorker( const char * cmd, Nan::Callback * callback );
	virtual ~TclWorker();

	void Execute();

protected:

	void HandleOKCallback();


private:

	std::string _cmd;
	std::string _result;

};

#endif /* !TCLWORKER_H */

