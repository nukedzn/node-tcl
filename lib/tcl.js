
'use strict';

var binding = require( 'bindings' )( 'tcl' );


var Tcl = function () {
	this._interp = new binding.TclInterp();
};


Tcl.prototype.load = function () { };


Tcl.prototype.cmd = function () {};


module.exports = Tcl;

