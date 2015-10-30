/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var path   = require( 'path' );
var sinon  = require( 'sinon' );
var tcl    = require( '../' );


describe( 'tcl', function () {

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

	it( 'should inject tcl commands as javascript functions', function () {
		expect( tcl.$.info ).to.be.a.method;
		expect( tcl.$.set ).to.be.a.method;
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
			var err = null;
			try {
				tcl.cmdSync( 'error {test error}' );
			} catch ( e ) {
				err = e;
			}

			expect( err ).to.be.an.instanceof( Error );
			expect( err.message ).to.be.string( 'test error' );
		} );

		it( 'should be possible to use the method alias', function () {
			var result = tcl.evalSync( 'info cmdcount' );
			expect( result.data() ).to.be.a( 'string' );
			expect( isNaN( parseInt( result.data() ) ) ).to.be.false;

			var err = null;
			try {
				tcl.evalSync( 'error {test error}' );
			} catch ( e ) {
				err = e;
			}

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

				tcl.queue( 'incr x', function ( err, result ) {
					expect( err ).to.be.null;
					expect( parseInt( result.data() ) ).to.equal( 1 );
					done();
				} );
			} );
		} );

	} );


	context( 'when executing injected javascript functions', function () {
		it( 'should pass arguments correctly', function () {
			var hostname = tcl.$( 'info hostname' ).data();
			var result = tcl.$.set( 'hostname', '[info hostname]' );
			expect( result.data() ).to.eql( hostname );

			tcl.$.proc( 'add', '{x y}', '{return [expr {$x + $y}]}' );
			tcl.$inject();

			result = tcl.$.add( 1, 2 );
			expect( parseInt( result.data() ) ).to.eql( 3 );
		} );
	} );


	context( 'when sourcing tcl files', function () {
		it( 'should update internal tcl command references', function () {
			tcl.source( path.join( __dirname, 'support/script.tcl' ) );
			expect( tcl.$.multiply ).to.be.a.method;

			var result = tcl.$.multiply( 5, 10 );
			expect( parseInt( result.data() ) ).to.eql( 50 );
		} );
	} );


	context( 'when loading tcl modules', function () {
		it( 'should update internal tcl command references', function () {
			sinon.stub( tcl, '$' );
			sinon.stub( tcl, '$inject' );

			tcl.load( 'libdummy.so' );
			expect( tcl.$.calledOnce ).to.be.true;
			expect( tcl.$inject.calledOnce ).to.be.true;

			tcl.$.restore();
			tcl.$inject.restore();
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

