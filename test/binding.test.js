
'use strict';

var expect  = require( 'chai' ).expect;
var binding = require( 'bindings' )( 'tcl' );
var tclb    = new binding.TclBinding();


describe( 'TclBinding', function () {

	context( 'cmdSync', function () {

		it( 'should validate arguments', function () {
			var result;

			try {
				tclb.cmdSync();
			} catch ( e ) {
				result = e;
			}
			expect( result ).to.be.an.instanceof( Error );
			expect( result.message ).to.be.string( 'Require a Tcl command to execute' );
		} );

	} );

} );

