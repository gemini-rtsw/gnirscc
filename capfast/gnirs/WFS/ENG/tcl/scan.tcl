#! /bin/echo scan.tcl should not be run directly from the command line

# $Id: scan.tcl,v 1.2 2009/05/27 19:34:52 fkraemer Exp $

set help(scan) \
{The scan tool is used for characterizing hardware, for measuring backlash, and for diagnosing problems.  The normal mode of operation is as follows:
	
Forward Pass:
	Move to "Pre-forward".  
	Move to "Start", and read the Hall effect sensors
	Move to "Start" + "Increment", and read the Hall effect sensors
	(Repeat until passes "End")

Reverse Pass:
	Move to "Pre-reverse"
	Move to "End", and read the Hall effect sensors
	Move to "End" - "Increment", and read the Hall effect sensors
	(Repeat until passes "Start")

The difference between the values on the forward and reverse pass provides an indication of the amount of backlash in the system.  Significant differences between the forward and reverse passes indicate a serious hardware problem, i.e., something is slipping or sticking.

For more details, see niri_hty_004.fm.gz (compressed framemaker file), available from ftp://gemftp.ifa.hawaii.edu/pub/gemini_IFA/niri.  It is also contained within the compressed postscript file niri_hty.ps.gz.}

proc scan:loop { w mech } {
	global parms const ca

	set status 0

	# Make sure that the range is legal, or the display will crash.

	if { $parms($mech,start) >= $parms($mech,end) } { 
		set status 1
		tk_dialog .dialog {Error} {Start >= End} error 1 Ok
	} 
	
	if { !$status && $parms($mech,preForward) > $parms($mech,start) &&
			($parms($mech,scanDir) & $const(dir,forward)) } { 
		if { [tk_dialog .dialog {Warning} {Pre-Forward > Start} \
				warning 1 {Set Pre-Forward} Cancel] != 0 } {
			set status 1
		} else {
			set parms($mech,preForward) $parms($mech,start)
			update
		}
	} 
	
	if { !$status && $parms($mech,end) > $parms($mech,preReverse) &&
			($parms($mech,scanDir) & $const(dir,reverse)) } { 
		if { [tk_dialog .dialog {Warning} {End > Pre-Reverse} \
				warning 1 {Set Pre-Reverse} Cancel] != 0 } {
			set status 1
		} else {
			set parms($mech,preReverse) $parms($mech,end)
			update
		}
	}
	
	if { !$status && $parms($mech,dataFile) != {} \
			&& [file exists $parms($mech,dataFile)] } {
		if { [tk_dialog .dialog Warning \
				"$parms($mech,dataFile) already exists" \
				warning 0 Cancel Overwrite] != 1 } {
			set status 1
		}
	} 
	
	if { !$status && $parms($mech,dataFile) == {} } {
		if {[tk_dialog .dialog Warning \
				{Data will not be saved to disk} warning 0 Continue Cancel]
				!= 0} {
			set status 1
		}
	}

	#
	# Set the power so that it never turns off.  This is necessary for
	# very fine motion, because the motor will fall into a detente
	# when the power is removed.
	#

	if { !$status } {
		set status [ca:readVars "${mech}Hallstep.POFF"]
		set oldPoff $ca(${mech}Hallstep.POFF)
	}

	if { !$status } {
		set ca(${mech}Hallstep.POFF) No 
		set status [ca:writeVars ${mech}Hallstep.POFF]
	}

	#
	# Set the mechanism raw mode, so that a movement does exactly what 
	# we want it to do.  Otherwise, backlash correction will be done
	# and circular mechanisms will move in an optimized fashion.
	#

	if { !$status } {
		set status [ca:readVars "${mech}Hallstep.RAW"]
		set oldRaw $ca(${mech}Hallstep.RAW)
	}

	if { !$status } {
		set ca(${mech}Hallstep.RAW) Raw 
		set status [ca:writeVars ${mech}Hallstep.RAW]
	}

	#
	# Disable interface elements that can disrupt the main loop, then
	# execute the main loop.
	#

	if { !$status } {
		set parms($mech,loop) 1
		$w.stop configure -state normal
		$w.preForward configure -state disabled
		$w.preReverse configure -state disabled
		$w.inc configure -state disabled
		$w.avg configure -state disabled
		$w.run configure -state disabled
		$w.close configure -state disabled
		$w.dataFile configure -state disabled
		$w.start configure -state disabled
		$w.scanDir.forward configure -state disabled
		$w.scanDir.reverse configure -state disabled
		$w.scanDir.both configure -state disabled
		$w.end configure -state disabled
		$w.backlash configure -state disabled
		update

		if {$parms($mech,dataFile) != {}} {
			set fd [open $parms($mech,dataFile) w]

			puts $fd "# date [exec date]"
			puts $fd "# mech $mech"
			puts $fd "# parms($mech,backlash) $parms($mech,backlash)"
			puts $fd "# parms($mech,end) $parms($mech,end)"
			puts $fd "# parms($mech,inc) $parms($mech,inc)"
			puts $fd "# parms($mech,max) $parms($mech,max)"
			puts $fd "# parms($mech,min) $parms($mech,min)"
			puts $fd "# parms($mech,avg) $parms($mech,avg)"
			puts $fd "# parms($mech,dataFile) $parms($mech,dataFile)"
			puts $fd "# parms($mech,preForward) $parms($mech,preForward)"
			puts $fd "# parms($mech,preReverse) $parms($mech,preReverse)"
			puts $fd "# parms($mech,start) $parms($mech,start)"
			puts $fd "# parms($mech,scanDir) $parms($mech,scanDir)"
		} else {
			set fd {}
		}

		foreach g "$w.g1p $w.g1b $w.g2p $w.g2b" {
			$g element configure f -data {}
			$g element configure r -data {}
			$g xaxis configure -min $parms($mech,start) -max $parms($mech,end)
			$g element show {f r}
		}

		if { ($parms($mech,scanDir) & $const(dir,forward)) \
				&& $parms($mech,loop) } {
			if { !$status } {
				set parms($mech,state) {Pre-Forward}
				set status [scan:moveTo $mech $parms($mech,preForward)]
			}
			if { !$status } {
				set parms($mech,state) {Forward Pass}
				set status [scan:readLoop $w $mech \
					$parms($mech,start) $parms($mech,end) $parms($mech,inc) \
					f $fd]
			}
		}

		if { ($parms($mech,scanDir) & $const(dir,reverse)) \
				&& $parms($mech,loop)} {
			if { !$status } {
				set parms($mech,state) {Pre-Forward}
				set status [scan:moveTo $mech $parms($mech,preReverse)]
			}
			if { !$status } {
				set parms($mech,state) {Reverse Pass}
				set status [scan:readLoop $w $mech \
					$parms($mech,end) $parms($mech,start) $parms($mech,inc) \
					r $fd]
			}
		}

		if { $fd != {} } {
			close $fd
		}

		set parms($mech,moveTo) {}
		set parms($mech,loop) 0
		set parms($mech,state) Inactive 

		$w.stop configure -state disabled
		$w.preForward configure -state normal
		$w.preReverse configure -state normal
		$w.inc configure -state normal
		$w.avg configure -state normal
		$w.run configure -state normal
		$w.close configure -state normal
		$w.dataFile configure -state normal
		$w.start configure -state normal
		$w.end configure -state normal
		$w.backlash configure -state normal
		$w.scanDir.forward configure -state normal
		$w.scanDir.reverse configure -state normal
		$w.scanDir.both configure -state normal
	}

	if { !$status } {
		set ca(${mech}Hallstep.POFF) $oldPoff 
		set status [ca:writeVars ${mech}Hallstep.POFF]
	}

	if { !$status } {
		set ca(${mech}Hallstep.RAW) $oldRaw 
		set status [ca:writeVars ${mech}Hallstep.RAW]
	}

	return $status
}

proc scan:readLoop {w mech start end incr dir fd} {
	global parms

	set status 0

	if {$start > $end} {
		set sign -1
		set incr [expr -abs($incr)]
	} else {
		set sign 1
		set incr [expr abs($incr)]
	}

	if {!$status} {
		set status [scan:moveTo $mech $start]
	}

	set data(hs1p) {}
	set data(hs1b) {}
	set data(hs2p) {}
	set data(hs2b) {}

	for {set i $start} {$i * $sign <= $end * $sign && !$status} {incr i $incr} {
		update

		if {!$parms($mech,loop)} {
			break
		}

		if {!$status && $parms($mech,backlash) != 0} {
			set status [scan:moveTo $mech [expr $i - $parms($mech,backlash)]]
		}
		if {!$status} {
			set status [scan:moveTo $mech $i]
		}

		set sum(hs1p) 0.0
		set sum(hs1b) 0.0
		set sum(hs2p) 0.0
		set sum(hs2b) 0.0

		for {set j 0} {!$status && $j < $parms($mech,avg)} {incr j} {
			global ca

			after 50

			if {!$status} {
				set status [ca:readVars \
					"${mech}Hs1P.VAL ${mech}Hs1B.VAL \
					${mech}Hs2P.VAL ${mech}Hs2B.VAL"]
			}

			if {!$status} {
				set sum(hs1p) [expr $ca(${mech}Hs1P.VAL) + $sum(hs1p)]
				set sum(hs1b) [expr $ca(${mech}Hs1B.VAL) + $sum(hs1b)]
				set sum(hs2p) [expr $ca(${mech}Hs2P.VAL) + $sum(hs2p)]
				set sum(hs2b) [expr $ca(${mech}Hs2B.VAL) + $sum(hs2b)]
			}
		}

		set avg(hs1p) [expr $sum(hs1p) / $parms($mech,avg)]
		set avg(hs1b) [expr $sum(hs1b) / $parms($mech,avg)]
		set avg(hs2p) [expr $sum(hs2p) / $parms($mech,avg)]
		set avg(hs2b) [expr $sum(hs2b) / $parms($mech,avg)]

		lappend data(x) $i
		lappend data(hs1p) $avg(hs1p)
		lappend data(hs1b) $avg(hs1b)
		lappend data(hs2p) $avg(hs2p)
		lappend data(hs2b) $avg(hs2b)

		if {$fd != {}} {
			puts $fd [format {%7d %8.5f %8.5f %8.5f %8.5f %s} \
				$i $avg(hs1p) $avg(hs1b) $avg(hs2p) $avg(hs2b) $dir]
		}
		puts stdout [format {%7d %8.5f %8.5f %8.5f %8.5f %s} \
			$i $avg(hs1p) $avg(hs1b) $avg(hs2p) $avg(hs2b) $dir]

		$w.g1p element append $dir "$i $avg(hs1p)"
		$w.g1b element append $dir "$i $avg(hs1b)"
		$w.g2p element append $dir "$i $avg(hs2p)"
		$w.g2b element append $dir "$i $avg(hs2b)"

		# Kludge to get around blt redraw bug 

		$w.g1p xaxis configure -min [lindex [$w.g1p xaxis configure -min] 4]
		$w.g1b xaxis configure -min [lindex [$w.g1b xaxis configure -min] 4]
		$w.g2p xaxis configure -min [lindex [$w.g2p xaxis configure -min] 4]
		$w.g2b xaxis configure -min [lindex [$w.g2b xaxis configure -min] 4]
	}

	return $status
}

proc scan:moveTo {mech dest} {
	global parms ca

	set status 0

	#
	# For some reason, if two move commands are issued too closely
	# together, the second command times out.
	#

	after 100

	#
	# Update the display
	#

	set parms($mech,moveTo) $dest

	#
	# Move to the new location
	#

	if { !$status } {
		set ca(${mech}MoveTo.VAL) $dest
		set status [ca:writeVars ${mech}MoveTo.VAL]
	}

	#
	# Give the hallstep record time to toggle Busy
	#

	after 100

	while { !$status } {
		after 100

		if { !$status } {
			set status [ca:readVars ${mech}Busy.VAL]
		}

		if { !$status && $ca(${mech}Busy.VAL) == 0 } {
			break
		}
	}

	#
	# Check to see if we're where we should be
	#

	after 100

	if { !$status } {
		set status [ca:readVars ${mech}Motor.RBV]
	}

	if { !$status } {
		if { $ca(${mech}Motor.RBV) != $dest } {
			set status 1
			tk_dialog .dialog {Error} "Did not reach destination" error 1 Ok
		}
	}

	return $status
}

proc scan:connect { w mech } {
	global parms ca const

	set parms($mech,avg) 4 
	set parms($mech,preForward) -100
	set parms($mech,start) -10
	set parms($mech,end) 10
	set parms($mech,preReverse) 100
	set parms($mech,inc) 5
	set parms($mech,backlash) 0
	set parms($mech,state) Inactive
	set parms($mech,dataFile) {} 
	set parms($mech,scanDir) [expr $const(dir,forward) | $const(dir,reverse)]

	set parms($w,mech) $mech

	set status [ca:connect "
			${mech}Hs1P.VAL
			${mech}Hs1B.VAL
			${mech}Hs2P.VAL
			${mech}Hs2B.VAL
			${mech}Motor.RBV
			${mech}MoveTo.VAL
			${mech}Busy.VAL
			${mech}Hallstep.LOPR
			${mech}Hallstep.HOPR
			${mech}Hallstep.POFF
			${mech}Hallstep.RAW
			${mech}Label.VAL
		"]

	if { !$status } {
		set parms($mech,max) $ca(${mech}Hallstep.HOPR)
		set parms($mech,min) $ca(${mech}Hallstep.LOPR)
	}

	if { !$status } {
		if { $w == {} } {
			wm title . "$ca(${mech}Label.VAL) -- Scan Tool"
		} else {
			wm title $w "$ca(${mech}Label.VAL) -- Scan Tool"
		}

		$w.avg configure -textvar parms($mech,avg)
		$w.backlash configure -textvar parms($mech,backlash)
		$w.dataFile configure -textvar parms($mech,dataFile)
		$w.end configure -textvar parms($mech,end)
		$w.inc configure -textvar parms($mech,inc)
		$w.max configure -textvar parms($mech,max)
		$w.mech configure -text $mech
		$w.min configure -textvar parms($mech,min)
		$w.preForward configure -textvar parms($mech,preForward)
		$w.preReverse configure -textvar parms($mech,preReverse)
		$w.dest configure -textvar parms($mech,moveTo)
		$w.scanDir.both configure -var parms($mech,scanDir)
		$w.scanDir.forward configure -var parms($mech,scanDir) 
		$w.scanDir.reverse configure -var parms($mech,scanDir) 
		$w.start configure -textvar parms($mech,start)
		$w.state configure -textvar parms($mech,state)

		$w.run configure -command [list scan:loop "$w" "$mech"]
		$w.stop configure -command "global parms; set parms($mech,loop) 0"
	}

	return $status
}

proc scan:win { w } {
	global const

	set status 0

	util:win $w scan

	frame $w.controls -relief groove -border 0.5m 
	blt_table $w.main $w.controls 0,0 -padx 1.0m -pady 1.0m \
		-rowspan 2 -fill both

	util:table $w.params 15 "
		{label $w.mech {Label} {The EPICS prefix}}
		{label $w.state {Running} {Is a scan in progress?}}
		{label $w.dest {Motor Destination} {The next position}}
		{label $w.min {Min} {Minimum valid position}}
		{label $w.max {Max} {Maximum valid position}}
		{entry $w.preForward {Pre-Forward}
			{Move here before starting a forward scan}}
		{entry $w.start {Start}
			{Initial point for forward scan / End point for reverse scan}}
		{entry $w.end {End}
			{End point for forward scan / Initial point for reverse scan}}
		{entry $w.preReverse {Pre-Reverse} {Move here before reverse scan}}
		{entry $w.inc {Step Size} {Scan step size}}
		{entry $w.backlash {Backlash} {Backlash compensation}}
		{entry $w.avg {Average} {How many points to sample}}
		{entry $w.dataFile {Data File} {Name of file for saving scan data}}
		{frame $w.scanDir {Scan Direction}
			{Forward scan, reverse scan, or both}}
	"

	pack $w.params -in $w.controls -padx 1.0m -pady 1.0m -side top

	blt_graph $w.g1p -width 300 -height 250 -border 0.5m -relief groove \
		-title {Hall Effect Sensor 1 (Primary)}
	blt_table $w.main $w.g1p 0,1 -padx 1.0m -pady 1.0m
	bind $w.g1p <Enter> "global help; set help($w) \
		{Hall Effect Sensor 1 value (Primary)}" 
	bind $w.g1p <Leave> "global help; set help($w) {}" 

	blt_graph $w.g1b -width 300 -height 250 -border 0.5m -relief groove \
		-title {Hall Effect Sensor 1 (Backup)}
	blt_table $w.main $w.g1b 1,1 -padx 1.0m -pady 1.0m
	bind $w.g1b <Enter> \
		"global help; set help($w) {Hall Effect Sensor 1 value (Backup)}" 
	bind $w.g1b <Leave> "global help; set help($w) {}" 

	blt_graph $w.g2p -width 300 -height 250 -border 0.5m -relief groove \
		-title {Hall Effect Sensor 2 (Primary)}
	blt_table $w.main $w.g2p 0,2 -padx 1.0m -pady 1.0m
	bind $w.g2p <Enter> \
		"global help; set help($w) {Hall Effect Sensor 2 value (Primary)}" 
	bind $w.g2p <Leave> "global help; set help($w) {}" 

	blt_graph $w.g2b -width 300 -height 250 -border 0.5m -relief groove \
		-title {Hall Effect Sensor 2 (Backup)}
	blt_table $w.main $w.g2b 1,2 -padx 1.0m -pady 1.0m
	bind $w.g2b <Enter> \
		"global help; set help($w) {Hall Effect Sensor 2 value (Backup)}" 
	bind $w.g2b <Leave> "global help; set help($w) {}" 

	foreach g "$w.g1p $w.g1b $w.g2p $w.g2b" {
		$g element create f -fg green -bg green -symbol plus -linewidth 0
		$g element create r -fg blue -bg blue -symbol cross -linewidth 0
		$g xaxis configure -title {Position (Ministeps)}
		$g yaxis configure -title {Sensor Voltage (V)}
		SetZoom $g
		SetActiveLegend $g
	}

	radiobutton $w.scanDir.forward -text {Forward} \
		-anchor w -val $const(dir,forward) 
	radiobutton $w.scanDir.reverse -text {Reverse} \
		-anchor w -val $const(dir,reverse)
	radiobutton $w.scanDir.both -text {Both} \
		-anchor w -val [expr $const(dir,forward) | $const(dir,reverse)]
	pack $w.scanDir.forward $w.scanDir.reverse $w.scanDir.both -fill x

	button $w.run -text "Run"
	pack $w.run -in $w.controls -side bottom -fill x -padx 0.5m -pady 0.5m

	button $w.stop -text {Stop} -state disabled
	pack $w.stop -in $w.controls -side bottom -fill x -padx 0.5m -pady 0.5m

	return $status
}

proc scan:subRun { mech } {
	global parms

	set status 0

	set w .w$parms(winNo)
	incr parms(winNo)
	toplevel $w

	if { !$status } {
		set status [scan:win $w] 
	}

	if { !$status } {
		set status [scan:connect $w $mech]
	}

	return $status
}

#
# The entry point
#

proc scan:run { w t c m } {
	set status 0

	util:init $t$c$m

	if { !$status } {
		set status [scan:win $w] 
	}

	if { !$status } {
		set status [scan:connect $w $t$c$m]
	}

	return $status
}
