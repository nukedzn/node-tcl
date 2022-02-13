node-tcl
========

[![Join the chat at https://gitter.im/nukedzn/node-tcl](https://nukedzn.github.io/badges/gitter.svg)](https://gitter.im/nukedzn/node-tcl?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![npm version](https://img.shields.io/npm/v/tcl.svg)](https://www.npmjs.com/package/tcl)
[![Build Status](https://travis-ci.org/nukedzn/node-tcl.svg)](https://travis-ci.org/nukedzn/node-tcl)
[![codecov](https://codecov.io/gh/nukedzn/node-tcl/branch/master/graph/badge.svg?token=AAhVnMCHcn)](https://codecov.io/gh/nukedzn/node-tcl)

Provides Node.js bindings to execute Tcl commands using a native Tcl Interpreter.
Also includes the ability to define new commands in the Tcl intepretor that will invoke a
specified JavaScript function.


## Dependencies

- Tcl development files (e.g. ```sudo apt-get install tcl-dev``` or ```sudo yum install tcl-devel```)

The build process will look for ```tclConfig.sh``` to identify Tcl include directory
and linker flags. If you are using a Tcl version older than 8.5 or want to link to a specific
Tcl installation use the ```TCLCONFIG``` environment variable to override the default behaviour
(e.g. ```export TCLCONFIG=/path/to/tclConfig.sh```).

This version has been updated to use [node-addon-api](https://github.com/nodejs/node-addon-api)
and so should be compatible with future versions of node.js.  Unfortunately, it may not build
properly on very old node.js versions.

### Optional Dependencies

- Tcl thread support (To enable async commands outside the main event loop)
- C++ compiler with support for c++11 features (To enable async command queues outside the main event loop)


## Installation

```sh
$ npm install --save tcl
```


## Usage

```js
var tcl = require( 'tcl' );
```

```js
var Tcl = require( 'tcl' ).Tcl;
var tcl = new Tcl();
```

### $( cmd ), cmdSync( cmd ), evalSync( cmd )

Execute a Tcl command synchronously and returns a [Result](http://nukedzn.github.io/node-tcl/docs/Result.html).

#### Parameters

| Name | Type   | Description        |
|------|--------|--------------------|
| cmd  | String | Command to execute |


#### Example

```js
tcl.$( 'info version' );
```


### $.(tcl-command)( ... )

Execute a Tcl command using injected helper methods and returns a [Result](http://nukedzn.github.io/node-tcl/docs/Result.html).

#### Example

```js
tcl.$.info( 'tclversion' );
tcl.$.set( 'x', 10 );
tcl.$.expr( 22, '/', '7.0' );
```


### load( module )

Load a Tcl module and refresh injected helper methods.

#### Example

```js
tcl.load( 'libfoo.so' );
tcl.$.foo();
```


### source( file )

Source a Tcl script file and refresh injected helper methods.

#### Example

```js
tcl.source( '/path/to/multiply.tcl' );
tcl.$.multiply( 2, 3 );
```


### cmd( cmd, callback ), eval( cmd, callback )

Execute a Tcl command asynchronously using a new worker thread (A new Tcl interpreter
instance will be created for each call).

#### Parameters

| Name     | Type   | Description        |
|----------|--------|--------------------|
| cmd      | String | Command to execute |
| callback | [Callback](http://nukedzn.github.io/node-tcl/docs/Tcl.html#~cmdCallback) | Callback method to handle the response |

#### Example

```js
tcl.cmd( 'info tclversion', function ( err, result ) {
	if ( err ) {
		return console.log( err );
	}

	console.log( result.data() );
} );

tcl.eval( 'info commands', function ( err, result ) {
	if ( err ) {
		return console.log( err );
	}

	console.log( result.toArray() );
} );
```


### queue( cmd, callback )

Execute a Tcl command asynchronously using a shared worker thread. A new Tcl interpreter
instance will be created for the worker thread but will be shared between calls.

#### Parameters

| Name     | Type   | Description        |
|----------|--------|--------------------|
| cmd      | String | Command to execute |
| callback | [Callback](http://nukedzn.github.io/node-tcl/docs/Tcl.html#~cmdCallback) | Callback method to handle the response |

#### Example

``` js
tcl.queue( 'set x 1' );
tcl.queue( 'incr x', function ( err, result ) {
	if ( err ) {
		return console.log( err );
	}

	console.log( result.data() ); // 2
} );
```


### proc( cmd, callback )

Instructs the Tcl interpreter to create a new command: cmd.
When this new command is encountered at any time during the life of the interpreter,
the specified callback will be invoked with whatever parameters were supplied to cmd.
There is currently not a command for deleting commands from the interpreter.

#### Parameters

| Name     | Type   | Description               |
|----------|--------|---------------------------|
| cmd      | String | New Tcl command to create |
| callback | [Callback](http://nukedzn.github.io/node-tcl/docs/Tcl.html#~cmdCallback) | Callback method to handle the command |

#### Example

``` js
tcl.proc( 'newTclCmd', function ( arg1, arg2 ) {
	console.log( 'Got', arg1, arg2 ); // Got some arguments
} );

tcl.cmdSync( 'newTclCmd some arguments' );
```



## API Documentation

JSDoc generated API documentation can be found at [http://nukedzn.github.io/node-tcl/docs/](http://nukedzn.github.io/node-tcl/docs/).


## Contributing

Contributions are welcome through GitHub pull requests ([using fork & pull model](https://help.github.com/articles/using-pull-requests/#fork--pull)).

