
'use strict';


/**
*   Tcl module with native Tcl Interpreter binding
*   @module tcl
*/

var Tcl = require( './lib/tcl' );
var tcl = new Tcl();


/**
*   Exports functional invocation
*/
module.exports = tcl;

/**
*   Exports constructor invocation
*
*   @type {Tcl}
*/
module.exports.Tcl = Tcl;

