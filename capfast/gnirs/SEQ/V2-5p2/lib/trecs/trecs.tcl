#+
#  trecs.tcl
#
#  Implements a package for controlling the TRECS.
#
#
#  Matthieu Bec - 19 November 2002
#
#+

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Load required packages.
package require Itcl

# Create the required command senders and status acceptors.
epics cs trecs::observe
epics cs trecs::continue
epics sa trecs::status

set dirname [file dirname [info script]]
lappend auto_path [file join $dirname scripts]

# Create namespace variables and link to EPICS channels.
namespace eval trecs {

   variable commandStatus ""
   variable commandMessage ""
   variable observeStatus ""

# Proc the updates the namespace variables when ever the EPICS variable
# changes value. The message value is ignored if it is empty so that
# commandMessage always contains the last message output.
   proc statusProc {name alarm time value} {
      switch -exact $name {
         trecs::status.applyC {
            variable commandStatus
            set commandStatus $value
         }
         trecs::status.message {
            if { ![string is space $value] } {
               variable commandMessage
               set commandMessage $value
            }
         }
         trecs::status.observe {
            variable observeStatus
            set observeStatus $value
         }
      }
   }

# Link the proc to the status acceptor.
   set proc [namespace code statusProc]
   sa trecs::status proc applyC $proc message $proc observe $proc
}

package provide Trecs 2.0
