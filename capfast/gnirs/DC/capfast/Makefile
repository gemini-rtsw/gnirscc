# Makefile,v 1.2 1995/09/02 01:45:00 epics Exp
#
# Make postscript target added.
#

include .applTop/config/CONFIG

APPLIC_DIR_TYPE = capfast

include .applTop/config/RULES.Dirs

postscript:
	makePostscript `find . -name "*.sch" -type f -print`

