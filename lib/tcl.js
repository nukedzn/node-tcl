
'use strict';

var binding = require( 'bindings' )( 'tcl' );


var Tcl = function () {
	this._interp = new binding.TclInterp();
};


Tcl.prototype.load = function () { };


Tcl.prototype.cmd = function ( cmd, callback ) {

	var result = this._interp.cmd( cmd );

	if ( callback ) {
		callback( null, result );
	}

};


module.exports = Tcl;

