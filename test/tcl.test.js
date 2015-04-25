/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var tcl    = require( '../' );


describe( 'tcl', function () {

	it( 'should have _interp property', function () {
		expect( tcl ).to.have.property( '_interp' );
		expect( tcl._interp ).to.be.an( 'object' );
	} );


	context( 'when there is a valid Tcl object', function () {

		it( 'should be able to get Tcl version', function () {
			var version = parseFloat( tcl.version() );
			expect( isNaN( version ) ).to.be.false;
			expect( version ).to.be.a( 'number' );
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

