#! /bin/sh

#
# $Id: compShow.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
#
# This is a quick and dirty routine to show the current focus position
# in user units.
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
	recordShowDouble ${ccFoc}Pos
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccFilt1}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccFilt2}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccFilt3}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccFopl}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccPuvw}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccSplt}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccSter}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${ccCov}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShow ${wfsFilt}Name
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsFoc}Pos
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsPrb}xPos
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsPrb}yPos
	status=$?
fi

exit $status
