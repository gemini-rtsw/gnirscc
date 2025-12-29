#! /bin/sh

#
# $Id: compEngMove.SH,v 1.2 2009/05/27 19:35:06 fkraemer Exp $
#
# This is a quick and dirty routine to move the  to a specified
# location (units are microsteps).
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -ne 2 ]; then
	2>&1 echo `basename $0`: `basename $0` UNIT DEST
	2>&1 echo "Units are microsteps"
	exit 1
fi
unit=$1
dest=$2

case $unit in
filt1|ccFilt1) unit=${ccFilt1};;
filt2|ccFilt2) unit=${ccFilt2};;
filt3|ccFilt3) unit=${ccFilt3};;
fopl|ccFopl) unit=${ccFopl};;
puvw|ccPuvw) unit=${ccPuvw};;
splt|ccSplt) unit=${ccSplt};;
ster1|ccSter1) unit=${ccSter}1;;
ster2|ccSter2) unit=${ccSter}2;;
foc|ccFoc) unit=${ccFoc};;
cov|ccCov) unit=${ccCov};;
prbx|wfsPrbx) unit=${wfsPrb}x;;
prby|wfsPrby) unit=${wfsPrb}y;;
wfsFilt) unit=${wfsFilt};;
wfsFoc) unit=${wfsFoc};;
*) 1>&2 $echo "Illegal unit: $unit"; exit 1;;
esac

if [ $status -eq 0 ]; then
	cadSet ${unit}EngMove $dest
	status=$?
fi

if [ $status -eq 0 ]; then
	cadMark ${unit}EngMove
	status=$?
fi

exit $status
