
'use strict';

var path = require( 'path' );
var tcl  = require( '../' );

tcl.source( path.join( __dirname, '../test/support/script.tcl' ) );

// New procs that are not within a namespace from the sourced file will be injected automatically
console.log( '3.14 * 1.77 =', tcl.$.multiply( 3.14, 1.77 ).data() );

