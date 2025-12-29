#! /bin/echo tmp.tcl should not be run directly from the command line

# $Id: tmp.tcl,v 1.2 2009/05/27 19:34:52 fkraemer Exp $

proc tmp:loop { w mech } {
	global ca parms

	set ok 1
	if {$parms($mech,file) != {} && [file exists $parms($mech,file)]} {
		set ok [tk_dialog .dialog Warning \
			"$parms($mech,file) already exists" warning 0 Cancel Overwrite]
	}
	if {!$ok} {
		return
	}

	if {$parms($mech,file) != {}} {
		set fd [open $parms($mech,file) w]
	} else {
		set fd {}
	}

	$w.stop configure -state normal
	$w.close configure -state disabled
	$w.run configure -state disabled
	$w.file configure -state disabled

	set start [exec tm seconds]

	for {set parms(loop) 1} {$parms(loop)} {} {
		set deltat [expr [exec tm seconds] - $start]

		#
		# Note: We do not terminate this loop if there is an error, because
		# we seem to get a few random failures.
		#

		set status [ca:readVars "
				${mech}7tmpin.VAL ${mech}7atod0.VAL
				${mech}14tmpin.VAL ${mech}14atod0.VAL
				${mech}15tmpin.VAL ${mech}15atod0.VAL
			" {}]

		if { !$status } {
			$w.t element append t7 "$deltat $ca(${mech}7tmpin.VAL)"
			$w.t element append t14 "$deltat $ca(${mech}14tmpin.VAL)"
			$w.t element append t15 "$deltat $ca(${mech}15tmpin.VAL)"
			$w.v element append v7 "$deltat $ca(${mech}7atod0.VAL)"
			$w.v element append v14 "$deltat $ca(${mech}14atod0.VAL)"
			$w.v element append v15 "$deltat $ca(${mech}15atod0.VAL)"
			puts stdout [format "%d %.3f %.3f %.3f %.0f %.0f %.0f" \
				$deltat $ca(${mech}7atod0.VAL) $ca(${mech}14atod0.VAL) \
				$ca(${mech}15atod0.VAL) $ca(${mech}7tmpin.VAL) \
				$ca(${mech}14tmpin.VAL) $ca(${mech}15tmpin.VAL)]
			if { $fd != {} } {
				puts $fd [format "%d %.3f %.3f %.3f %.0f %.0f %.0f" \
					$deltat \
					$ca(${mech}7atod0.VAL) $ca(${mech}14atod0.VAL) \
					$ca(${mech}14tmpin.VAL) $ca(${mech}7tmpin.VAL) \
					$ca(${mech}14tmpin.VAL) $ca(${mech}15tmpin.VAL)]
				flush $fd
			}
		}

		update

		after 5000
	}

	if { "$fd" != "" } {
		close $fd
	}

	$w.stop configure -state disabled
	$w.close configure -state normal
	$w.run configure -state normal
	$w.file configure -state normal
}

proc tmp:connect { mech } {
	set status [ca:connect "
		${mech}7tmpin.VAL ${mech}7atod0.VAL
		${mech}14tmpin.VAL ${mech}14atod0.VAL
		${mech}15tmpin.VAL ${mech}15atod0.VAL
	"]

	set parms($mech,file) {}

	return $status
}

proc tmp:win { w mech } {
	set status 0

	frame $w.main -relief groove -border 1.0m 
	pack $w.main -padx 1.0m -pady 1.0m -ipadx 1.0m -ipady 1.0m

	frame $w.data -border 0.5m -relief ridge
	pack $w.data -in $w.main -padx 0.5m -pady 0.5m

	frame $w.data.cmd
	pack $w.data.cmd -side left

	label $w.file_l -text {Output File}
	blt_table $w.data.cmd $w.file_l 0,0 -fill x
	entry $w.file -textvar parms($mech,file) -relief sunken -border 0.5m
	blt_table $w.data.cmd $w.file 0,1 -fill x

	button $w.run -command "tmp:loop {$w} {$mech}" -text Run
	blt_table $w.data.cmd $w.run 1,1 -fill x

	button $w.stop -text "Stop" -command "global parms; set parms(loop) 0" \
		-state disabled
	blt_table $w.data.cmd $w.stop 2,1 -fill x

	blt_graph $w.t -border 1.0m -relief sunken
	pack $w.t -in $w.data -padx 0.5m -pady 0.5m -side left
	$w.t element create t7 -symbol plus -fg red -linewidth 0
	$w.t element create t14 -symbol diamond -fg blue -linewidth 0
	$w.t element create t15 -symbol cross -fg green -linewidth 0
	SetZoom $w.t
	SetActiveLegend $w.t

	blt_graph $w.v -border 1.0m -relief sunken
	pack $w.v -in $w.data -padx 0.5m -pady 0.5m -side left
	$w.v element create v7 -symbol plus -fg red -linewidth 0
	$w.v element create v14 -symbol diamond -fg blue -linewidth 0
	$w.v element create v15 -symbol cross -fg green -linewidth 0
	SetZoom $w.v
	SetActiveLegend $w.v

	if { "$w" == "" } {
		button $w.close -command "destroy ." -text Close
	} else {
		button $w.close -command "destroy $w" -text Close
	}
	pack $w.close -in $w.main

	return $status
}

proc tmp:run { w t c m } {
	set status 0

	if { !$status } {
		set status [tmp:connect $t$c${m}tmp]
	}

	if { !$status } {
		set status [tmp:win $w $t$c${m}tmp]
	}

	return $status 
}
