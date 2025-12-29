#! /bin/sh

#
# $Id: compStart.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
#
# This is a quick and dirty routine to clear all marked records.
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -ne 0 ]; then
	2>&1 echo `basename $0`: `basename $0`
	exit 1
fi

if [ $status -eq 0 ]; then
	applyStart ${ccApply} ${wfsApply}
	status=$?
fi

exit $status
