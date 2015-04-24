node-tcl
========

[![npm version](https://badge.fury.io/js/tcl.svg)](http://badge.fury.io/js/tcl)
[![Dependency Status](https://david-dm.org/nukedzn/node-tcl.svg)](https://david-dm.org/nukedzn/node-tcl)
[![devDependency Status](https://david-dm.org/nukedzn/node-tcl/dev-status.svg)](https://david-dm.org/nukedzn/node-tcl#info=devDependencies)

Node.js Tcl bindings

## Usage

``` js
var Tcl = require( 'tcl' );
var tcl = new Tcl();

tcl.cmd( 'info tclversion', function ( err, data ) {
	console.log( data );
} );
```

