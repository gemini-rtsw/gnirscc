#! /bin/sh

#
# $Id: ccSnapshot.SH,v 1.2 2009/05/27 19:35:06 fkraemer Exp $
#
# This is a quick and dirty routine to save the current state of the
# system for easy restoration.
#

# Don't change the next line.  It's needed by the Makefile.
. .applTop/config.par

status=0
. ${dir}/scripts/util.sh

if [ $# -ne 0 ]; then
	2>&1 echo `basename $0`: `basename $0`
	exit 1
fi

mechs="
	${engCcFilt1}
	${engCcFilt2}
	${engCcFilt3}
	${engCcFoc}
	${engCcSter1}
	${engCcSter2}
	${engCcPuvw}
	${engCcSplt}
	${engCcFopl}
	${engCcCov}
"

for mech in $mechs; do
	if [ $status -eq 0 ]; then
		val=`caGet ${mech}Motor.VAL`
		status=$?
	fi

	if [ $status -eq 0 ]; then
		dval=`caGet ${mech}Motor.DVAL`
		status=$?
	fi

	if [ $status -eq 0 ]; then
		poserr=`caGet ${mech}PosErr.VAL`
		status=$?
	fi

	if [ $status -eq 0 ]; then
		datum=`caGet ${mech}Hallstep.DATM`
		status=$?
	fi

if [ $status -eq 0 ]; then
cat << _EOF_
caput ${mech}Motor.SET 1
caput ${mech}Motor.VAL $val
caput ${mech}Motor.DVAL $dval
caput ${mech}Motor.SET 0
caput ${mech}Hallstep.DATM $datum
caput ${mech}PosErr.VAL $poserr
caput ${mech}Op.VAL 9
_EOF_
fi
done

exit $status
