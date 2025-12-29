#! /bin/sh

#
# $Id: compShowEng.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
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
	recordShowDouble ${ccFoc}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccFilt1}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccFilt2}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccFilt3}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccFopl}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccPuvw}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccSplt}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccSter}1Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccSter}2Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${ccCov}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsFilt}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsFoc}Eng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsPrb}xEng
	status=$?
fi

if [ $status -eq 0 ]; then
	recordShowDouble ${wfsPrb}yEng
	status=$?
fi

exit $status
