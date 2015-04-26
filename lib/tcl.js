
'use strict';

var binding = require( 'bindings' )( 'tcl' );
var Result  = require( './result' );

/**
*   Tcl Interpreter bridge
*   @constructor
*/
var Tcl = function () {

	// tcl interpreter binding
	this._interp = new binding.TclInterp();

	// intialise internals
	this._version = null;

};


Tcl.prototype.load = function () { };


/**
*   Execute a Tcl command using the Tcl Interpreter
*   binding
*   @param {string} cmd - command to execute
*   @param {Tcl~cmdCallback} callback - callback method to handle
*          the response
*/
Tcl.prototype.cmd = function ( cmd, callback ) {

	this._interp.cmd( cmd, function ( err, data ) {

		var result = null;
		if (! err ) {
			result = new Result( data, this._interp );
		}

		if ( callback ) {
			callback( err, result );
		}

	} );

};


/**
*   Execute a Tcl command synchronously using the Tcl Interpreter binding.
*   This will use a shared Tcl Interpreter and can share data between two
*   executions (e.g. global variable, loaded libraries etc.)
*
*   @param {string} cmd - Command to execute
*   @return {(Result|Error)} Result object containing response data
*           or Error object containing error data
*/
Tcl.prototype.cmdSync = function ( cmd ) {

	try {
		return new Result( this._interp.cmdSync( cmd ), this._interp );
	} catch ( e ) {
		return e;
	}

};


/**
*   Returns the Tcl Interpreter version
*   @return {string} Tcl version
*/
Tcl.prototype.version = function () {

	if (! this._version ) {
		this._version = this._interp.cmdSync( 'info tclversion' );
	}

	return this._version;

};



// jsdoc definitions
/**
*   Error first callback method to return error and response data after executing
*   a Tcl command
*   @callback Tcl~cmdCallback
*   @param {(Error|null)} err - JavaScript Error object with error details upon error or null
*   @param {Result} result - object containing response data from the executed Tcl command
*/



/**
*   Tcl module with native Tcl Interpreter binding
*   @module tcl
*/
var tcl = new Tcl();


/**
*   Exports tcl module
*/
module.exports = tcl;

