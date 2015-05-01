/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var tcl    = require( '../' );


describe( 'tcl', function () {

	it( 'should have _interp property', function () {
		expect( tcl ).to.have.property( '_interp' );
		expect( tcl._interp ).to.be.an( 'object' );
	} );

	it( 'should be able to return Tcl version', function () {
		var version = parseFloat( tcl.version() );
		expect( isNaN( version ) ).to.be.false;
		expect( version ).to.be.a( 'number' );
	} );


	context( 'when executing a Tcl command asynchronously', function () {
		it( 'should return a response object', function ( done ) {
			tcl.cmd( 'info nameofexecutable', function ( err, result ) {
				var Result = require( '../lib/result' );
				expect( err ).to.be.null;
				expect( result ).to.be.an.instanceof( Result );
				done();
			} );
		} );

		it( 'should return response data', function ( done ) {
			tcl.cmd( 'info cmdcount', function ( err, result ) {
				expect( err ).to.be.null;
				expect( result.data() ).to.be.a( 'string' );
				expect( isNaN( parseInt( result.data() ) ) ).to.be.false;
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

		it( 'should be possible to use the method alias', function ( done ) {
			tcl.eval( 'info cmdcount', function ( err, result ) {
				expect( err ).to.be.null;
				expect( result.data() ).to.be.a( 'string' );
				expect( isNaN( parseInt( result.data() ) ) ).to.be.false;

				tcl.eval( 'error {test error}', function ( err, data ) {
					expect( err ).to.be.an.instanceof( Error );
					expect( err.message ).to.be.string( 'test error' );
					done();
				} );
			} );
		} );
	} );


	context( 'when executing a Tcl command synchronously', function () {
		it( 'should return a response object', function () {
			var Result = require( '../lib/result' );
			var result = tcl.cmdSync( 'info nameofexecutable' );
			expect( result ).to.be.an.instanceof( Result );
		} );

		it( 'should return response data', function () {
			var result = tcl.cmdSync( 'info cmdcount' );
			expect( result.data() ).to.be.a( 'string' );
			expect( isNaN( parseInt( result.data() ) ) ).to.be.false;
		} );

		it( 'should handle errors', function () {
			var err = tcl.cmdSync( 'error {test error}' );
			expect( err ).to.be.an.instanceof( Error );
			expect( err.message ).to.be.string( 'test error' );
		} );

		it( 'should be possible to use the method alias', function () {
			var result = tcl.evalSync( 'info cmdcount' );
			expect( result.data() ).to.be.a( 'string' );
			expect( isNaN( parseInt( result.data() ) ) ).to.be.false;

			var err = tcl.evalSync( 'error {test error}' );
			expect( err ).to.be.an.instanceof( Error );
			expect( err.message ).to.be.string( 'test error' );
		} );
	} );

} );
