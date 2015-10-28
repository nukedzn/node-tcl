#!/usr/bin/tclsh
#

set tclconfig {}

if {[info exists env(TCLCONFIG)]} {
	set tclconfig $env(TCLCONFIG)
} else {
	package require Tcl 8.5

	set libdir [::tcl::pkgconfig get libdir,runtime]
	set tclconfig [file join $libdir tclConfig.sh]
}

puts $tclconfig
exit [expr {$tclconfig == {}}]

