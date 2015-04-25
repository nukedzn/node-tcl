/* jshint expr: true */

'use strict';

var expect  = require( 'chai' ).expect;
var binding = require( 'bindings' )( 'tcl' );
var Result  = require( '../lib/result' );


describe( 'Result', function () {

	// tcl interpreter object
	var interp = new binding.TclInterp();


	it( 'should have a _result property ', function () {
		var result = new Result();
		expect( result ).to.have.property( '_result' );
	} );

	it( 'should have a _interp property', function () {
		var result = new Result( 'data', interp );
		expect( result ).to.have.property( '_interp' );
		expect( result._interp ).to.be.an( 'object' );
	} );

	it( 'should store raw data', function () {
		var data = 'a5b1e83cd0273aefdf9cca2994b97c77';
		var result = new Result( data );
		expect( result._result.raw ).to.be.string( data );
	} );



	context( 'when converting a Tcl list to an Array', function () {

		it( 'should check for internal Tcl interpreter', function () {
			var result = new Result( 'data' );
			var array  = result.toArray();
			expect( array ).to.be.null;
		} );

		it( 'should validate input parameters', function () {
			var result = new Result( null, interp );
			var array  = result.toArray();
			expect( array ).to.be.null;
		} );

		it( 'should convert internal data into an array successfully', function () {
			var result = new Result( '{a x} b c {1 2 {3.1 3.2}}', interp );
			var array  = result.toArray();
			expect( array ).to.an.instanceof( Array );
			expect( array ).to.have.length( 4 );
		} );

		it( 'should convert a string into an array successfully', function () {
			var result = new Result( null, interp );
			var array  = result.toArray( 'data' );
			expect( array ).to.an.instanceof( Array );
			expect( array ).to.eql( [ 'data' ] );
		} );

	} );

} );

