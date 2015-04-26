
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

	var err = null;
	var result = null;

	try {
		result = new Result( this._interp.cmd( cmd ), this._interp );
	} catch ( e ) {
		err = e;
	}

	if ( callback ) {
		callback( err, result );
	}

};


/**
*   Returns the Tcl Interpreter version
*   @return {string} Tcl version
*/
Tcl.prototype.version = function () {

	if (! this._version ) {
		this._version = this._interp.cmd( 'info tclversion' );
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

