#! /bin/echo ca.tcl should not be run directly from the command line

# $Id: ca.tcl,v 1.2 2009/05/27 19:34:51 fkraemer Exp $

proc ca:connect { caChans { dialog .dialog } } {
	global errorCode

	set status 0

	foreach caChan $caChans {
		if { !$status } {
			set state [lindex [lindex [pv info ca($caChan) state] 0] 1]

			if { $state == "__" } {
				if { !$status && [pv linkw ca($caChan) $caChan] != 0 } {
					set status 1
					set msg "$caChan: Cannot connect (err=$errorCode)"
					if { "$dialog" != "" } {
						tk_dialog $dialog {Error} $msg error 1 Ok
					} else {
						puts stderr $msg
					}
				}
			} else {
				puts stdout "$caChan: Already connected"
			}
		}

		if { !$status } {
			set status [ca:checkVars $caChan]
		}
	}

	return $status
}

proc ca:checkVars { caChans { dialog .dialog } } {
	global stat

	set status 0

	foreach caChan $caChans {
		pv stat ca($caChan) stat
		if { $stat(state) != "OK" || $stat(severity) == "INVALID" } {
			set status 1
			set name [lindex [lindex [pv info ca($caChan) name] 0] 1]
			set msg "$name: I/O error (state=$stat(state),severity=$stat(severity))"
			if { "$dialog" != "" } {
				tk_dialog $dialog {Error} $msg error 1 Ok
			} else {
				puts stderr $msg
			}
		}
	}

	return $status
}

proc ca:writeVars { caChans { dialog .dialog } } {
	global errorCode

	set status 0

	foreach caChan $caChans {
		if { !$status } {
			if { [pv putw ca($caChan) 5] != 0 } {
				set status 1
				set name [lindex [lindex [pv info ca($caChan) name] 0] 1]
				set msg "$name: Write failed (err=$errorCode)"
				if { "$dialog" != "" } {
					tk_dialog $dialog {Error} $msg error 1 Ok
				} else { 
					puts stderr $msg
				}
			} else {
				set status [ca:checkVars $caChan "$dialog"]
			}
		}
	}

	return $status
}

proc ca:readVars { caChans { dialog .dialog } } {
	global errorCode

	set status 0

	foreach caChan $caChans {
		if { !$status } {
			if { [pv getw ca($caChan) 5] != 0 } {
				set status 1
				set name [lindex [lindex [pv info ca($caChan) name] 0] 1]
				set msg "$name: Read failed (err=$errorCode)"
				if { "$dialog" != "" } {
					tk_dialog $dialog {Error} $msg error 1 Ok
				} else { 
					puts stderr $msg
				}
			}
		}
	}

	return $status
}
