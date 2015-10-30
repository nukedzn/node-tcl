#!/usr/bin/tclsh

namespace eval node::tcl {
	proc add {x y} {
		return [expr {$x + $y}]
	}
}

proc multiply {x y} {
	return [expr {$x * $y}]
}

