
'use strict';

var binding = require( 'bindings' )( 'tcl' );


var Tcl = function () {
	this._interp = new binding.TclInterp();
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


module.exports = Tcl;

