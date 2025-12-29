#! /bin/sh

#
# $Id: dcGo.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
#
# This script just starts an integration.  It downloads no code, and
# does no setup, so all preparation must have already been done before
# executing this script.
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
count=1
. ${dir}/scripts/util.sh

if [ $# -gt 1 ]; then
	2>&1 $echo gnaacGo [COUNT]
	status=1
fi

if [ $status -eq 0 -a $# -eq 1 ]; then
	count=$1

	#
	# Sanity check
	#

	expr $count + 0 > /dev/null
	status=$?
fi

while [ $status -eq 0 -a $count -gt 0 ]; do
	$echo Exposures left: $count

	if [ $status -eq 0 ]; then
		name="`caGet $imName`"
		status=$?
	fi

	if [ $status -eq 0 ]; then
		num="`caGet $imNum`"
		status=$?
	fi

	if [ $status -eq 0 ]; then
		printf "Creating: %s%04d.fits\n" $name $num
	fi
		
	if [ $status -eq 0 ]; then
		cadSet $observe
		status=$?
	fi

	if [ $status -eq 0 ]; then
		cadMark $observe
		status=$?
	fi

	if [ $status -eq 0 ]; then
		cadPreset $observe
		status=$?
	fi

	if [ $status -eq 0 ]; then
		cadStart $observe
		status=$?
	fi

	if [ $status -eq 0 ]; then
		$echo -n "Integrating "

		carCWait "$echo -n ." $observeCar
		status=$?

		$echo " Done"
	fi

	if [ $status -eq 0 ]; then
		carShow $observeCar
		status=$?
	fi

	count=`expr $count - 1`
done

exit $status 
