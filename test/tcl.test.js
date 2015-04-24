/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var Tcl    = require( '../' );


describe( 'tcl', function () {

	it( 'should be able to create a new Tcl object', function () {
		var tcl = new Tcl();
		expect( tcl ).to.have.property( '_interp' );
		expect( tcl._interp ).to.be.an( 'object' );
	} );


	context( 'when there is a valid Tcl object', function () {

		// tcl object placeholder
		var tcl = {};

		beforeEach( function () {
			tcl = new Tcl();
		} );

		it( 'should be able to get Tcl version', function ( done ) {
			tcl.cmd( 'info tclversion', function ( err, data ) {
				var version = parseFloat( data );

				expect( err ).to.be.null;
				expect( isNaN( version ) ).to.be.false;
				expect( version ).to.be.a( 'number' );
				done();
			} );
		} );

		it( 'should handle errors', function ( done ) {
			tcl.cmd( 'error {test error}', function ( err, data ) {
				expect( err ).to.be.an.instanceof( Error );
				expect( err.message ).to.be.string( 'test error' );
				done();
			} );
		} );
	} );

} );

