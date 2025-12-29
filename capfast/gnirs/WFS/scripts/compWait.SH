#! /bin/sh

#
# $Id: compWait.SH,v 1.2 2009/05/27 19:35:07 fkraemer Exp $
#
# This is a quick and dirty routine to move the focus to a specified
# location (units are microns).
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
	$echo -n "Moving "

	carCWait "$echo -n ." ${prefix}cc:applyC ${prefix}wfs:applyC
	status=$?

	$echo " Done"
fi

if [ $status -eq 0 ]; then
	carShow ${ccFilt1}C ${ccFilt2}C ${ccFilt3}C ${ccFoc}C ${ccFopl}C \
		${ccPuvw}C ${spltC} ${ccSter}1C ${ccSter}2C ${ccCov}C ${wfsFilt}C \
		${wfsFoc}C ${wfsPrb}xC ${wfsPrb}yC
	status=$?
fi

exit $status
