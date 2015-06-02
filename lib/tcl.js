
'use strict';

var binding = require( 'bindings' )( 'tcl' );
var Result  = require( './result' );

/**
*   Tcl Interpreter bridge
*
*   @constructor
*/
var Tcl = function () {

	// tcl interpreter binding
	this._binding = new binding.TclBinding();

	// intialise internals
	this._version = null;

};


/**
*   Execute a Tcl command using the Tcl Interpreter binding. This is an
*   asynchronous call and a new Tcl Interpreter instance is used for
*   each execution.
*
*   @param {string} cmd - command to execute
*   @param {Tcl~cmdCallback} callback - callback method to handle
*          the response
*/
Tcl.prototype.cmd = function ( cmd, callback ) {

	var self = this;

	this._binding.cmd( cmd, function ( err, data ) {

		var result = null;
		if (! err ) {
			result = new Result( data, self._binding );
		}

		if ( callback ) {
			callback( err, result );
		}

	} );

};


/**
*   Alias for {@link Tcl#cmd}
*
*   @method
*   @see Tcl#cmd
*/
Tcl.prototype.eval = Tcl.prototype.cmd;


/**
*   Execute a Tcl command synchronously using the Tcl Interpreter binding.
*   This will use a shared Tcl Interpreter and can share data between two
*   executions (e.g. global variable, loaded libraries etc.)
*
*   @param {string} cmd - Command to execute
*   @return {Result} Result object containing response data
*   @throws {Error} Error object containing error data
*/
Tcl.prototype.cmdSync = function ( cmd ) {

	try {
		return new Result( this._binding.cmdSync( cmd ), this._binding );
	} catch ( e ) {
		throw e;
	}

};


/**
*   Alias for {@link Tcl#cmdSync}
*
*   @method
*   @see Tcl#cmdSync
*/
Tcl.prototype.evalSync = Tcl.prototype.cmdSync;


/**
*   Add a Tcl command to the asynchronous processing queue. Each command
*   added will be executed using a single worker thread outside of the
*   main event loop. Each command shares the same worker thread and the
*   Tcl interpreter instance and will share states.
*
*   @param {string} cmd - command to execute
*   @param {Tcl~cmdCallback} callback - callback method to handle
*          the response
*/
Tcl.prototype.queue = function ( cmd, callback ) {

	this._binding.queue( cmd, function ( err, data ) {

		var result = null;
		if (! err ) {
			result = new Result( data, this._binding );
		}

		if ( callback ) {
			callback( err, result );
		}

	} );

};


/**
*   Returns the Tcl Interpreter version
*
*   @return {string} Tcl version
*/
Tcl.prototype.version = function () {

	if (! this._version ) {
		this._version = this._binding.cmdSync( 'info patchlevel' );
	}

	return this._version;

};



// jsdoc definitions
/**
*   Error first callback method to return error and response data after executing
*   a Tcl command asynchronously
*
*   @callback Tcl~cmdCallback
*   @param {(Error|null)} err - JavaScript Error object with error details upon error or null
*   @param {Result} result - object containing response data from the executed Tcl command
*/


module.exports = Tcl;

