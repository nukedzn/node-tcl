/* jshint expr: true */

'use strict';

var expect = require( 'chai' ).expect;
//var path   = require( 'path' );
//var sinon  = require( 'sinon' );
var tcl    = require( '../' );

//const T = require('tcl');
//T.jsProc('zzz', (a,b,c) => {console.log(a,b,c)});
//T.cmdSync('zzz hi there you');

describe( 'Call back to JS functino', function () {

	it( 'should be able to define new Tcl command', function (done) {
		var res = tcl.jsFunc('zzz', (...args) => {
console.log("Got:", ...args);
		});

//		expect( match ).to.be.an.instanceof( Array );
	});

} );

