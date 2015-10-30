
'use strict';

var tcl = require( '../' );

tcl.eval( 'after 1500; error {some error message}', function ( err, result ) {
	console.log( err );
} );

console.log( 'waiting for tcl worker thread...' );

