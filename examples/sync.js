
'use strict';

var tcl = require( '../' );

// Each tcl command execution returns a Result object and you can use .data()
// method to access raw data without any data conversions
var version  = tcl.$( 'info tclversion' ).data();
var cmdcount = tcl.$.info( 'cmdcount' ).data();

console.log( 'version:', version );
console.log( 'cmdcount:', cmdcount );

// .toArray() method attempts to convert a tcl result into a javascript array
var list = tcl.$.set( 'list', '[list a b {c d} {e f {g h}}]' );
console.log( list.toArray() );

// Using tcl 'puts' instead of console.log()
var time = tcl.$.clock( 'seconds' ).data();
var date = new Date( time * 1000 );
tcl.$.puts( '{' + date.toISOString() + '}' );

