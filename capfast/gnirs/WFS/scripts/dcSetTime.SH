#! /bin/sh

#
# $Id: dcSetTime.SH,v 1.2 2009/05/27 19:35:08 fkraemer Exp $
#
# This script just starts an integration.  It downloads no code, and
# does no setup, so all preparation must have already been done before
# executing this script.
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -ne 1 ]; then
	1>&2 echo `basename $0`: `basename $0` TIME
	status=1
fi
time=$1

if [ $status -eq 0 ]; then
	$caput $integTime $time > /dev/null
	status=$?
fi

if [ $status -eq 0 ]; then
	cadMark $obsSetup
	status=$?
fi

if [ $status -eq 0 ]; then
	cadPreset $obsSetup
	status=$?
fi

if [ $status -eq 0 ]; then
	cadStart $obsSetup
	status=$?
fi

if [ $status -eq 0 ]; then
	$echo -n "Setting Time "
	carCWait "$echo -n ." $obsSetupCar
	status=$?
	$echo " Done"
fi

if [ $status -eq 0 ]; then
	carShow $obsSetupCar
	status=$?
fi

exit $status
