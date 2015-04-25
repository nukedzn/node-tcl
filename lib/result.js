
'use strict';


var Result = function ( data, interp ) {

	// update internals
	this._result = {
		raw : data
	};

	this._interp = interp;

};


Result.prototype.data = function () {
	return this._result.raw;
};


Result.prototype.toArray = function ( str ) {

	// sanity checks
	if (! this._interp ) {
		return null;
	}

	// if a string is passed in use that instead of internal data
	if ( str && str.length > 0 ) {
		return this._interp.toArray( str );
	}

	if ( this._result.raw &&
		this._result.raw.length > 0 ) {
		return this._interp.toArray( this._result.raw );
	}

	// no valid conversion
	return null;

};


module.exports = Result;

