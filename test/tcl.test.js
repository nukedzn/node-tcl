
'use strict';

var Tcl = require( '../' );

describe( 'tcl', function () {

	it( 'should be able to return an object', function ( done ) {
		var tcl = new Tcl();
		done();
	} );


	it( 'should be able to execute a tcl command', function ( done ) {
		var tcl = new Tcl();
		tcl.cmd( 'info commands', done );
	} )

} );

