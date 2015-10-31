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



## API Documentation

JSDoc generated API documentation can be found at [http://nukedzn.github.io/node-tcl/docs/](http://nukedzn.github.io/node-tcl/docs/).


## Contributing

Contributions are welcome through GitHub pull requests ([using fork & pull model](https://help.github.com/articles/using-pull-requests/#fork--pull)).

