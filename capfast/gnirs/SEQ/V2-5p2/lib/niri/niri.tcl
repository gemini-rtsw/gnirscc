#+
#  niri.tcl
#
#  Implements a package for controlling the NIRI.
#
#+

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Load required packages.
package require Itcl

# Create the required command senders and status acceptors.
epics cs niri::observe
epics cs niri::endObserve
epics cs niri::config
epics cs niri::dcconfig

epics cs niri::stop
epics cs niri::abort
epics cs niri::pause
epics cs niri::continue


epics sa niri::apply
epics sa niri::status

epics sa niri::dcapply
epics sa niri::dcstatus

set dirname [file dirname [info script]]
lappend auto_path [file join $dirname scripts]

# Create namespace variables and link to EPICS channels.
namespace eval niri {
   variable commandStatus ""
   variable commandMessage ""
   variable observeStatus ""

   variable commandDCStatus ""
   variable commandDCMessage ""
   variable observeDCStatus ""

   variable exposureTime ""

# Proc the updates the namespace variables when ever the EPICS variable
# changes value. The message value is ignored if it is empty so that
# commandMessage always contains the last message output.
   proc statusProc {name alarm time value} {
      switch -exact $name {
         niri::status.applyC {
            variable commandStatus
            set commandStatus $value
         }
         niri::status.message {
            if { ![string is space $value] } {
               variable commandMessage
               set commandMessage $value
            }
         }
         niri::status.observe {
            variable observeStatus
            set observeStatus $value
         }
      }
   }
   proc statusDCProc {name alarm time value} {
      switch -exact $name {
         niri::dcstatus.applyC {
            variable commandDCStatus
            set commandDCStatus $value
         }
         niri::dcstatus.message {
            if { ![string is space $value] } {
               variable commandDCMessage
               set commandDCMessage $value
            }
         }
         niri::dcstatus.observe {
            variable observeDCStatus
            set observeDCStatus $value
         }
         niri::dcstatus.intTime {
            variable exposureTime
            set exposureTime $value
         }
         niri::dcstatus.minInt {
         }
      }
   }

# Link the proc to the status acceptor.
   set proc [namespace code statusProc]
   sa niri::status proc applyC $proc message $proc observe $proc
   set proc [namespace code statusDCProc]
   sa niri::dcstatus proc applyC $proc message $proc observe $proc intTime $proc minInt $proc 
}

package provide Niri 4.0
