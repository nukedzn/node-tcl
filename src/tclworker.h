
#ifndef TCLWORKER_H
#define TCLWORKER_H

#include <napi.h>
#include <string>

class TclWorker : public Napi::AsyncWorker {
public:

	TclWorker( Napi::Function& callback, const char * cmd );
	virtual ~TclWorker();

	void Execute();
	void OnOK();

private:

	std::string _cmd;
	std::string _result;

};

#endif /* !TCLWORKER_H */
