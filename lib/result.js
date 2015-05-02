
'use strict';

/**
*   Represents a Tcl command result
*
*   @constructor
*   @param {string} data - raw response data
*   @param {object} binding - Tcl interpreter binding
*/
var Result = function ( data, binding ) {

	// update internals
	this._result = {
		raw : data
	};

	this._binding = binding;

};


/**
*   Returns raw resonse data
*
*   @return {string} Raw response data
*/
Result.prototype.data = function () {
	return this._result.raw;
};


/**
*   Converts a string that represents a Tcl list into a JavaScript array using
*   native Tcl binding. If a string is not passed in as a parameter, the
*   internal response data representation will be used.
*
*   @param {(string|null)} str - Tcl list as a string
*   @return {Array} When the conversion is successful
*   @return {null} When the conversion fails
*/
Result.prototype.toArray = function ( str ) {

	// sanity checks
	if (! this._binding ) {
		return null;
	}

	// if a string is passed in use that instead of internal data
	if ( str && str.length > 0 ) {
		return this._binding.toArray( str );
	}

	if ( this._result.raw &&
		this._result.raw.length > 0 ) {
		return this._binding.toArray( this._result.raw );
	}

	// no valid conversion
	return null;

};


module.exports = Result;

