#! /bin/sh

#
# $Id: scriptRun.SH,v 1.2 2009/05/27 19:35:08 fkraemer Exp $
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -ne 1 ]; then
	2>&1 echo `basename $0`: `basename $0` SCRIPT
	status=1
fi
script=$1

#
# Generic start routine
#

move () {
	echo "Request: " "$@"
	compMove $@
	return 0
}

init () {
	echo ""
	echo "*************************************************************"
	echo "$@"
	compClear
	return 0
}

go () {
	compPreset
	compStart
	compWait
	return 0
}

observe () {
	compShow
	dcGo $@
	return 0
}

setTime () {
	echo "Request: Time " "$@"
	dcSetTime $@
}

setCoolers () {
	echo "Request: Set coolers " "$@"
	setCoolRate $@
}

if [ $status -eq 0 ]; then 
	compClear
	status=$?

	if [ $status -eq 0 ]; then
		set -e
		. ./$script
		unset -e
	fi

	compClear
fi
