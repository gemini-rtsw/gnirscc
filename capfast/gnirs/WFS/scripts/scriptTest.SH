#! /bin/sh

#
# $Id: scriptTest.SH,v 1.2 2009/05/27 19:35:08 fkraemer Exp $
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
	compMove $@
	return 0
}

init () {
	echo "Testing $@"
	compClear
	return 0
}

go () {
	compPreset
	return 0
}

observe () {
	return 0
}

setTime () {
	return 0
}

setCoolers () {
	return 0
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
