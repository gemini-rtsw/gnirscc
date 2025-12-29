#+
#  michelle.tcl
#
#  Implements a package for controlling the MICHELLE.
#
#+

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Load required packages.
package require Itcl

# Create the required command senders and status acceptors.
epics cs michelle::observe
epics cs michelle::endObserve
epics cs michelle::config
epics cs michelle::dcconfig

epics cs michelle::stop
epics cs michelle::abort
epics cs michelle::pause
epics cs michelle::continue


epics sa michelle::apply
epics sa michelle::status

epics sa michelle::dcapply
epics sa michelle::dcstatus

set dirname [file dirname [info script]]
lappend auto_path [file join $dirname scripts]

# Create namespace variables and link to EPICS channels.
namespace eval michelle {
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
         michelle::status.applyC {
            variable commandStatus
            set commandStatus $value
         }
         michelle::status.message {
            if { ![string is space $value] } {
               variable commandMessage
               set commandMessage $value
            }
         }
         michelle::status.observe {
            variable observeStatus
            set observeStatus $value
         }
      }
   }
   proc statusDCProc {name alarm time value} {
      switch -exact $name {
         michelle::dcstatus.applyC {
            variable commandDCStatus
            set commandDCStatus $value
         }
         michelle::dcstatus.message {
            if { ![string is space $value] } {
               variable commandDCMessage
               set commandDCMessage $value
            }
         }
         michelle::dcstatus.observe {
            variable observeDCStatus
            set observeDCStatus $value
         }
         michelle::dcstatus.intTime {
            variable exposureTime
            set exposureTime $value
         }
         michelle::dcstatus.minInt {
         }
      }
   }

# Link the proc to the status acceptor.
   set proc [namespace code statusProc]
   sa michelle::status proc applyC $proc message $proc observe $proc
   set proc [namespace code statusDCProc]
   sa michelle::dcstatus proc applyC $proc message $proc observe $proc intTime $proc minInt $proc 
}

package provide Michelle 4.0
