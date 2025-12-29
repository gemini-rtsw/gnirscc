#! /bin/echo help.tcl should not be run directly from the command line

# $Id: help.tcl,v 1.2 2009/05/27 19:34:51 fkraemer Exp $

proc help:win { w text } {
	if [winfo exists $w ] {
		destroy $w
	}

	toplevel $w

	frame $w.main -relief sunken -border 0.5m
	pack $w.main -fill both -expand 1

	scrollbar $w.sb -command "$w.text yview"
	pack $w.sb -in $w.main -side left -fill y

	text $w.text -wrap word -setgrid yes -yscroll "$w.sb set" -width 1 -height 1
	pack $w.text -in $w.main -side right -fill both -expand 1

	button $w.close -command "destroy $w" -text "Close"
	pack $w.close

	$w.text insert end $text
	$w.text configure -state disabled

	wm geometry $w 80x25
}
