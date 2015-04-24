node-tcl
========

[![npm version](https://badge.fury.io/js/tcl.svg)](http://badge.fury.io/js/tcl)

Node.js Tcl bindings

## Usage

``` js
var Tcl = require( 'tcl' );
var tcl = new Tcl();

tcl.cmd( 'info tclversion', function ( err, data ) {
	console.log( data );
} );
```

