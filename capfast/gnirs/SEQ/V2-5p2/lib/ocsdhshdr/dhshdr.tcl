#+
#  dhshdr.tcl
#
#  Implements a package for sending data headers information to the DHS.
#
#  D Terrett 30 July 2001
#
#  Copyright CCLRC
#+

# Load required packages.
package require Itcl
package require Ocspkg
package require Headerpkg

# Check that the DHS service exists.
if { [llength [info commands dhs]] == 0 } {
   service dhs
}

# Create the dbCtl command sender that is used to get a data label from the
# DHS.
dhs cs bdCtl -command bd

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Create the status acceptor that connects to the TCS status items that
# are need to construct the data headers.
epics sa dhshdr::tcs
epics sa dhshdr::gcal
epics sa dhshdr::gpol

set dirname [file dirname [info script]]
lappend auto_path [file join $dirname scripts]

# Create namespace variables that define the values of header items that have
# to be user settable.
namespace eval dhshdr {
   variable destination <undefined>
   variable debug 0
   variable autodhslabel 1
   variable dhslabel ""
   variable gemprgid ""
   variable obsid ""
   variable datalabel ""
   variable obstype ""
   variable object ""
   variable observer ""
   variable ssa ""
   variable rawiq ""
   variable rawcc ""
   variable rawwv ""
   variable rawbg ""
   variable rawpireq ""
   variable rawgemqa ""

# Populate the dictionary.
   dictload
}

package provide Dhshdr 1.4
