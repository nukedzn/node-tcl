node-tcl
========

[![npm version](https://badge.fury.io/js/tcl.svg)](http://badge.fury.io/js/tcl)
[![Build Status](https://travis-ci.org/nukedzn/node-tcl.svg)](https://travis-ci.org/nukedzn/node-tcl)
[![Coverage Status](https://coveralls.io/repos/nukedzn/node-tcl/badge.svg)](https://coveralls.io/r/nukedzn/node-tcl)
[![Dependency Status](https://david-dm.org/nukedzn/node-tcl.svg)](https://david-dm.org/nukedzn/node-tcl)
[![devDependency Status](https://david-dm.org/nukedzn/node-tcl/dev-status.svg)](https://david-dm.org/nukedzn/node-tcl#info=devDependencies)

Node.js Tcl bindings

## Usage

``` js
var tcl = require( 'tcl' );

tcl.cmd( 'info tclversion', function ( err, data ) {
	console.log( data );
} );
```

