#+
#  gmos.tcl
#
#  Implements a package for controlling the GMOS.
#
#  D Terrett 24 July 2001
#
#  Copyright CCLRC
#+

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Load required packages.
package require Itcl

# Create the required command senders and status acceptors.
epics cs gmos::observe
epics cs gmos::endObserve
epics cs gmos::config
epics cs gmos::dcconfig

epics cs gmos::stop
epics cs gmos::abort
epics cs gmos::pause
epics cs gmos::continue

epics sa gmos::apply
epics sa gmos::status

epics sa gmos::dcapply
epics sa gmos::dcstatus

set dirname [file dirname [info script]]
lappend auto_path [file join $dirname scripts]

# Create namespace variables and link to EPICS channels.
namespace eval gmos {
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
         gmos::status.applyC {
            variable commandStatus
            set commandStatus $value
         }
         gmos::status.message {
            if { ![string is space $value] } {
               variable commandMessage
               set commandMessage $value
            }
         }
         gmos::status.observe {
            variable observeStatus
            set observeStatus $value
         }
      }
   }
   proc statusDCProc {name alarm time value} {
      switch -exact $name {
         gmos::dcstatus.applyC {
            variable commandDCStatus
            set commandDCStatus $value
         }
         gmos::dcstatus.message {
            if { ![string is space $value] } {
               variable commandDCMessage
               set commandDCMessage $value
            }
         }
         gmos::dcstatus.observe {
            variable observeDCStatus
            set observeDCStatus $value
         }
         gmos::dcstatus.exposureTime {
            variable exposureTime
            set exposureTime $value
         }
      }
   }

# Link the proc to the status acceptor.
   set proc [namespace code statusProc]
   sa gmos::status proc applyC $proc message $proc observe $proc
   set proc [namespace code statusDCProc]
   sa gmos::dcstatus proc applyC $proc message $proc observe $proc exposureTime $proc 
}

package provide Gmos 2.0
