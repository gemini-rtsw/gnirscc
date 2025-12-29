#+
#  dhshdr.tcl
#
#  Implements a package for sending data headers information to the DHS.
#
#  M Bec     31 July 2001
#
#  Copyright CCLRC
#+

# Load required packages.
package require Itcl
package require Ocspkg
package require Headerpkg

package require -exact Dhshdr 1.4

# Check that the EPICS service exists.
if { [llength [info commands epics]] == 0 } {
   error "unable to load package: EPICS ocs service does not exist"
}

# Create the status acceptor that connects to various status items that
# are need to construct the data headers.
epics sa dhshdr::niri


# load additional keywords entries
::dhshdr::HdrNiri::dictload

package provide niriDhshdr 1.4
