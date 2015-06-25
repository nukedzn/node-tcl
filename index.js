
'use strict';


/**
*   Tcl module with native Tcl Interpreter binding
*   @module tcl
*/

var Tcl = require( './lib/tcl' );
var tcl = new Tcl();


/**
*   Exports a singleton
*
*   @type {Tcl}
*/
module.exports = tcl;

/**
*   Exports a constructor
*
*   @type {Tcl}
*/
module.exports.Tcl = Tcl;

