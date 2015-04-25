
'use strict';

var binding = require( 'bindings' )( 'tcl' );


var Tcl = function () {

	// tcl interpreter binding
	this._interp = new binding.TclInterp();

	// intialise internals
	this._version = null;

};


Tcl.prototype.load = function () { };


Tcl.prototype.cmd = function ( cmd, callback ) {

	var err = null;
	var result = null;

	try {
		result = this._interp.cmd( cmd );
	} catch ( e ) {
		err = e;
	}

	if ( callback ) {
		callback( err, result );
	}

};


Tcl.prototype.version = function () {

	if (! this._version ) {
		this._version = this._interp.cmd( 'info tclversion' );
	}

	return this._version;

};


var tcl = new Tcl();

module.exports = tcl;

