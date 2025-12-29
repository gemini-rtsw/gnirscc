#! /bin/echo util.tcl should not be run directly from the command line

# $Id: util.tcl,v 1.2 2009/05/27 19:34:52 fkraemer Exp $

set mechList {
	{{CC Beam Splitter} eng:ccSplt}
	{{CC Beam Steerer 1} eng:ccSter1}
	{{CC Beam Steerer 2} eng:ccSter2}
	{{CC Cover} eng:ccCov}
	{{CC Filter 1} eng:ccFilt1}
	{{CC Filter 2} eng:ccFilt2}
	{{CC Filter 3} eng:ccFilt3}
	{{CC Focal Plane Mask} eng:ccFopl}
	{{CC Focus} eng:ccFoc}
	{{CC Pupil Viewer} eng:ccPuvw}
	{{WFS Filter Wheel} eng:wfsFilt}
	{{WFS Focus} eng:wfsFoc}
	{{WFS Probe X} eng:wfsPrbx}
	{{WFS Probe Y} eng:wfsPrby}
}

proc util:win { w winType } {
	global parms mechList

	if { $w != {} } {
		wm protocol $w WM_DELETE_WINDOW "util:exit {$w}"
	} else { 
		wm protocol . WM_DELETE_WINDOW "util:exit {}"
	}

	frame $w.menu
	pack $w.menu -side top -padx 0.5m -pady 0.5m -fill x

	menubutton $w.menu.file -text File -menu $w.menu.file.m
	pack $w.menu.file -side left

	menu $w.menu.file.m
	if { $w == {} } {
		$w.menu.file.m add command -label Exit -command "util:exit {$w}"
	} else {
		$w.menu.file.m add command -label Close -command "destroy {$w}"
	}

	menubutton $w.menu.mech -text Mechanism -menu $w.menu.mech.m
	pack $w.menu.mech -side left

	menu $w.menu.mech.m
	foreach m $mechList {
		$w.menu.mech.m add radiobutton -label [lindex $m 0] \
			-var parms($w,mech) -value "$parms(prefix)[lindex $m 1]" \
			-command "${winType}:connect {$w} $parms(prefix)[lindex $m 1]"
	}

	menubutton $w.menu.tools -text Tools -menu $w.menu.tools.m
	pack $w.menu.tools -side left

	menu $w.menu.tools.m
	$w.menu.tools.m add cascade -label scan -menu $w.menu.tools.m.scan
	$w.menu.tools.m add cascade -label cycle -menu $w.menu.tools.m.cycle

	menu $w.menu.tools.m.scan
	$w.menu.tools.m.scan add command -label Current \
		-command "global parms; scan:subRun \$parms($w,mech)"
	$w.menu.tools.m.scan add separator
	foreach m $mechList {
		$w.menu.tools.m.scan add command -label [lindex $m 0] \
			-command "scan:subRun $parms(prefix)[lindex $m 1]"
	}

	menu $w.menu.tools.m.cycle
	$w.menu.tools.m.cycle add command -label Current \
		-command "global parms; cycle:subRun \$parms($w,mech)"
	$w.menu.tools.m.cycle add separator
	foreach m $mechList {
		$w.menu.tools.m.cycle add command -label [lindex $m 0] \
			-command "cycle:subRun $parms(prefix)[lindex $m 1]"
	}

	menubutton $w.menu.help -text Help -menu $w.menu.help.m
	pack $w.menu.help -side right

	menu $w.menu.help.m
    $w.menu.help.m add command -label Help \
		-command "global help; help:win $w.help \$help($winType)"

	tk_menuBar $w.menu $w.menu.file.m $w.menu.tools.m $w.menu.help.m

	frame $w.main -relief sunken -border 0.5m
	pack $w.main -padx 0.5m -pady 0.5m -side top

	frame $w.buttons -relief sunken -border 0.5m
	pack $w.buttons -padx 0.5m -pady 0.5m -side top -fill x

	if { $w == {} } {
		button $w.close -text "Exit" -command "util:exit {$w}"
	} else {
		button $w.close -text "Close" -command "destroy {$w}"
	}
	pack $w.close -in $w.buttons -padx 0.5m -pady 0.5m

	label $w.helpText -textvar helpLine($w) -anchor w
	pack $w.helpText -padx 0.5m -pady 0.5m -fill x -side top
}

proc util:table { f width items } {
	set status 0

	frame $f -border 0.5m -relief raised

	set row 0

	foreach item $items {
		if { [llength $item] != 4 } {
			set status 1
			tk_dialog .dialog {Error} "Expected 4 items: $item" error 1 Ok
			break
		}
		set type [lindex $item 0]
		set e [lindex $item 1]
		set text [lindex $item 2]
		set htext [lindex $item 3]

		label $e:t -text $text -anchor e
		blt_table $f $e:t $row,0 -fill x

		switch -exact $type {
			entry {
				entry $e -width $width -relief sunken -border 0.5m
			}
			label {
				label $e -relief sunken -border 0.5m -bg gray -anchor e
			}
			frame {
				frame $e -relief sunken -border 0.5m
			}
			default {
				$type $e
			}
		}
		blt_table $f $e $row,1 -fill x

		if { [winfo toplevel $f] == "." } {
			set helpVar helpLine()
		} else {
			set helpVar helpLine([winfo toplevel $f])
		}
		bind $e <Enter> "global help; set $helpVar {$htext}"
		bind $e <Leave> "global help; set $helpVar {}" 

		incr row
	}

	return $status
}

proc util:init { mech } {
	global parms env

	set parms(prefix) $env(NIRI_PREFIX)
	set parms(winNo) 0
}

proc util:exit { w } {
	global parms

	if { $w != {} } {
		destroy $w
	} elseif { $parms(winNo) == 0 \
			|| [ tk_dialog .dialog {Warning} \
					"This will close ALL windows of the calibration tool" \
					warning 0 Ok Cancel ] == 0 } {
		destroy .
	}		
}
