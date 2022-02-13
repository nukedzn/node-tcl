/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
var tcl    = require( '../' );

describe( 'Call back to JS function', function () {

	it( 'should be able to define a new Tcl command', function (done) {
		tcl.proc('abc', (...args) => {
			expect( args.length ).to.equal( 2 );
			expect( args[0] ).to.equal( 'hi' );
			expect( args[1] ).to.equal( 'there' );
			done();
		});
		tcl.cmdSync('abc hi there');
	});

	it( 'should be able to define distinct Tcl commands', function (done) {
		tcl.proc('qrs', (...args) => {
			expect( args[0] ).to.equal( 'what' );
			expect( args[1] ).to.equal( 'up' );
		});

		tcl.proc('xyz', (...args) => {
			expect( args[0] ).to.equal( 'ho' );
			expect( args[1] ).to.equal( 'here' );
			done();
		});

		tcl.cmdSync('qrs what up');
		tcl.cmdSync('xyz ho here');
	});

} );

