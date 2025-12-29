#! /bin/sh

#
# $Id: coolRate.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
#
# This script changes the cooling rate.
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
count=1
. ${dir}/scripts/util.sh

if [ $# -ne 1 ]; then
	2>&1 $echo coolRate RATE
	status=1
fi
rate=$1

if [ $status -eq 0 ]; then
	setCoolRate $rate
fi

exit $status 
