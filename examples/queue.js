
'use strict';

var tcl = require( '../' );

console.log( 'requesting to set x @', Date.now() );

tcl.queue( 'after 2000; set x [clock milliseconds]; puts "x set as $x"' );
tcl.queue( 'incr x', function ( err, result ) {
	console.log( 'after incr x is', result.data() );
} );

console.log( 'waiting @', Date.now() );


