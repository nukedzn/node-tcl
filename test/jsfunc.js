/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var tcl    = require( '../' );

describe( 'Call back to JS function', function () {

	it( 'should be able to define new Tcl command', function (done) {
		var res = tcl.jsFunc('xyz', (...args) => {
			expect( args.length ).to.equal( 2 );
			expect( args[0] ).to.equal( 'hi' );
			expect( args[1] ).to.equal( 'there' );
			done();
		});
		tcl.cmdSync('xyz hi there');
	});

} );

