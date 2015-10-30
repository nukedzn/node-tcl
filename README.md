node-tcl
========

[![npm version](https://img.shields.io/npm/v/tcl.svg)](https://www.npmjs.com/package/tcl)
[![Build Status](https://travis-ci.org/nukedzn/node-tcl.svg)](https://travis-ci.org/nukedzn/node-tcl)
[![Coverage Status](https://coveralls.io/repos/nukedzn/node-tcl/badge.svg)](https://coveralls.io/r/nukedzn/node-tcl)
[![Dependency Status](https://david-dm.org/nukedzn/node-tcl.svg)](https://david-dm.org/nukedzn/node-tcl)
[![devDependency Status](https://david-dm.org/nukedzn/node-tcl/dev-status.svg)](https://david-dm.org/nukedzn/node-tcl#info=devDependencies)

Node.js Tcl bindings to execute Tcl commands using a native Tcl Interpreter.


## Dependencies

- Tcl development files (e.g. ```sudo apt-get install tcl-dev``` or ```sudo yum install tcl-devel```)

The build process will look for ```tclConfig.sh``` to identify Tcl include directory
and linker flags. If you are using a Tcl version older than 8.5 or want to link to a specific
Tcl installation use the ```TCLCONFIG``` environment variable to override the default behaviour
(e.g. ```export TCLCONFIG=/path/to/tclConfig.sh```).

### Optional Dependencies

- Tcl thread support (To enable async commands outside the main event loop)
- C++ compiler with support for c++11 features (To enable async command queues outside the main event loop)


## Installation

```sh
$ npm install --save tcl
```


## Usage

You can execute any Tcl command supported by the Tcl shell (```tchsh```) and you
can even load native Tcl modeles (```load module.so```), source scripts
(```source filename.tcl```) and source Tcl libraries (```package require name```).

**Note :** Asynchronous commands (```cmd``` and ```eval```) are executed usng a
dedicated worker thread using a new Tcl Interpreter instance and comes with the
overhead of creating and destroying a Tcl Interpreter for each call. But these
executions are parallel and useful for batched tasks.


``` js
var tcl = require( 'tcl' );

// synchronous commands
console.log( tcl.version() );
console.log( tcl.$( 'info tclversion' ) );  // version >= 2.3.0
console.log( tcl.$.info( 'tclversion' ) );  // version >= 2.3.0
console.log( tcl.cmdSync( 'info tclversion' ) );
console.log( tcl.evalSync( 'info tclversion' ) );


// asynchronous commands (parallelly executed in decated threads)
tcl.cmd( 'info tclversion', function ( err, result ) {
	console.log( result.data() );
} );

tcl.eval( 'info commands', function ( err, result ) {
	console.log( result.toArray() );
} );


// queued asynchronous commands (executed in a shared thread)
tcl.queue( 'set x 0' );
tcl.queue( 'incr $x', function ( err, result ) {
	console.log( result );
} );
```

``` js
var Tcl = require( 'tcl' ).Tcl;
var tcl = new Tcl();

console.log( tcl.version() );
```


## API Documentation

JSDoc generated API documentation can be found at [http://nukedzn.github.io/node-tcl/docs/](http://nukedzn.github.io/node-tcl/docs/).


## Contributing

Contributions are welcome through GitHub pull requests ([using fork & pull model](https://help.github.com/articles/using-pull-requests/#fork--pull)).

