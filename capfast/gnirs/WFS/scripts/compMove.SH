#! /bin/sh

#
# $Id: compMove.SH,v 1.2 2009/05/27 19:35:06 fkraemer Exp $
#
# This is a quick and dirty routine to move the focus to a specified
# location (units are microns).
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -lt 2 ]; then
	2>&1 echo `basename $0`: `basename $0` UNIT POSITION
	status=1
fi
unit=$1
shift

if [ $status -eq 0 ]; then
	case $unit in
	cov|ccCov) unit=${ccCov}; cmd=Sel; nargs=1;;
	filt1|ccFilt1) unit=${ccFilt1}; cmd=Sel; nargs=1;;
	filt2|ccFilt2) unit=${ccFilt2}; cmd=Sel; nargs=1;;
	filt3|ccFilt3) unit=${ccFilt3}; cmd=Sel; nargs=1;;
	foc|ccFoc) unit=${ccFoc}; cmd=Move; nargs=1;;
	fopl|ccFopl) unit=${ccFopl}; cmd=Sel; nargs=1;;
	puvw|ccPuvw) unit=${ccPuvw}; cmd=Sel; nargs=1;;
	splt|ccSplt) unit=${ccSplt}; cmd=Sel; nargs=1;;
	ster|ccSter) unit=${ccSter}; cmd=Sel; nargs=1;;
	wfsFilt) unit=${wfsFilt}; cmd=Sel; nargs=1;;
	wfsFoc) unit=${wfsFoc}; cmd=Move; nargs=1;;
	prb|wfsPrb) unit=${wfsPrb}; cmd=Move; nargs=2;;
	*) 1>&2 $echo "Illegal unit: $unit"; status=1;;
	esac
fi

if [ $status -eq 0 ]; then
	if [ $nargs -ne $# ]; then
		1>&2 $echo "Expected $nargs position parameters, got $#"
		status=1
	fi
fi

if [ $status -eq 0 ]; then
	if [ $nargs -eq 1 ]; then
		cadSet $unit$cmd $1
		status=$?
	elif [ $nargs -eq 2 ]; then
		cadSet $unit$cmd $1 $2
		status=$?
	else
		1>&2 $echo "Only 1 or 2 arguments are supported"
		status=1
	fi
fi

if [ $status -eq 0 ]; then
	cadMark $unit$cmd
	status=$?
fi

exit $status
