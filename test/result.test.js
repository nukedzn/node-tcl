/* jshint expr: true */

'use strict';

var expect  = require( 'chai' ).expect;
var Result  = require( '../lib/result' );


describe( 'Result', function () {

	// tcl interpreter binding
	var binding = new require( 'bindings' )( 'tcl' ).TclBinding();


	it( 'should have a _result property ', function () {
		var result = new Result();
		expect( result ).to.have.property( '_result' );
	} );

	it( 'should have a _binding property', function () {
		var result = new Result( 'data', binding );
		expect( result ).to.have.property( '_binding' );
		expect( result._binding ).to.be.an( 'object' );
	} );

	it( 'should store raw data', function () {
		var data = 'a5b1e83cd0273aefdf9cca2994b97c77';
		var result = new Result( data );
		expect( result._result.raw ).to.be.string( data );
	} );



	context( 'when converting a Tcl list to an Array', function () {

		it( 'should check for internal Tcl binding', function () {
			var result = new Result( 'data' );
			var array  = result.toArray();
			expect( array ).to.be.null;
		} );

		it( 'should validate input parameters', function () {
			var result = new Result( null, binding );
			var array  = result.toArray();
			expect( array ).to.be.null;
		} );

		it( 'should convert internal data into an array successfully', function () {
			var result = new Result( '{a x} b c {1 2 {3.1 3.2}}', binding );
			var array  = result.toArray();
			expect( array ).to.an.instanceof( Array );
			expect( array ).to.have.length( 4 );
		} );

		it( 'should convert a string into an array successfully', function () {
			var result = new Result( null, binding );
			var array  = result.toArray( 'data' );
			expect( array ).to.an.instanceof( Array );
			expect( array ).to.eql( [ 'data' ] );
		} );

	} );

} );

