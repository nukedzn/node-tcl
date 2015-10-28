
#include "taskrunner.h"
#include <tcl.h>


TaskRunner::TaskRunner() : _terminate( false ) {

	// initialise worker thread
	_worker = std::thread( &TaskRunner::worker, this );

}


TaskRunner::~TaskRunner() {

	// flag termination
	_terminate = true;

	// notify worker
	_worker_cnd.notify_all();

	// wait for the worker thread
	_worker.join();

}


void TaskRunner::queue( const char * cmd, Nan::Callback * callback ) {

	task_t task = {
		cmd,
		new AsyncHandler( callback )
	};

	{
		std::unique_lock< std::mutex > lock( _task_mutex );
		_tasks.push( task );

		// schedule an async worker outside of main event loop
		Nan::AsyncQueueWorker( task.handler );
	}

	// notify worker thread
	_worker_cnd.notify_one();

}


void TaskRunner::worker() {

	task_t task;

	Tcl_Interp * interp = Tcl_CreateInterp();
	int status = Tcl_Init( interp );

	while (! _terminate ) {

		{
			std::unique_lock< std::mutex > lock( _task_mutex );
			while ( (! _terminate ) && _tasks.empty() ) {
				_worker_cnd.wait( lock );
			}

			if ( _terminate && _tasks.empty() ) {
				break;
			}

			task = _tasks.front();
			_tasks.pop();
		}


		if ( status == TCL_OK ) {

			if ( Tcl_EvalEx( interp, task.cmd.c_str(), -1, 0 ) == TCL_ERROR ) {
				task.handler->notify( Tcl_GetStringResult( interp ), "" );
			} else {
				task.handler->notify( "", Tcl_GetStringResult( interp ) );
			}

		} else {
			task.handler->notify( "Failed to initialise Tcl interpreter", "" );
		}

	}

	// cleanup
	Tcl_DeleteInterp( interp );

}

