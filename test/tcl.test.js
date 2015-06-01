/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var tcl    = require( '../' );


describe( 'tcl', function () {

	it( 'should have _binding property', function () {
		expect( tcl ).to.have.property( '_binding' );
		expect( tcl._binding ).to.be.an( 'object' );
	} );

	it( 'should be able to return Tcl version', function () {
		var version = tcl.version();
		var re      = /^(\d+)\.(\d+)\.(\d+)$/;
		var match   = version.match( re );

		expect( match ).to.be.an.instanceof( Array );
	} );

	it( 'should cache Tcl version', function () {
		tcl._version = '1.0-beta';
		expect( tcl.version() ).to.equal( '1.0-beta' );
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

		it( 'should be possible to convert resonse data to an array', function ( done ) {
			tcl.eval( 'info commands', function ( err, result ) {
				expect( err ).to.be.null;
				expect( result.data() ).to.be.a( 'string' );
				expect( result.toArray() ).to.be.an.instanceof( Array );
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

		it( 'should be possible to use the method aliases', function ( done ) {
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


	context( 'when using async queue to execute Tcl commands', function () {

		it( 'should return response data', function ( done ) {
			tcl.queue( 'info cmdcount', function ( err, result ) {
				var Result = require( '../lib/result' );
				expect( err ).to.be.null;
				expect( result ).to.be.an.instanceof( Result );
				expect( result.data() ).to.be.a( 'string' );
				expect( isNaN( parseInt( result.data() ) ) ).to.be.false;
				done();
			} );
		} );

		it( 'should handle errors', function ( done ) {
			tcl.queue( 'error {test error}', function ( err, data ) {
				expect( err ).to.be.an.instanceof( Error );
				expect( err.message ).to.be.string( 'test error' );
				done();
			} );
		} );

		it( 'should share state between commands', function ( done ) {
			tcl.queue( 'set x 0', function ( err, result ) {
				expect( err ).to.be.null;

				tcl.queue( 'incr $x', function ( err, result ) {
					expect( err ).to.be.null;
					expect( parseInt( result.data() ) ).to.equal( 1 );
					done();
				} );
			} );
		} );

	} );

} );



describe( 'Tcl', function () {

	it( 'should have a constructor method', function () {
		expect( tcl.Tcl ).to.be.a( 'function' );
	} );


	it( 'should be possible to create a new instance using the constructor method', function ( done ) {

		// set a var in global instance
		var result = tcl.cmdSync( 'set leak {leaked}' );
		expect( result.data() ).to.equal( 'leaked' );

		// create a new instance
		var t = new tcl.Tcl();

		// check for leaks
		t.cmd( 'set val $leak', function ( err, data ) {
			expect( err ).to.be.an.instanceof( Error );
			expect( data ).to.be.null;
			done();
		} );

	} );

} );

